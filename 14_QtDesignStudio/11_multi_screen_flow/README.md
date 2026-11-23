# 11_multi_screen_flow

Build an application with several screens and move between them. Until now every lesson had one screen. Here a Home, a Details and a Settings screen are separate components, and a navigation bar and a button switch between them.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings, and the navigation was clicked with a test script (section 4).

The main part of this lesson uses components, states and connections from lessons 04, 05 and 08, so it works in every Qt project. Design Studio also has a special **Flow View** for this task. It is described in section 5, with a warning.

The same idea written by hand is in `11_QML/16_navigation`.

---

## 1. What You Build

A window with three screens and a navigation bar at the bottom.

| Screen | Component file | Content |
|--------|----------------|---------|
| Home | `HomeScreen.ui.qml` | A title, a text and an **Open details** button |
| Details | `DetailsScreen.ui.qml` | A card with an item name |
| Settings | `SettingsScreen.ui.qml` | A title and a text |

| Navigation | Result |
|------------|--------|
| Tab **Home**, **Details** or **Settings** | Shows that screen. The active tab is green |
| **Open details** on the Home screen | Shows the Details screen |

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The three screens, the navigation bar, the states and the connections |
| `HomeScreen.ui.qml`, `DetailsScreen.ui.qml`, `SettingsScreen.ui.qml` | One file per screen |
| `App.qml` | The window |

---

## 2. Idea

| Part | How it is done |
|------|----------------|
| One screen = one component | You design and test each screen in its own file (lesson 04) |
| The screens sit on top of each other | All three have the same anchors, above the navigation bar. Only one is visible |
| Which screen is shown | A **state** of the root item: `home`, `details`, `settings`. Each state changes the `visible` of the screens (lesson 05) |
| Switching | A **connection** on every tab changes the state (lesson 08) |
| A button inside a screen | The screen exports its Mouse Area with a **property alias**, so the root item can connect to it (lesson 04) |

Why states and not "hide and show by hand": the screen that is shown is described in one place, and the base state is the starting screen.

---

## 3. Steps

### 3.1 Create the project

Create `Multi_screen_flow` with the settings of the [area README](../README.md). Keep the `import` line that Design Studio generated in `App.qml`.

### 3.2 Design one screen at a time

1. Create the file `HomeScreen.ui.qml` with **File > New File > Qt Quick Files > Qt Quick UI File**. The file name must start with a capital letter, or it is not listed under **My Components**.
2. Open it in the 2D view and design the Home screen: a title, a text and a button (a Rectangle with a Text and a **Mouse Area**).
3. Select the Mouse Area in the Navigator and click the **Export** button. This creates `property alias openArea: openArea`.
4. Do the same for `DetailsScreen.ui.qml` and `SettingsScreen.ui.qml`. In the Details screen export the item name text as the alias `itemName`.

Give the screens the size `800 x 400`. Their size is overruled by anchors later.

### 3.3 Put the screens into the main scene

1. Open `Screen01.ui.qml`.
2. Drag the three components from **My Components** into the scene. Give them the ids `homeScreen`, `detailsScreen` and `settingsScreen`.
3. Draw a white Rectangle at the bottom as the navigation bar: `navBar`, height 80. Anchor it left, right and bottom.
4. Anchor each screen left, right and top to the parent, and bottom to the **top of the navigation bar** (lesson 03).
5. Set `visible` off for `detailsScreen` and `settingsScreen` in the base state. The Home screen is the starting screen.
6. Put a **Row** with three tabs into the navigation bar. Each tab is a Rectangle with a Text and a Mouse Area. Use the ids `homeArea`, `detailsArea` and `settingsArea`.

### 3.4 Add the states

In the States view add three states: `home`, `details` and `settings`.

| State | Changes |
|-------|---------|
| `home` | None. It is the base look: Home visible, Home tab green |
| `details` | `homeScreen.visible` off, `detailsScreen.visible` on, `detailsTab` green and its label white, `homeTab` and `homeLabel` back to the normal colors |
| `settings` | The same with `settingsScreen` and `settingsTab` |

Make `home` the default state.

### 3.5 Add the connections

Open the Connections view and add four connections, all with the signal `clicked` and the action **Change State** (root item as the state group):

| Target | State |
|--------|-------|
| `homeArea` | `home` |
| `detailsArea` | `details` |
| `settingsArea` | `settings` |
| `homeScreen.openArea` | `details` |

The last one is the button inside the Home screen. It can be a target because the Home screen exports it as an alias.

### 3.6 Preview

Press **Alt+P**. Click the tabs and the **Open details** button.

### 3.7 Read the code

Open the Text Editor of `Screen01.ui.qml`. The three screens are instances, and the states and connections are at the end. Compare with section 4.

---

## 4. The Code

### 4.1 `Screen01.ui.qml` (the main scene)

```qml
import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"
    state: "home"

    HomeScreen {
        id: homeScreen
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: navBar.top
    }

    DetailsScreen {
        id: detailsScreen
        itemName: "Item 42"
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: navBar.top
    }

    SettingsScreen {
        id: settingsScreen
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: navBar.top
    }

    Rectangle {
        id: navBar
        height: 80
        color: "#ffffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // a Row with three tabs: homeTab, detailsTab and settingsTab.
        // Each is a Rectangle with a Text (homeLabel and so on) and a
        // MouseArea (homeArea, detailsArea, settingsArea).
    }

    Connections {
        target: detailsArea
        function onClicked() { root.state = "details" }
    }

    Connections {
        target: homeScreen.openArea
        function onClicked() { root.state = "details" }
    }

    states: [
        State { name: "home" },
        State {
            name: "details"
            PropertyChanges { target: homeScreen; visible: false }
            PropertyChanges { target: detailsScreen; visible: true }
            PropertyChanges { target: detailsTab; color: "#2e7d5a" }
        }
        // the state "settings" is the same for settingsScreen
    ]
}
```

The listing is shortened. The complete working file is `Screen01.ui.qml`.

| Part | Meaning |
|------|---------|
| `HomeScreen { ... }` | An instance of the component in `HomeScreen.ui.qml` |
| `itemName: "Item 42"` | The alias of the Details screen, set on the instance |
| `anchors.bottom: navBar.top` | The screen ends where the navigation bar begins |
| `visible: false` | The screen is hidden in the base state |
| `state: "home"` | The starting screen |
| `homeScreen.openArea` | The Mouse Area that the Home screen exports with `property alias openArea: openArea` |
| `PropertyChanges { target: detailsScreen; visible: true }` | The state shows the screen |

### 4.2 `HomeScreen.ui.qml` (the exported alias)

```qml
Rectangle {
    id: root
    width: 800
    height: 400
    color: "#eef2f0"

    property alias openArea: openArea

    // title, text and the button with MouseArea { id: openArea }
}
```

The other screens are in the files `DetailsScreen.ui.qml` and `SettingsScreen.ui.qml`.

What the test script saw with Qt 6.12.0:

| Action | State | Visible screen |
|--------|-------|----------------|
| Start | `home` | Home |
| Click **Open details** | `details` | Details |
| Click tab **Settings** | `settings` | Settings |
| Click tab **Home** | `home` | Home |
| Click tab **Details** | `details` | Details |

---

## 5. The Flow View of Design Studio

Design Studio has a special editor for application flows. According to the Qt documentation, a flow view represents a schematic diagram of the screens, and you create it with a wizard.

| Step | What to do |
|------|------------|
| Flow View | **File > New File > Qt Quick Files > Flow View**. Tick **Use Application Import** in the wizard, so that the event simulator and the project `Constants.qml` work |
| Flow Items | Create one **Flow Item** file per screen with the same wizard group, design each screen in it, and drag it into the flow view from **Components > My Components** |
| Action areas | Select a flow item in the 2D view, right-click and choose **Flow > Create Flow Action**. Following the documentation, this creates a clickable action area and a transition to a target screen |
| Transitions | Set the effect, the duration and the easing curve of each flow transition in the Properties view |
| Wildcards | A Flow Wildcard sets the priority of flow items through allow and block lists |

The result is a visual diagram of your screens and the paths between them, which is good for a prototype.

**Warning (not tested here).** The Flow View module (`import FlowView`) comes with Qt Design Studio. A Qt forum thread about Qt 6.5 reports that a flow project exported to CMake and opened in Qt Creator did not find the module, so it did not build outside Design Studio. I did not test this with Design Studio 4.8.2. Before you build a real application on a flow view, export the project to CMake and check that it builds in Qt Creator. The solution of this lesson, states and connections, uses only Qt modules and has no such problem.

---

## 6. Try This

1. Add a fourth screen, `AboutScreen.ui.qml`, a fourth tab and a fourth state.
2. Fade the screens instead of switching: add `opacity` changes to the states and a transition from lesson 06. Remember that an item with opacity 0 still receives mouse clicks, so keep `visible` or `enabled` in sync.
3. Add a **Back** button on the Details screen. Export its Mouse Area and connect it to the state `home`.
4. Set the `itemName` of the Details screen from the Home screen. For example, add a second button on the Home screen that sets it to `Item 7` and opens the Details screen (Set Property plus Change State, lesson 08).
5. Replace the tab buttons by a `TabBar` from lesson 10.

---

## 7. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| Two screens are visible at once | A state does not hide the previous screen. Every state must set `visible` of all screens other than its own |
| A screen is not in My Components | The file name does not start with a capital letter, or the file is not in the project folder |
| `homeScreen.openArea` is not found | The Home screen does not export the alias. Select the Mouse Area in the Home screen and click **Export** |
| The tab is green in two states | The state `home` is the base state, so another state must switch the home tab back to the normal colors |
| The screen is under the navigation bar | The screen is anchored to `parent.bottom` instead of `navBar.top` |
| Clicks go to the wrong screen | The hidden screen is still `visible`. Hidden screens must have `visible: false`, because an invisible item does not receive clicks |
| `Type FlowView unavailable` | The flow view module is missing. It is part of Design Studio, and must be found by the project. See the warning in section 5 |

---

## 8. Git

Commit this README, the four `.ui.qml` files and `App.qml`. See the area README for the files that stay out of Git.

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Designing application flows | <https://doc.qt.io/qtdesignstudio/studio-app-flows.html> |
| Adding flow views | <https://doc.qt.io/qtdesignstudio/studio-flow-view.html> |
| Adding flow items | <https://doc.qt.io/qtdesignstudio/studio-flow-item.html> |
| Adding action areas and transitions | <https://doc.qt.io/qtdesignstudio/studio-flow-action-area.html> |
| Applying states in flows | <https://doc.qt.io/qtdesignstudio/studio-flow-states.html> |
| Working with states | <https://doc.qt.io/qtdesignstudio/quick-states.html> |
| Connecting components to signals | <https://doc.qt.io/qtdesignstudio/quick-signals.html> |
| Qt forum: FlowView and Qt Creator (Qt 6.5) | <https://forum.qt.io/post/758118> |
