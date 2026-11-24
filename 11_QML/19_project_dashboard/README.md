# 19_project_dashboard

A project for the end of the chapter: a **dashboard** with live numbers, a bar chart and an alarm message, all animated, with a **C++ backend** that produces the data. It uses what the earlier lessons taught: components (07), states and transitions (08), animations (09), models and views (10), properties and signals of C++ classes (12, 13).

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; one is an explicit `import project_dashboard` in `Main.qml`, which Qt 6.8 does not need). An automated check tested the `Dashboard` class alone (signals, limits, the history, `reset`, the timer) and clicked through the window: steps, the cards, the bars, the alarm message, reset and the slider (62 checks, all passed). Animations were run offscreen, so the look of the movement was not judged: please build it once with your own 6.8+ kit and look at it.

---

## 1. What You Build

| File | Content |
|------|---------|
| `dashboard.h`, `dashboard.cpp` | The backend: simulated measurements, a level, a history, an alarm signal |
| `MetricCard.qml` | A card with a title and a number that counts to its new value; the colours follow the level |
| `BarChart.qml` | The history as bars; the heights are animated |
| `Main.qml` | The window: header, three cards, the chart, the interval slider, the alarm message |

Start the program. The data are **simulated** (two waves), so the program runs everywhere and a test gets the same numbers every time. With a real machine, only `Dashboard::advance()` changes: it would read a sensor or a network answer (lesson 15) instead of calling `loadAt`.

---

## 2. The Backend

`Dashboard` has one job: produce data. It has no idea how they are shown.

```cpp
Q_PROPERTY(double load READ load NOTIFY changed)
Q_PROPERTY(QList<double> history READ history NOTIFY changed)
Q_PROPERTY(QString level READ level NOTIFY levelChanged)
Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
signals:
    void alarmRaised(const QString &level, const QString &message);
```

Three decisions are worth a look:

1. **One signal for the measurements.** `load`, `temperature`, `requests` and `history` change together, so one signal `changed()` serves all of them. `advance()` first sets **all** values and then sends the signal, so no binding sees half of a measurement (the new load with the old temperature).
2. **State and event are different things.** `level` is a **state**: a property with a signal, and the cards bind to it. `alarmRaised` is an **event**: it happens once, when the level gets **worse**, and it carries a message. The test counted: no alarm when the level gets better, and exactly one alarm for each step to a worse level.
3. **The limits are in the class.** `setInterval` keeps the value between 100 and 2000 ms (`qBound`), whatever the QML code writes. A slider can be wrong; the backend must not depend on it.

`advance()` is an ordinary `Q_INVOKABLE` function. The timer calls it, the button "One step" calls it, and the test calls it. **Code that can be called directly is easy to test.**

`QML_ELEMENT` registers the class, so QML creates it like any element:

```qml
Dashboard {
    id: dashboard
    interval: intervalSlider.value
    Component.onCompleted: running = true
}
```

`interval: intervalSlider.value` is a binding on a C++ property (lesson 12), and the slider drives the timer.

A text like `"warning"` is simple, but a typo (`"warnning"`) is found only at run time. A real program would use an `enum class` with `Q_ENUM` (see the Qt documentation for `Q_ENUM`) so that QML writes `Dashboard.Warning`.

---

## 3. MetricCard: A Number That Counts

```qml
property real value: 0
property real shown: value
Behavior on shown { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }
...
text: card.shown.toFixed(card.decimals) + " " + card.unit
```

`value` is the truth, `shown` is the picture. When `value` changes, `shown` runs to it in 400 ms, and the text follows `shown`. This is the pattern for every number that should not jump. In a test use `tryCompare(card, "shown", 75.9)` to wait for the end of the animation.

The colours are **states** (lesson 08):

```qml
states: [
    State { name: "warning";  when: card.level === "warning"
            PropertyChanges { target: card; color: "#fff4d6"; border.color: "#e0a800" } },
    State { name: "critical"; when: card.level === "critical"; ... }
]
transitions: Transition { ColorAnimation { duration: 300 } }
```

The card knows only a `level` text. It does not know the backend, so the same card works for any data. Look at `Main.qml`: all three cards bind `level: dashboard.level`.

---

## 4. BarChart: Make the Bars Once

The history changes at every measurement. If the `Repeater` used the list itself as its model, QML would **destroy and create all bars** at every measurement, and no animation could run. So the model is a **number**:

```qml
Repeater {
    model: chart.capacity            // 24 delegates, made once
    delegate: Item {
        required property int index
        readonly property real v: index >= offset ? chart.values[index - offset] : 0
        ...
        Rectangle { height: chart.height * slot.v / chart.maximum
                    Behavior on height { NumberAnimation { duration: 250 } } }
    }
}
```

Each delegate reads **its** value, and the `Behavior` animates the height. The newest value is at the right; while there are fewer values than bars (at the start), the left bars stay empty (the test checks that the first bar is empty and the last bar is not, after seven steps). The bar colour follows two limits, `warningAt` and `criticalAt`, which are properties of the chart.

If you need to add, remove and move single entries, a model class (lesson 14) is the better tool. For a fixed number of slots, this is shorter.

---

## 5. The Alarm Message

```qml
Connections {
    target: dashboard
    function onAlarmRaised(level, message) {
        toastText.text = message
        toast.accent = level === "critical" ? "#c0392b" : "#e0a800"
        toastAnimation.restart()
    }
}
SequentialAnimation {
    id: toastAnimation
    NumberAnimation { target: toast; property: "opacity"; to: 1; duration: 150 }
    PauseAnimation  { duration: 1800 }
    NumberAnimation { target: toast; property: "opacity"; to: 0; duration: 400 }
}
```

An event from C++ (lesson 13) starts an animation in QML. `restart()` means: a new alarm while the old message is still visible starts the three steps again.

---

## 6. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app19_project_dashboard
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Let it run. Watch the cards change colour when the load goes above 60 and 85, and read the messages.
2. Switch "Live" off and use "One step". Press "Reset".
3. Move the slider to 100 ms and to 2000 ms. What happens to the animation of the cards (400 ms) at 100 ms?
4. Add a fourth card, "Errors", with a new property `errors` in `Dashboard` (a number that grows when the level is critical).
5. Change the thresholds of the level: make them properties of `Dashboard` (`warningAt`, `criticalAt`) with a `NOTIFY`, and bind the `BarChart` to them (`warningAt: dashboard.warningAt`).
6. Make `Dashboard` write each measurement into a file (`QFile`, chapter `10_IO_and_Data`) in `advance()`.
7. Replace the bar chart by a `ListView` of the last ten measurements with a model class (lesson 14).

---

## 7. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `Dashboard is not a type` | The class is not in `SOURCES`, or has no `QML_ELEMENT` | Check `CMakeLists.txt` and the class (needs `Q_OBJECT`, `QML_ELEMENT`, `#include <QtQml/qqmlregistration.h>`) |
| The numbers jump (no animation) | The `Behavior` is on `value` of a read-only binding, or missing | Put the `Behavior` on the property that the text shows (`shown`) |
| All bars jump at every measurement | The list is the model of the `Repeater`, so all bars are made again | Use a number as the model, as here |
| A binding does not update | The property has no `NOTIFY`, or the signal is not sent | Send the signal after every change |
| The animation is cut at a short interval | The animation (400 ms) is longer than the interval (100 ms) | Animations restart from the current value; shorten them or limit the interval |
| The window is empty at the start | `running` was set before the engine loaded the QML | Set it in `Component.onCompleted`, as here |
| The alarm message stays | `restart()` is called before the old animation ended, and the pause is long | Use `restart()` as here; shorten the `PauseAnimation` |
| A test is flaky | It waits for a timer | Call `advance()` directly, as the tests of this lesson do |

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Exposing C++ to QML | <https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html> |
| `Q_PROPERTY` | <https://doc.qt.io/qt-6/properties.html> |
| `QTimer` | <https://doc.qt.io/qt-6/qtimer.html> |
| `Behavior`, `NumberAnimation`, `SequentialAnimation` | <https://doc.qt.io/qt-6/qml-qtquick-behavior.html>, <https://doc.qt.io/qt-6/qml-qtquick-sequentialanimation.html> |
| `State`, `Transition` | <https://doc.qt.io/qt-6/qml-qtquick-state.html>, <https://doc.qt.io/qt-6/qml-qtquick-transition.html> |
| `Repeater` | <https://doc.qt.io/qt-6/qml-qtquick-repeater.html> |
| `Connections` | <https://doc.qt.io/qt-6/qml-qtqml-connections.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Headers | `<QObject>`, `<QTimer>`, `<QList>`, `<QtQml/qqmlregistration.h>` |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
