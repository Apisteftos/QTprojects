# 13_cpp_signals_to_qml

Properties tell QML **what is**. Signals tell QML **what happened**. This lesson shows a C++ object that works on its own (a timer produces readings) and tells QML about them, and a **singleton**: one object that every QML file can use by its name.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; one is an explicit `import cpp_signals_to_qml` in `Main.qml`, which Qt 6.8 does not need). An automated check tested `Sensor` in C++ with `QSignalSpy` (signal counts, alarm arguments, start and stop, the real timer) and then the window (22 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A simulated temperature sensor:

- A big value and a bar that follow the readings (properties with `NOTIFY`).
- **Start** and **Stop** run the C++ timer. **One reading** takes a single reading by hand.
- Two sliders set the timer **interval** and the **alarm limit** (writes to C++ properties).
- A list of **alarms**: each time the value goes above the limit, the C++ object sends a signal, and QML adds a line.

The readings are a smooth wave between 5 and 35, calculated from the reading number, so every run gives the same values.

---

## 2. A Signal with Arguments

In `sensor.h`:

```cpp
signals:
    void alarm(double value, int reading);
```

In `sensor.cpp`, the object **emits** it:

```cpp
if (previous <= m_threshold && m_value > m_threshold)
    emit alarm(m_value, m_readings);
```

Two kinds of signals in this class:

| Signal | Kind | Used by |
|--------|------|---------|
| `valueChanged()`, `runningChanged()` ... | **Notify signals** of properties: "this value changed" | Bindings in QML (`text: Sensor.value`) |
| `alarm(double, int)` | An **event** that carries data: "this just happened" | Handlers in QML |

A notify signal says that a **state** changed, and QML reads the new value. An event signal is **not a state**: it happens once. The alarm is sent when the value crosses the limit **upwards** (previous value at or below the limit, new value above), not at every reading above it. For something that should be sent only once, put the condition for the edge in C++.

---

## 3. Reacting in QML

```qml
Connections {
    target: Sensor
    function onAlarm(value, reading) {
        alarms.insert(0, { message: qsTr("Reading %1: %2 is above the limit").arg(reading).arg(value.toFixed(1)) })
    }
}
```

- `Connections` has a `target` (the sender), and a **function** for each signal: the name is `on` + the signal name with a capital letter, and the parameters have the **same names** as in C++ (lesson 05).
- You can also use the handler form `onAlarm` directly on an object that has the signal, if the object is declared in QML. A C++ object that QML did not create (like the singleton) is reached with `Connections`.
- **Properties** need no handler: the notify signal is used by the binding machine.

**Direction**: QML calls C++ with methods and property writes (lesson 12), C++ calls QML with **signals**. C++ should not know QML items. It only emits a signal, and does not care who listens. This keeps the class usable in a test (see `QSignalSpy` below), in Qt Widgets, or with several listeners.

---

## 4. `QML_SINGLETON`

```cpp
class Sensor : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    ...
};
```

With `QML_SINGLETON` the engine creates **one** `Sensor` object, the first time QML uses the name, and keeps it for the whole program. In QML, you write the name and use it:

```qml
Text { text: Sensor.value.toFixed(1) }
Button { onClicked: Sensor.start() }
```

You cannot write `Sensor { }`, and you need no `id`. Every QML file of the module (and of a module that imports it) sees the **same** object. This is the right tool for one shared service: settings, a backend, a connection, a theme, a logger.

Notes:

- The class must have a default constructor, or a `static create(QQmlEngine *, QJSEngine *)` function (see the Qt documentation).
- A singleton that is **really global** makes the code harder to test and to reuse. For state that belongs to one screen, create a normal object (lesson 12) and give it to the screen. Use a singleton for what is one for the whole application.
- Other ways to give a C++ object to QML: `engine.rootContext()->setContextProperty("backend", &backend)` (old, works, but QML tools cannot see the type) and `engine.setInitialProperties({{"backend", QVariant::fromValue(&backend)}})` for an object that **main.cpp** owns and gives to the root item of the QML file (lesson 20 uses this).

---

## 5. Objects That Work on Their Own

The sensor has a `QTimer` as a member:

```cpp
connect(&m_timer, &QTimer::timeout, this, &Sensor::advance);
```

The timer works in the thread of the object (the main thread), through the event loop (chapter `01_Fundamentals`, lesson 04). `advance()` is also a `Q_INVOKABLE` method, so the **One reading** button and the test can call it at once, with no waiting. That is a good pattern: the timer only calls a function that is also useful without a timer.

A slow reading must not be done in `advance()`: it would freeze the interface. A long job belongs to a thread or a `QtConcurrent` task (chapter `09_Concurrency`), and it sends a signal when it is ready.

---

## 6. Test the Signals in C++

The test did not need QML for the logic:

```cpp
Sensor s;
QSignalSpy alarms(&s, &Sensor::alarm);
for (int i = 0; i < 60; ++i)
    s.advance();
QVERIFY(alarms.count() >= 3);                       // the wave crossed the limit 4 times
QCOMPARE(alarms.first().at(1).toInt(), ...);        // the arguments of the signal
```

`QSignalSpy` records the signals of an object. It sees what QML would see. In the test the sensor made four alarms in 60 readings, none when the limit was 100, and about 6 readings in 330 ms with an interval of 50 ms (the real timer).

---

## 7. A Trap: a Role Named `text`

In the first version of the list delegate, the role of the model was called `text`:

```qml
delegate: Text {
    required property string text
    text: text          // WRONG: the property refers to itself
}
```

The alarm list stayed empty. `Text` already has a property `text`, and the line `text: text` is a binding to itself. The test found it because no alarm text was in the window. Give model roles names that are **not** properties of the delegate item: here `message`.

---

## 8. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app13_cpp_signals_to_qml
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Click **One reading** about 12 times and watch the alarm list.
2. Click **Start**, move the interval slider, then move the alarm limit slider.
3. Add a signal `void reset()` to the sensor and a method that emits it. Handle it in `Connections`.
4. Change the alarm rule to "every reading above the limit" and watch the list fill.
5. Add a second `Connections` that writes the value to the output of the program with `console.log` at every `onValueChanged`.
6. Make the `Sensor` a normal object: remove `QML_SINGLETON`, and create it in `Main.qml` with `Sensor { id: sensor }`. What must change in the other lines?

---

## 9. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `ReferenceError: Sensor is not defined` | The class has no `QML_SINGLETON`, or it is not in `SOURCES`, or the module is not imported | Check these |
| The handler is never called | The name is wrong, or the parameter names are different | `function onAlarm(value, reading)`; check the signal |
| The handler gets `undefined` | The parameter is not named, or the type is not known to QML | Use basic types, or register the type |
| The value does not update | The property has no `NOTIFY`, or the signal is not emitted | Add and emit it |
| The singleton cannot be created | No default constructor | Add one, or a static `create()` function |
| The signal is emitted, but the C++ listener is not called | The `connect()` was made with a wrong signature | Use the pointer-to-member form (checked by the compiler) |
| An alarm is sent at every reading | The condition is not an edge | Compare the previous value with the new one, as in `advance()` |
| The interface freezes | The slot does slow work | Move it to a thread (chapter 09) |

---

## 10. Reference

| Topic | Where to look |
|-------|---------------|
| Signals and slots | <https://doc.qt.io/qt-6/signalsandslots.html> |
| Attaching C++ signals to QML | <https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html#exposing-signals> |
| `QML_SINGLETON` | <https://doc.qt.io/qt-6/qqmlintegration.html#QML_SINGLETON> |
| Singletons in QML | <https://doc.qt.io/qt-6/qml-singleton.html> |
| `Connections` | <https://doc.qt.io/qt-6/qml-qtqml-connections.html> |
| `QTimer` | <https://doc.qt.io/qt-6/qtimer.html> |
| `QSignalSpy` | <https://doc.qt.io/qt-6/qsignalspy.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`); `QSignalSpy` is in Qt Test (only for the test) |
| Headers | `<QObject>`, `<QTimer>`, `<QtMath>`, `<QtQml/qqmlregistration.h>` |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
