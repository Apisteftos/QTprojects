# 18_testing_qml

A program that works today can break tomorrow, when you change one line. Tests check it again, in seconds, every time. This lesson tests a small QML form with **Qt Quick Test** (QML tests written in QML), and then shows three more tools: **qmllint** (finds mistakes without running), **qmlformat** (one code style), and the **QML Profiler** (finds what is slow).

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; the module `qml6-module-qttest` had to be installed there). The test program ran 29 results (rows of the data tests included), all passed. A copy of `Stepper.qml` with one deliberate bug (`value += 2`) made three tests fail, so the tests really check something. `qmllint` of 6.4.2 reported no warning for the files of this folder. The QML Profiler (section 6) was **not** tried here. Please build it once with your own 6.8+ kit.

---

## 1. What You Build

| File | Content |
|------|---------|
| `logic.js` | Three small functions: `clamp`, `formatPrice`, `isValidName` |
| `Stepper.qml` | A number with a "-" button and a "+" button, limits, and a signal `limitReached` |
| `OrderForm.qml` | A form: name, quantity (`Stepper`), total, order button |
| `Main.qml` | A window with the form |
| `tests/tst_logic.qml` | Tests of the functions (data-driven) |
| `tests/tst_stepper.qml` | Tests of `Stepper`: methods, signals, clicks |
| `tests/tst_orderform.qml` | Tests of `OrderForm`: typing, clicking, the whole order |
| `tests/tst_main.cpp` | The runner (three lines) |

Run the program and click around once. Then run the tests.

---

## 2. Running the Tests

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Or start the test program itself, which prints every test:

```bash
./build/tst_testing_qml
```

With a computer that has no screen (a server, a CI job), use the offscreen platform. The `CMakeLists.txt` already sets it for `ctest`; by hand:

```bash
QT_QPA_PLATFORM=offscreen ./build/tst_testing_qml
```

Qt Creator shows CTest tests in the **Test Results** pane after **Build > Run Test** (or through the "Tests" mode, depending on your version).

The runner is this:

```cpp
#include <QtQuickTest>
QUICK_TEST_MAIN(qml_tests)
```

`QUICK_TEST_MAIN` writes `main()`: it starts Qt Quick, finds every `tst_*.qml` file in `QUICK_TEST_SOURCE_DIR` (set in `CMakeLists.txt`) and runs them. The test program is a **separate executable** (`tst_testing_qml`), so the real program has no test code.

The same files also run with the ready-made tool `qmltestrunner -input tests` that comes with Qt, if you have no C++ runner.

---

## 3. A TestCase

```qml
import QtQuick
import QtTest

TestCase {
    name: "Logic"

    function test_clamp() {
        compare(Logic.clamp(5, 0, 10), 5)
        compare(Logic.clamp(99, 0, 10), 10)
    }
}
```

Every function whose name starts with `test_` is a test. The output says `PASS : qml_tests::Logic::test_clamp()`.

### 3.1 The checks

| Function | Meaning |
|----------|---------|
| `compare(actual, expected)` | Equal? On a difference the output shows both values. **Actual first, expected second** |
| `verify(condition, message)` | True? Use it when there is no value to show |
| `fuzzyCompare(a, b, delta)` | For numbers with decimals: equal within `delta` |
| `tryCompare(object, "property", value)` | Waits (up to 5 seconds) until the property has the value. For timers and animations |
| `tryVerify(function)` | The same with a condition |
| `wait(ms)` | A fixed pause. Avoid it: `tryCompare` is faster and safer |
| `skip("reason")` | The test is skipped, and it is listed as skipped |
| `expectFail(tag, message)` | The next check is allowed to fail (a known bug) |
| `fail("message")` | Fails at once |

A failed check ends the test function (the next lines do not run). The failure has the line of the QML file.

### 3.2 Before and after

| Function | Runs |
|----------|------|
| `initTestCase()` / `cleanupTestCase()` | Once for the whole TestCase |
| `init()` / `cleanup()` | Before / after **every** test function |

Tests must not depend on each other: each one makes its own object in `init()` or in the test.

```qml
Component { id: stepperComponent; Stepper { from: 0; to: 3 } }

function init() {
    stepper = createTemporaryObject(stepperComponent, testCase)   // destroyed after the test
}
```

`createTemporaryObject(component, parent)` makes an object and deletes it when the test function ends. Without it, the objects of old tests stay alive.

### 3.3 Data-driven Tests

Many inputs, one test:

```qml
function test_formatPrice_data() {
    return [
        { tag: "zero",       cents: 0,    expected: "0.00" },
        { tag: "with cents", cents: 1250, expected: "12.50" }
    ]
}
function test_formatPrice(data) {
    compare(Logic.formatPrice(data.cents), data.expected)
}
```

The function with `_data` returns the rows. The test runs once per row with `data`, and the output shows the `tag`: `test_formatPrice(with cents)`. A new case is one new line. The `tag` is the first thing you read when a row fails, so write it for humans. Look at the boundary values: 20 and 21 characters, 0 and 5 cents.

---

## 4. Testing the Interface

```qml
TestCase {
    visible: true
    when: windowShown
    ...
    mouseClick(findChild(stepper, "plus"))
    keyClick("A")
}
```

- **`when: windowShown`**: the test starts when the window is shown. Without it, mouse and key events can be lost.
- **`visible: true`**: a `TestCase` is **invisible** by default, and so are its children. Mouse clicks do not reach invisible items, and a `Label` with `visible: true` still reports `visible` as `false` when its parent is not visible. The first version of this lesson missed this line: three tests failed (the click did nothing, and the hint label did not show). If a test of the interface fails for no reason, check this first.
- **`findChild(parent, "name")`** finds an item by its `objectName`. Give the items that tests need an `objectName`. It is a name for the test; it has no other effect.
- **`mouseClick(item)`** clicks the middle of the item. A click on a disabled button does nothing, as for a user (the test shows it).
- **`keyClick("A")`** sends a key to the item with the **active focus**. Get the focus first: `field.forceActiveFocus()`, and `verify(field.activeFocus)`. There is no "type a whole text" call, so the helper `typeName` in `tst_orderform.qml` sends one key per letter. For simple cases, set `field.text = "Al"` directly.
- **`SignalSpy`** counts the signals of an object:

```qml
SignalSpy { id: limitSpy; signalName: "limitReached" }
...
limitSpy.target = stepper
stepper.increment()
compare(limitSpy.count, 1)
compare(limitSpy.signalArguments[0][0], "max")     // first signal, first argument
```

- **Test the behaviour, not the details.** "After a click on plus the total is 5.00" is a good test. "The third child of the layout is a Label" breaks when you move things.

### 4.1 What to Test

| Test | Example here |
|------|--------------|
| Functions without interface | `tst_logic.qml`: the easiest and the fastest |
| Methods and signals of a component | `Stepper`: `increment()`, `limitReached` |
| One user story | `OrderForm`: type a name, click plus, click order, look at the status |
| Boundaries | The limit of the stepper, 20 and 21 characters |
| A bug that was found once | `test_noSignalInTheMiddle`: the test stays, so the bug cannot come back |

What is hard to test with QML tests: the look (colours, pixels), and animations. Test their **end state** with `tryCompare`.

### 4.2 A Test Must Be Able to Fail

A test that never fails is worth nothing. Break the code on purpose once (change `value += 1` to `value += 2` in `Stepper.qml`), run the tests, and see them fail, then undo it. That is what the test status above did.

---

## 5. qmllint and qmlformat

### qmllint

Finds mistakes **without running** the program:

```bash
qmllint Main.qml OrderForm.qml Stepper.qml
```

A file with typing mistakes (the sandbox test):

```qml
Text { text: root.cuont }          // the property is "count"
Rectangle { width: parent.widht }
```

```
Warning: Bad.qml:7:20: Property "cuont" not found on type "Item"
Info: Did you mean "count"?
Warning: Bad.qml:11:23: Property "widht" not found on type "Item"
```

Without `qmllint`, these lines only print an empty value or a console message when they run, and perhaps only in a rare path of the program. qmllint also reports an unused import. It found one in the first version of `Main.qml` (`import QtQuick.Layouts` was not used), and it was removed.

With `qt_add_qml_module`, CMake makes the targets for all of a module's files: `cmake --build build --target all_qmllint` (and one per module, named `<target>_qmllint`). In Qt Creator, the code model shows the same warnings in the editor. Use the Qt Creator setting for the QML language server (`qmlls`) to see them while you type.

### qmlformat

One code style for all files:

```bash
qmlformat -i Main.qml          # -i: change the file
```

Teams run it before a commit or in the CI job. Be careful: it changes **many** lines the first time, and different Qt versions format a little differently (the Qt 6.4 version used in the sandbox added a semicolon to every line and spread a data table of the tests over many lines). So this lesson does **not** run it over its own files. Agree on one Qt version for the team, and run it once over the project in its own commit.

---

## 6. The QML Profiler

When a program is slow, do not guess: measure. In Qt Creator:

1. Open the project and choose a kit with a **debug** or **release with debug info** build.
2. **Analyze > QML Profiler** (the exact menu names depend on the Qt Creator version). The program starts.
3. Use the program for the slow action, then stop it. The profiler shows a timeline.

| What you see | What it tells you |
|--------------|-------------------|
| **Bindings** and **JavaScript** with times and counts | Which binding runs too often or takes too long |
| **Creating** | Which components are created, and how long it takes (a `Loader` or a `delegate` of a list) |
| **Animations** and the frame rate | Frames that take longer than 16 ms (at 60 frames per second) |
| **Painting** and **Scene Graph** | Too many items, or too much drawing |

Typical findings: a binding with a long calculation that is evaluated again for every item of a list; a `ListView` with `delegate`s that are too heavy; a `Timer` that starts a lot of work. The usual fixes are a `Loader`, a smaller delegate, and moving the calculation into C++ (lessons 12 to 14). The command line tool `qmlprofiler` also exists, for the same recordings without Qt Creator.

---

## 7. Try This

1. Run the tests. Then add a fifth row to `test_isValidName_data()`: a name of exactly 2 spaces and a letter (`" A "`). Is it valid? What does the code say? Is that what you want?
2. Add a test for `OrderForm` that checks the status text is empty at the start.
3. In `Stepper.qml`, remove the `enabled` binding of the plus button. Which test fails?
4. Change `to: 5` in `OrderForm.qml` to `to: 2`. Which tests fail, and is the test or the code wrong?
5. Write `Stepper` with a `step` property (1 by default), and write the tests **first**.
6. Run `qmllint` over your own lessons of this chapter. Does it find anything?

---

## 8. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `module "QtTest" is not installed` | The Qt Quick Test QML module is missing | With a Qt from the installer it is there. On Linux distributions install the package for it (for example `qml6-module-qttest` on Ubuntu/Debian) |
| CMake: `Qt6QuickTest` not found | Package for the module is missing | Install the Qt Quick Test development package of your distribution |
| `mouseClick` does nothing | The TestCase is invisible, or the window is not shown yet | `visible: true` and `when: windowShown` |
| `keyClick` writes nothing | The field has no active focus | `field.forceActiveFocus()` before |
| `findChild` returns `null` | The `objectName` is wrong or missing | Check the name; `verify(item)` before you use it |
| A test passes alone and fails with the others | The tests share an object | Make a new object in `init()` with `createTemporaryObject` |
| The test fails on the build server only | There is no screen | `QT_QPA_PLATFORM=offscreen` |
| The test is flaky (fails sometimes) | It uses `wait(100)` and the computer is slow | `tryCompare` / `tryVerify` |
| `module "..." is not installed` for your own module in a test | The test program cannot find the QML files | This lesson imports the folder (`import ".."`). A real module needs the import path set in a setup class (`QUICK_TEST_MAIN_WITH_SETUP`) |
| `compare` shows two values that look equal | One is a number and one is a string, or a `double` with decimals | Compare the same type; `fuzzyCompare` for decimals |

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Qt Quick Test overview | <https://doc.qt.io/qt-6/qtquicktest-index.html> |
| `TestCase` | <https://doc.qt.io/qt-6/qml-qttest-testcase.html> |
| `SignalSpy` | <https://doc.qt.io/qt-6/qml-qttest-signalspy.html> |
| `QUICK_TEST_MAIN` | <https://doc.qt.io/qt-6/qtquicktest-index.html> |
| qmllint | <https://doc.qt.io/qt-6/qtqml-tooling-qmllint.html> |
| qmlformat | <https://doc.qt.io/qt-6/qtqml-tooling-qmlformat.html> |
| QML Profiler | <https://doc.qt.io/qtcreator/creator-qml-performance-monitor.html> |
| CTest | <https://cmake.org/cmake/help/latest/manual/ctest.1.html> |
| Module | Qt Quick Test (`find_package(Qt6 REQUIRED COMPONENTS QuickTest)`, link `Qt6::QuickTest`) |
| Imports | `import QtTest` in the tests, `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Header | `<QtQuickTest>` |
| Minimum Qt version | Qt 6.8 |
