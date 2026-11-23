# 08_connections_and_actions

Make buttons do something without writing code. In lesson 05 you already used one connection to change a state. Here you use the Connections view with all its main actions: Change State with a condition, Call Function, Set Property and Print Message.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings. The three buttons were clicked with a test script and each action was checked (section 4).

The same idea written by hand is in `11_QML/05_signals_and_handlers`.

---

## 1. What You Build

A card with a lamp, a status text, a message text and three buttons.

| Button | Signal | Action | Result |
|--------|--------|--------|--------|
| **Toggle** | `clicked` | Change State, with a condition and an else branch | Switches the lamp between `off` and `on` |
| **Pop** | `clicked` | Call Function `popAnimation.restart()` | Plays a short timeline animation of the lamp (it pops) |
| **Hello** | `clicked` | Set Property `messageText.text` | Changes the message text |
| **Hello** | `clicked` | Print Message | Writes a line to the Output view |

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The scene, the states, the timeline and the connections |
| `App.qml` | The window |

---

## 2. Terms

| Term | Meaning |
|------|---------|
| **Signal** | An event a component sends, for example `clicked` of a Mouse Area |
| **Signal handler** | Code that runs when the signal is emitted. For `clicked` it is named `onClicked` |
| **Connection** | Connects the signal of a **target** component to an **action** |
| **Action** | What happens: Call Function, Assign, Change State, Set Property or Print Message |
| **Condition** | An extra test that decides whether the action runs. You can add an else branch |

The Connections view has several tabs. In this lesson you use the **Connections** tab. The **Bindings** tab connects properties so that one follows another (lesson 09), and the **Properties** tab adds custom properties to a component.

---

## 3. Steps

### 3.1 Create the project and the base scene

Create `Connections_and_actions` with the settings of the [area README](../README.md). Keep the `import` line that Design Studio generated in `App.qml`.

Build the scene of section 4: a card, a lamp (a circle), two Text items and three buttons. Each button is a Rectangle with a Text and a **Mouse Area** inside. Use these ids: `lamp`, `statusText`, `messageText`, `toggleArea`, `popArea`, `helloArea`.

Add two states in the States view: `off` (no changes, default) and `on` (lamp color `#ffd54a`, `statusText` text `On`), as in lesson 05.

### 3.2 Add a small timeline for the Pop button

1. In the Timeline view add a timeline (Plus button). Set the animation ID to `popAnimation` and the duration to `600`. **Clear** "Running in base state", so it does not run at startup.
2. Record two keyframes for the lamp `scale`: `0.6` at frame 0 and `1` at the last frame. Set an `OutBack` easing on the second keyframe.
3. Move the playhead to the last frame, so the lamp looks normal at rest.

Timelines are described in lesson 07.

### 3.3 The Toggle connection (with a condition)

1. Open **View > Views > Connections**.
2. Select `toggleArea` in the Navigator.
3. In the **Connections** tab click the **Plus** button (Add).
4. Select the **Signal** `clicked` and the **Action** `Change State`. Choose the root item as the state group and the state `on`.
5. Click **Add Condition**. Choose the component `root`, the property `state`, the operator `EQUAL` and the value `on`. The action then runs only if the condition is true.
6. Add an **Else Statement** with the action `Change State` to the state `off`.

Now the button switches in both directions. For complex logic there is also **Manual Edit**, where you write JavaScript directly.

You can also right-click an item in the Navigator and use **Connections > Add signal handler**.

### 3.4 The Pop connection (Call Function)

1. Select `popArea`, add a connection with the signal `clicked`.
2. Choose the action **Call Function**. Select the target `popAnimation` and the function `restart`.

### 3.5 The Hello connections (Set Property and Print Message)

1. Select `helloArea` and add a connection: signal `clicked`, action **Set Property**, item `messageText`, property `text`, value `Hello from a signal`.
2. Add a second connection on `helloArea`: signal `clicked`, action **Print Message**, message `Hello button clicked`.

Several connections can use the same signal of the same item. They all run.

### 3.6 Preview

Press **Alt+P** and click the buttons. The Print Message text appears in the **Output** view, not in the window.

### 3.7 Read the code

Open the Text Editor and find the `Connections` blocks. Each connection you created is one of them. Compare with section 4.

---

## 4. The Code

```qml
import QtQuick
import QtQuick.Timeline

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"
    state: "off"

    Rectangle {
        id: panel
        width: 400
        height: 360
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Rectangle {
            id: lamp
            width: 100
            height: 100
            color: "#b8c2bc"
            radius: 50
            x: 150
            y: 28
        }

        Text {
            id: statusText
            text: "Off"
            font.pixelSize: 28
            color: "#1d4d38"
            anchors.top: lamp.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: messageText
            text: "Click a button"
            font.pixelSize: 16
            color: "#5b6b63"
            anchors.top: statusText.bottom
            anchors.topMargin: 12
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: buttonRow
            spacing: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter

            // three buttons: toggleButton, popButton and helloButton,
            // each a Rectangle with a Text and a MouseArea
            // (toggleArea, popArea, helloArea). See Screen01.ui.qml.
        }
    }

    Connections {
        target: toggleArea
        function onClicked() {
            if (root.state === "on")
                root.state = "off"
            else
                root.state = "on"
        }
    }

    Connections {
        target: popArea
        function onClicked() { popAnimation.restart() }
    }

    Connections {
        target: helloArea
        function onClicked() { messageText.text = "Hello from a signal" }
    }

    Connections {
        target: helloArea
        function onClicked() { console.log("Hello button clicked") }
    }

    // Timeline with popAnimation (running: false) and a KeyframeGroup
    // for lamp.scale, see Screen01.ui.qml and lesson 07

    states: [
        State { name: "off" },
        State {
            name: "on"
            PropertyChanges { target: lamp; color: "#ffd54a" }
            PropertyChanges { target: statusText; text: "On" }
        }
    ]
}
```

The listing is shortened where the code repeats. The complete working file is `Screen01.ui.qml`.

| Part | Meaning |
|------|---------|
| `Connections { target: toggleArea ... }` | A connection: listens to signals of `toggleArea` |
| `function onClicked() { ... }` | The signal handler for `clicked`. The action runs inside it |
| `if (root.state === "on") ... else ...` | The condition and the else branch of the Toggle connection |
| `popAnimation.restart()` | The Call Function action. `restart()` is a method of the animation |
| `messageText.text = "..."` | The Set Property action |
| `console.log(...)` | The Print Message action. The text goes to the Output view |
| Two `Connections` with `target: helloArea` | Two actions for the same signal |

What the test script saw with Qt 6.12.0:

| Step | State | Lamp | Lamp scale | Message text |
|------|-------|------|-----------|--------------|
| Start | `off` | gray | 1.0 | `Click a button` |
| Toggle | `on` | yellow | 1.0 | `Click a button` |
| Toggle again | `off` | gray | 1.0 | `Click a button` |
| Pop, 150 ms later | `off` | gray | 0.92 | `Click a button` |
| Pop, 950 ms later | `off` | gray | 1.0 | `Click a button` |
| Hello | `off` | gray | 1.0 | `Hello from a signal` |

The Output also showed `Hello button clicked`.

---

## 5. Try This

1. Add a fourth button **Reset** with a Set Property action that sets `messageText.text` back to `Click a button`.
2. Change the Toggle condition: add a second test with `AND`, for example only toggle if the message text is not `Hello from a signal`.
3. Add a connection on the Pop button with the action **Change State** to `on`, so it also turns the lamp on.
4. Connect the `pressed` signal of `toggleArea` instead of `clicked`. What changes when you click and hold?
5. Open **Manual Edit** for a connection and write one line of JavaScript, for example `console.log(root.state)`.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| The button does nothing | The connection points to another item, or the Mouse Area is covered by another item. Check the target in the Connections tab, and the order in the Navigator |
| `Implicitly defined onFoo properties in Connections are deprecated` | The old form `onClicked: ...` inside `Connections`. Use `function onClicked() { ... }` |
| Print Message shows nothing | The text goes to the Output view or the terminal, not to the window. Open the Output view |
| The animation does not start with Call Function | The timeline is not enabled, or the function is not `restart` or `start`. Check the animation id |
| The state switches only one way | The Else Statement is missing. Add it in the condition |
| Clicking the lamp does nothing | The lamp has no Mouse Area. Only components with a signal can be a target |
| The connection is gone after renaming an id | Connections refer to ids. Open the Connections tab and select the new id |

---

## 7. Git

Commit this README, `Screen01.ui.qml` and `App.qml`. See the area README for the files that stay out of Git.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Connecting components to signals (actions, conditions) | <https://doc.qt.io/qtdesignstudio/quick-signals.html> |
| The Connections view | <https://doc.qt.io/qtdesignstudio/qtquick-connection-view.html> |
| Creating UI logic | <https://doc.qt.io/qtdesignstudio/qtquick-creating-ui-logic.html> |
| Log In UI: States (a Change State connection) | <https://doc.qt.io/qtdesignstudio/qtdesignstudio-loginui3-example.html> |
| `Connections` | <https://doc.qt.io/qt-6/qml-qtqml-connections.html> |
| `MouseArea` | <https://doc.qt.io/qt-6/qml-qtquick-mousearea.html> |
| Signal and handler event system | <https://doc.qt.io/qt-6/qtqml-syntax-signals.html> |
