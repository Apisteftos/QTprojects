# 05_states

Give one screen several looks and switch between them. A **state** is a named set of property changes. Here a lamp has three states, `off`, `dim` and `on`, and three buttons switch between them.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings, and all three states were set and read back.

The same idea written by hand is in `11_QML/08_states_and_transitions`. Animating the change between states is the next lesson, `06_transitions_and_animations`.

---

## 1. What You Build

A card with a round lamp, a status text and three buttons.

| State | Lamp | Text | Card border |
|-------|------|------|-------------|
| `off` | gray | `Off` | light gray |
| `dim` | yellow, opacity 0.45 | `Dim` | light gray |
| `on` | yellow | `On` | yellow |

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The scene with the three states and the button connections |
| `App.qml` | The window (as in lesson 03) |

---

## 2. What a State Is

| Term | Meaning |
|------|---------|
| **Base state** | The scene as you built it, with all items and their normal values. The States view always starts with it |
| **State** | A name plus a list of **property changes** to the base state. It never adds or removes items, it only changes their properties |
| **Default state** | The state the application starts in |
| **`PropertyChanges`** | The QML type that records one change: a target item and the new values |

Because a state only changes properties, build **all** items in the base state first. To hide an item in a state, change its `visible` or `opacity`. Qt recommends a base state that contains all the components you will need, and states in which you hide and show a set of them.

States and bindings: while a state is active, its values replace the base values. When you switch back, the base values return.

---

## 3. Steps

### 3.1 Create the project

Create `States_demo` with the settings of the [area README](../README.md). Use `App.qml` from lesson 03 (with the project name in the import).

### 3.2 Build the base scene

In `Screen01.ui.qml` build the card, the lamp, the text and three buttons, each button a Rectangle with a Text and a **Mouse Area** inside. Give the items clear ids: `lamp`, `statusText`, `panel`, `offArea`, `dimArea`, `onArea`. Anchor them as in lesson 03.

The base scene should show the **off** look: a gray lamp and the text `Off`.

### 3.3 Add the states

1. Open **View > Views > States**.
2. Click the **Plus** button three times and rename the states to `off`, `dim` and `on`.
3. Select the state `dim`. The 2D view now records changes for this state. In Properties, set the lamp `color` to `#ffd54a` and `opacity` to `0.45`. Select `statusText` and change its text to `Dim`.
4. Select the state `on`. Set the lamp `color` to `#ffd54a`, `statusText` to `On`, and the `border.color` of the panel to `#ffd54a`.
5. Select `off` again. It stays empty. It is the base look.

Properties that you change in a state are highlighted in blue in the Properties panel. A blue value belongs to the state, not to the base.

6. Set `off` as the **default** state with the **Default** option, so that the application starts in it.

Click each state in the States view and watch the 2D view switch. This is the whole point of states: you design every look in one place.

### 3.4 Switch the state with a click

States do not change by themselves. Here you use the Connections view, which is covered fully in `08_connections_and_actions`.

1. Open **View > Views > Connections**.
2. Select `onArea` in the Navigator.
3. In the **Connections** tab, click the **Plus** button.
4. Set **Signal** to `clicked`, **Action** to `Change State`, **State Group** to the root item and **State** to `on`.
5. Repeat for `dimArea` (state `dim`) and `offArea` (state `off`).

You can also right-click the item in the Navigator and use **Connections > Add signal handler > clicked > Change State**.

Press **Alt+P** and click the buttons.

### 3.5 Read the code

Open the Text Editor and find the `states` list at the end of the file and the three `Connections` blocks. Compare with section 4.

---

## 4. The Code

```qml
import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"
    state: "off"

    Rectangle {
        id: panel
        width: 360
        height: 340
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Rectangle {
            id: lamp
            width: 120
            height: 120
            color: "#b8c2bc"
            radius: 60
            anchors.top: parent.top
            anchors.topMargin: 36
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: statusText
            text: "Off"
            font.pixelSize: 28
            color: "#1d4d38"
            anchors.top: lamp.bottom
            anchors.topMargin: 24
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: buttonRow
            spacing: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: offButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Off"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: offArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: dimButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Dim"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: dimArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: onButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "On"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: onArea
                    anchors.fill: parent
                }
            }
        }
    }

    Connections {
        target: offArea
        function onClicked() { root.state = "off" }
    }

    Connections {
        target: dimArea
        function onClicked() { root.state = "dim" }
    }

    Connections {
        target: onArea
        function onClicked() { root.state = "on" }
    }

    states: [
        State {
            name: "off"
        },
        State {
            name: "dim"

            PropertyChanges {
                target: lamp
                color: "#ffd54a"
                opacity: 0.45
            }

            PropertyChanges {
                target: statusText
                text: "Dim"
            }
        },
        State {
            name: "on"

            PropertyChanges {
                target: lamp
                color: "#ffd54a"
            }

            PropertyChanges {
                target: statusText
                text: "On"
            }

            PropertyChanges {
                target: panel
                border.color: "#ffd54a"
            }
        }
    ]
}
```

| Part | Meaning |
|------|---------|
| `state: "off"` | The state at startup. An empty string `""` means the base state |
| `states: [ ... ]` | The list of states of the root item |
| `State { name: "dim" }` | One state. The name is what you use to switch |
| `PropertyChanges { target: lamp; ... }` | The properties of the item `lamp` that differ in this state |
| `State { name: "off" }` | A state without changes: it shows the base look |
| `Connections { target: onArea; function onClicked() { ... } }` | Runs code when `onArea` emits `clicked`. The code sets `root.state` |

Notice what is **not** in the code: no `if` and no list of "what to reset". Switching to `off` restores every value, because the base values come back when no state changes them.

---

## 5. Try This

1. Add a fourth state `night` that sets the root `color` to `#1d2a24` and the text color to white.
2. Add a `when` condition to a state. Select the state, click **Edit** in the **When Condition** field and choose a property, for example `onArea.pressed`. The state is then applied automatically while the button is held. If several `when` conditions are true at the same time, the first matching state is applied.
3. Change the `state` line at the top to `"on"` and run again. The application now starts in `on`.
4. Add an extra change to `dim`: set the `scale` of `lamp` to `0.9`.
5. Open the Text Editor and rename a state there. The Connections no longer find it. Fix them.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| The change appears in all states | You edited the base state. Select the right state in the States view first, then change the property |
| The state changes nothing | `PropertyChanges` has the wrong target, or the property is not changed. Properties changed in a state are blue in the Properties panel |
| A button does not switch the state | The Connection points to another item, or the state name has a typo. Names are case-sensitive |
| `Implicitly defined onFoo properties in Connections are deprecated` | The old form `onClicked: ...` in `Connections`. Use `function onClicked() { ... }` as in this lesson |
| An item appears in the wrong place after switching | A state cannot add items. Build every item in the base state and use `visible` or `opacity` to hide it |
| Setting a state from inside another state is not allowed | Switch states from outside, for example from a signal handler |

---

## 7. Git

Commit this README, `Screen01.ui.qml` and `App.qml`. See the area README for the files that stay out of Git.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Working with states | <https://doc.qt.io/qtdesignstudio/quick-states.html> |
| The States view | <https://doc.qt.io/qtdesignstudio/qtquick-states-view.html> |
| Base state and states as views | <https://doc.qt.io/qtcreator/qtquick-states-base-state.html> |
| Log In UI: States (tutorial) | <https://doc.qt.io/qtdesignstudio/qtdesignstudio-loginui3-example.html> |
| Connecting components to signals | <https://doc.qt.io/qtdesignstudio/quick-signals.html> |
| `State` | <https://doc.qt.io/qt-6/qml-qtquick-state.html> |
| `PropertyChanges` | <https://doc.qt.io/qt-6/qml-qtquick-propertychanges.html> |
| Qt Quick States | <https://doc.qt.io/qt-6/qtquick-statesanimations-states.html> |
