# 16_project_dashboard_ui

The final project of the Design Studio area: a small dashboard that uses most of the earlier lessons together. You build it from the parts you already know, so this README only lists the steps and the new ideas.

**Tested with Qt Design Studio 4.8.2:** not run in Design Studio itself. The final QML was run with Qt 6.12.0 without warnings. A test script clicked the chart, the navigation and the **Water** and **Dry out** buttons and read back the values (section 5). Two screenshots of the result were checked.

---

## 1. What You Build

A plant care dashboard with a sidebar and two pages.

| Part | What it does |
|------|--------------|
| Sidebar | Title and two navigation items. The active item is highlighted |
| Overview page | Three stat cards (number of plants, selected plant, its water level) and a bar chart with one bar per plant. Click a bar to select a plant |
| Plants page | A list with a colored dot, a small progress bar and the water level. **Water** sets the selected plant to 100 %, **Dry out** lowers it by 20 % |

| File | Purpose |
|------|---------|
| `StatCard.ui.qml` | A reusable card with the aliases `label`, `value` and `accent` (lesson 04) |
| `Screen01.ui.qml` | The sidebar, both pages, the model, the states and the actions |
| `App.qml` | The window |

---

## 2. Which Lesson Is Used Where

| Part of the dashboard | Lesson |
|-----------------------|--------|
| Sidebar and pages placed with anchors | 03 |
| `StatCard` as a component with property aliases | 04 |
| Two states `overview` and `plants`, switched by the navigation | 05 |
| Fade between the pages | 06 |
| Navigation and buttons with Connections | 08 |
| `selectedIndex` as a custom property that several items bind to | 09 |
| `ListModel`, `ListView` and `Repeater` with delegates | 12 |

---

## 3. Steps

### 3.1 Create the project

Create `Project_dashboard_ui` with the settings of the [area README](../README.md). Keep the `import` line that Design Studio generated in `App.qml`. Set the size of the screen to 960 x 600.

### 3.2 Add the component

Create `StatCard.ui.qml` as in lesson 04: a rounded rectangle with a colored bar on the left and two texts. Export the two texts and the bar color as the aliases `label`, `value` and `accent`.

### 3.3 Add the model and the shared value

Add a `ListModel` with five plants. Every entry has the roles `name`, `water` (0 to 100) and `tint`. Add a custom property `selectedIndex` (int, default 0) to the root item.

### 3.4 Build the sidebar

A dark rectangle on the left, a title text and two navigation rectangles, each with a text and a `MouseArea`.

### 3.5 Build the two pages

Both pages are an `Item` that fills the area next to the sidebar. The second page starts with `opacity: 0`. Both use `visible: opacity > 0`, so the hidden page does not catch clicks.

**Overview page:** a `Row` with three `StatCard` items and a card with a `Repeater`. The delegate is one bar:

```qml
Rectangle {
    height: 200 * model.water / 100
    color: model.tint
    border.width: index === root.selectedIndex ? 3 : 0
}
```

**Plants page:** a `ListView` with `currentIndex: root.selectedIndex`, and the two buttons below it.

### 3.6 Bind the stat cards

The cards read the selected plant from the **current item** of the list:

```qml
value: plantList.currentItem ? plantList.currentItem.waterValue + " %" : "-"
```

For this the list delegate has two properties that copy the model roles:

```qml
property string plantName: model.name
property int waterValue: model.water
```

Why not `plantModel.get(index).water`? A binding that reads a value through `get()` is not updated when the value changes later. A property of the delegate is updated, so the card follows the **Water** and **Dry out** buttons.

### 3.7 Add the states

Two states change the `opacity` of the pages and the colors of the navigation items. One `Transition` from `"*"` to `"*"` with a `NumberAnimation` on `opacity` and a `ColorAnimation` makes the change smooth.

### 3.8 Add the actions

Four `Connections` items with the `function onClicked()` form:

| Area | Action |
|------|--------|
| Overview navigation | `root.state = "overview"` |
| Plants navigation | `root.state = "plants"` |
| Water | `plantModel.setProperty(root.selectedIndex, "water", 100)` |
| Dry out | `plantModel.setProperty(root.selectedIndex, "water", Math.max(0, plantModel.get(root.selectedIndex).water - 20))` |

---

## 4. Ideas to Continue

- Add a third page with settings (lesson 10 controls) and a third state.
- Move the data into a backend, as in lesson 15.
- Add a `Timeline` that grows the bars when the page opens (lesson 07).

---

## 5. How This Was Tested

With Qt 6.12.0, offscreen, with a script that clicked the items:

| Step | Result |
|------|--------|
| Start | State `overview`, cards: 5 plants, `Fern`, `80 %` |
| Click the second bar | Cards: `Cactus`, `25 %` |
| Click **Plants** | State `plants`, list page visible |
| Click **Water** | Card: `100 %` |
| Click **Dry out** | Card: `80 %` |
| Click **Overview** | State `overview` |

---

## Common Problems

- **The stat card does not change after Water or Dry out:** the binding reads the value with `get()`. Use a property of the delegate (section 3.6).
- **`QML module not found`:** the `import` line in `App.qml` must match the project name. Keep the line that Design Studio generated.
- **Design Studio shows M209 "This id might be ambiguous":** use a more specific id such as `plantList`, not `list`.
- **Clicks on the visible page do nothing:** the hidden page is still on top. Set `visible: opacity > 0` on both pages.
- **The list shows no current item:** `currentItem` is empty while the list is empty. The binding in 3.6 handles this with the `? :` check.

## Reference

- **Modules:** `QtQuick` only
- **Qt version:** Qt 6.8 or newer
- **Design Studio views:** 2D, Navigator, Properties, States, Transitions, Connections, Components
- **Documentation:** the Qt Design Studio manual; the QML types `ListModel`, `ListView`, `Repeater`, `State`, `Transition`, `Connections`
