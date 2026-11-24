# 12_cpp_properties_and_methods

Now the C++ side. The chapter rule is **QML shows and reacts, C++ decides and computes**. This lesson shows how a C++ class becomes a QML type: its data are **properties** (`Q_PROPERTY`), its actions are **methods** (`Q_INVOKABLE`), and `QML_ELEMENT` registers it. The example is a counter with a step and a limit.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; one of them is an explicit `import cpp_properties_and_methods` in `Main.qml`, which Qt 6.8 does not need). An automated check tested the C++ class **without QML** and then the whole window with clicks and the two spin boxes (31 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A window that shows a counter value from 0 up to a limit:

- A big number, a progress bar and a summary text, all bound to the C++ object.
- Buttons for `+step`, `-step` and **Reset**. The `+` button reports when the limit stops it.
- Two spin boxes that set the **step** and the **limit** of the C++ object.
- A **Describe** button that calls a C++ method and shows its answer.

The window has no logic: it has no `if` that decides how much to add, or what the limit means. That is all in `Counter`.

---

## 2. The Files

| File | Job |
|------|-----|
| `counter.h`, `counter.cpp` | The class `Counter` (plain C++ and Qt Core) |
| `Main.qml` | The window. It uses `Counter { }` like any other item |
| `main.cpp` | The usual starter (lesson 01). It does not mention `Counter` at all |
| `CMakeLists.txt` | Lists the C++ files in the QML module |

```cmake
qt_add_qml_module(app12_cpp_properties_and_methods
    URI cpp_properties_and_methods
    QML_FILES
        Main.qml
    SOURCES
        counter.h
        counter.cpp
)
```

`SOURCES` puts the C++ files **into the QML module**. `qt_add_qml_module` then runs moc and the QML type registration for them. The URI `cpp_properties_and_methods` is the module of the type `Counter`. In Qt 6.8, the QML files of the same module can use `Counter` with no `import`. A C++ class that is in another module needs `import thatmodule`.

---

## 3. `QML_ELEMENT`

```cpp
class Counter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    ...
};
```

- The class must derive from `QObject` and have `Q_OBJECT` (the meta-object is the table that QML reads).
- `QML_ELEMENT` registers the class under **its own name**: `Counter`. `QML_NAMED_ELEMENT(Name)` uses another name. `QML_SINGLETON` makes it a singleton (lesson 13), `QML_UNCREATABLE("reason")` allows it only as a property type or an attached object.
- The header `<QtQml/qqmlregistration.h>` defines the macros.
- The registration happens at start-up by the generated code. There is **no** `qmlRegisterType` call in `main.cpp`, as in old Qt 5 programs.
- The object is **created by QML** (`Counter { id: counter }`) and **owned by QML**: when the item that contains it is destroyed, the object is deleted. You do not call `delete`.
- It needs a constructor with the form `Counter(QObject *parent = nullptr)`.

---

## 4. `Q_PROPERTY`

```cpp
Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
```

| Part | Meaning |
|------|---------|
| type and name | The type and the name that QML sees: `counter.value` |
| `READ` | The getter: a `const` function that returns the value |
| `WRITE` | The setter. A property without it is **read-only** from QML (`atLimit`, `summary`) |
| `NOTIFY` | The signal that the class emits **when the value changes**. This is what makes a binding in QML update |
| `STORED false` | The value is calculated, not stored (`atLimit`). It is only a note for tools |

Rules for a setter (see `Counter::setValue`):

1. **Check the value** and keep the object valid. `std::clamp` keeps the value between 0 and the limit. The window does not need to know the limit rule.
2. **Return early when nothing changed.** `if (value == m_value) return;` avoids a signal and a lot of work in QML.
3. **Emit the notify signal after the change.**

A calculated property (`atLimit`, `summary`) has no setter and needs a notify signal that is emitted when **any value it depends on** changes. `atLimit` uses `valueChanged` (the limit rule is applied in `setLimit`, which calls `setValue`); `summary` has its own signal `summaryChanged`, because it depends on value and limit.

In QML, the property is used like a normal property:

```qml
Text { text: counter.value }                       // a binding: updates by itself
SpinBox { onValueModified: counter.step = value }  // a write: calls setStep()
```

Other attributes you will meet: `CONSTANT` (never changes, no notify), `REQUIRED` (QML must set it), `FINAL`, `BINDABLE` (Qt 6 bindable properties, see the Qt documentation).

---

## 5. `Q_INVOKABLE` Methods

```cpp
Q_INVOKABLE bool increment();
Q_INVOKABLE QString describe(const QString &unit) const;
```

A normal member function is **not** visible to QML. `Q_INVOKABLE` makes it callable: `counter.increment()`. Arguments and the return value are converted: `QString` is a JavaScript string, `bool` a boolean, `int` and `double` numbers. A public **slot** (`public slots:`) is callable from QML as well, and so is a signal.

| Use | When |
|-----|------|
| A **property** | The thing is a state or a value that the interface shows |
| A **method** | The thing is an action ("reset") or a question with arguments ("describe in this unit") |
| A **signal** | The thing is an event that C++ reports to QML (lesson 13) |

---

## 6. A Trap: Methods in Bindings Do Not Update

The first version of this lesson showed the summary with this line:

```qml
Text { text: counter.describe(qsTr("points")) }    // WRONG
```

The test showed that the text stayed at `0 of 10 points` while the value went up. QML can find out that `counter` was read, but **not what the C++ function read inside**. A method has no notify signal, so the binding is calculated once and never again.

Fix: make the result a **property with a notify signal**, as `summary` is, and use it in the binding. Call a method only in a **handler** (`onClicked`), where you want the answer of that moment, as the **Describe** button does.

---

## 7. QML and C++ Types

| QML type | C++ type |
|----------|----------|
| `int` | `int` |
| `real`, `double` | `double`, `qreal`, `float` |
| `bool` | `bool` |
| `string` | `QString` |
| `url` | `QUrl` |
| `color` | `QColor` |
| `date` | `QDateTime`, `QDate` |
| `var`, `variant` | `QVariant` (any of them) |
| list or array | `QList<T>` of those types, or `QVariantList` |
| object | a `QObject *` (a registered class, or any `QObject` as `QObject *`) |

Your own enum can be given to QML with `Q_ENUM(MyEnum)`, and a value type with `Q_GADGET`. Both are in the Qt documentation (see the Reference).

---

## 8. Test the Class Without QML

`Counter` includes only `QObject`, so a C++ test can create it directly:

```cpp
Counter c;
c.setStep(4);
QVERIFY(c.increment());
QCOMPARE(c.value(), 4);
```

The test of this lesson did that first: the step, the limit, the clamp, the early return (one `valueChanged` for two equal calls) and `describe()`. Then it checked the window. This is the main reason to put logic in C++: you can test it without a screen (chapter `12_BuildTestDeploy`, lesson 01).

---

## 9. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app12_cpp_properties_and_methods
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Click **+2** until the limit. Read the red message and the text under it.
2. Change the limit to 4 while the value is 8. What happens to the value? Which line of C++ does it?
3. Add a property `bool even` that is true when the value is even, with a notify signal, and show it in a `Text`.
4. Change the summary text in the C++ code (not in QML) and rebuild.
5. Remove `emit summaryChanged();` from `setValue`. What happens to the summary text?
6. Make `limit` read-only: remove `WRITE setLimit` and set the limit in the constructor. What does QML say about `counter.limit = value`?

---

## 10. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `Counter is not a type` | The class is not in `SOURCES`, has no `QML_ELEMENT`, or CMake did not run again | Check all three, run CMake again |
| `module "..." is not installed` | The URI in `loadFromModule` and in CMake are different | Make them the same |
| The text does not update | The property has no `NOTIFY`, or the signal is not emitted | Add and emit it |
| A binding with a method call is stale | A method has no notify signal | Use a property (section 6) |
| `TypeError: Property 'foo' of object Counter is not a function` | The method has no `Q_INVOKABLE` | Add it |
| `Cannot assign to read-only property` | The property has no `WRITE` | Add the setter |
| Linker error `undefined reference to vtable for Counter` | The header was not processed by moc | Header in `SOURCES`, `Q_OBJECT` in the class, run CMake again |
| QML says `Unable to assign QString to int` | The C++ type and the QML value are different | Convert, or change the type |

---

## 11. Reference

| Topic | Where to look |
|-------|---------------|
| Exposing attributes of C++ classes to QML | <https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html> |
| Defining QML types from C++ | <https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html> |
| Overview: QML and C++ integration | <https://doc.qt.io/qt-6/qtqml-cppintegration-overview.html> |
| The property system | <https://doc.qt.io/qt-6/properties.html> |
| `qt_add_qml_module` | <https://doc.qt.io/qt-6/qt-add-qml-module.html> |
| `QML_ELEMENT` and the other macros | <https://doc.qt.io/qt-6/qqmlintegration.html> |
| `Q_ENUM`, `Q_GADGET` | <https://doc.qt.io/qt-6/qobject.html#Q_ENUM> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Headers | `<QObject>`, `<QtQml/qqmlregistration.h>` |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
