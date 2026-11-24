# 05_signals_and_handlers

Properties hold the state of an interface. **Signals** tell that something happened: a click, a key, a value that changed, a timer that fired. A **handler** is the code that runs when a signal is emitted. This lesson builds a counter with buttons and shows the five common ways to react to events in QML.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with two small changes for that old version, which are not part of this folder). An automated check clicked the buttons, sent key presses, tapped, double-clicked and ran the real timer for about two seconds (17 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A window with a counter from 0 to 5:

- The buttons **+1**, **-1** and **Reset** change the counter.
- A status line says when the limit 5 is reached and the counter cannot go higher.
- A green box counts taps and double taps.
- An orange box reacts to the **Up**, **Down** and **Space** keys.
- A button starts and stops a timer that adds 1 every second.

---

## 2. Signals and Handlers

A **signal** is a message that an object sends: "I was clicked", "my value changed". A **handler** is the code that you attach to it. In QML a handler is written as a property whose name is `on` and the name of the signal with a capital first letter:

| Signal | Handler |
|--------|---------|
| `clicked` | `onClicked` |
| `pressed` | `onPressed` |
| `triggered` | `onTriggered` |
| `valueChanged` | `onValueChanged` |

```qml
MouseArea {
    onClicked: counter.increment()                     // one line
    onPressed: {                                       // or a block with several lines
        console.log("pressed")
        counter.reset()
    }
}
```

**Every property has a signal** named `<property>Changed`. So you can react to any change: `onTextChanged`, `onWidthChanged`. Signal handlers are code (JavaScript), while the lines `name: value` are bindings (lesson 04).

If a signal has arguments, the handler gets them by name: `onPositionChanged: mouse => console.log(mouse.x)`.

---

## 3. Custom Signals

```qml
component Btn: Rectangle {
    signal clicked()                 // declares a signal, here without arguments
    MouseArea { onClicked: btn.clicked() }
}
```

`signal name(type argument, ...)` declares a signal. **Calling it like a function emits it.** The `Btn` forwards the click of its inner `MouseArea` as its own signal. The place that uses a `Btn` writes `onClicked: ...` and does not need to know what is inside. This is how reusable components give out events (lesson 07).

The counter in this lesson is a `QtObject` (an object with no graphics) with the signal `limitReached(int limit)`. The function `increment()` emits it when the counter is at the limit. The signal carries the limit as an argument.

Use signals for **events** ("a click happened"). Use properties for **state** ("the count is 3").

---

## 4. `Connections`

A handler like `onClicked` can be written only in the object that has the signal. If you want to react to a signal of **another** object, use `Connections`:

```qml
Connections {
    target: counter
    function onLimitReached(limit) {
        status.text = qsTr("Status: the limit %1 is reached").arg(limit)
    }
    function onValueChanged() { ... }
}
```

- `target` is the object that sends the signals.
- The handlers are **functions** named `on` + signal (the Qt 6 syntax). The old form `onLimitReached: { ... }` inside `Connections` is deprecated in Qt 6.
- A signal handler of a normal object stays where the object is declared. `Connections` is for sender and receiver that are in different places: for example, `target: someBackend`, an object that C++ gives to QML (chapters 12 and 13).
- Set `target: null` to disconnect. Set `enabled: false` to switch the connections off.

In C++ terms, `Connections` is a `connect()` call that is described in QML.

---

## 5. `TapHandler`

```qml
Rectangle {
    TapHandler {
        onTapped: tapBox.taps += 1
        onDoubleTapped: tapBox.doubleTaps += 1
    }
}
```

`TapHandler` is a **pointer handler**. It reacts to taps from a mouse, a finger or a pen, and it works without an extra item like `MouseArea`: it is put inside the item that should react. Other handlers are `DragHandler` (move an item), `PinchHandler` (zoom and rotate) and `HoverHandler`. The test observed that a double click is counted as **two taps and one double tap**: `onTapped` also runs for the two taps that make a double tap.

`MouseArea` and the handlers can both be used. `MouseArea` is older and simple. The handlers are the modern way, they work better with touch, and several handlers can share one item.

---

## 6. `Keys` and Focus

```qml
Rectangle {
    focus: true
    Keys.onUpPressed: counter.increment()
    Keys.onSpacePressed: counter.reset()
}
```

Key events go to **one** item: the item that has the **active focus**. Rules:

- `focus: true` means "I want the focus". Only one item per scope can win it.
- `activeFocus` is a read-only property: this item really has the focus now. The orange box uses it in a binding for its color.
- `forceActiveFocus()` takes the focus by code. A click does **not** move the focus by itself: a `MouseArea` does not take it. The box calls the function so that a click gives it the focus back after another item (for example a text field) has taken it.
- A `Window` with several items needs one that has the focus, or the keys go nowhere.
- `Keys.onPressed: event => { if (event.key === Qt.Key_Escape) ... }` is the form for any key. Set `event.accepted = true` when your code handled the key, so it is not sent on. The names such as `onUpPressed` are short forms for common keys.
- A shortcut that works everywhere in the window is a `Shortcut` object (it needs `import QtQuick`).

---

## 7. `Timer`

```qml
Timer {
    interval: 1000
    repeat: true
    onTriggered: counter.increment()
}
```

`running` starts and stops it, `interval` is in milliseconds, `repeat: false` (the default) fires once. A `Timer` is a normal object with the signal `triggered`. It needs no code to start: `running: true` or a binding such as `running: checkBox.checked` does it. The button of the lesson uses `running = !running`.

---

## 8. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app05_signals_and_handlers
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Click **+1** six times. Read the status line, then click **-1**.
2. Click the **Reset** button and press the **Up** key. Does it still work? Why? Then add a `TextInput` to the window, click into it, and press **Up** again.
3. Tap the green box and double-tap it. Read the numbers.
4. Change `interval` of the timer to `250`.
5. Add a second `signal` to `Btn`: `longPressed()`, and emit it from `onPressAndHold` of the `MouseArea`.
6. In `Connections`, add `function onValueChanged() { console.log(counter.value) }` and look at the output.

---

## 9. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The handler never runs | The name is wrong (`onclicked`, `onClick`) | `on` + signal name with a capital letter |
| `Keys.onPressed` does nothing | The item has no focus | `focus: true` and `forceActiveFocus()` |
| `ReferenceError: x is not defined` in `Connections` | The `id` is in another file | Pass the object through a property |
| `Connections: Implicitly defined onFoo properties in Connections are deprecated` | The old Qt 5 style | Use `function onFoo() { }` |
| The signal handler gets no argument | The name of the argument is not in the handler | `onFoo: argument => ...` or `function onFoo(argument)` |
| A `MouseArea` blocks another one | The top item takes the click | Give each its own area; use `mouse.accepted = false` to pass the click on |
| The timer does not stop | `repeat: true` and `running` was never set to `false` | `running = false` |
| A property change handler runs at the start | `onValueChanged` also runs when the initial value is set | Check the state inside, or use `Component.onCompleted` |

---

## 10. Reference

| Topic | Where to look |
|-------|---------------|
| Signal and handler attributes | <https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html#signal-attributes> |
| `Connections` | <https://doc.qt.io/qt-6/qml-qtqml-connections.html> |
| `MouseArea` | <https://doc.qt.io/qt-6/qml-qtquick-mousearea.html> |
| `TapHandler` and input handlers | <https://doc.qt.io/qt-6/qml-qtquick-taphandler.html> |
| Keyboard focus in Qt Quick | <https://doc.qt.io/qt-6/qtquick-input-focus.html> |
| `Keys` | <https://doc.qt.io/qt-6/qml-qtquick-keys.html> |
| `Timer` | <https://doc.qt.io/qt-6/qml-qtqml-timer.html> |
| `QtObject` | <https://doc.qt.io/qt-6/qml-qtqml-qtobject.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Imports | `import QtQuick` |
| Minimum Qt version | Qt 6.8 |
