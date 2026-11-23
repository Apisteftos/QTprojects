# 03_layout_and_anchors

Make a Design Studio scene follow the window size. In `02_items_and_assets` every item had a fixed `x` and `y`. Here the items are tied to the window edges and to each other with anchors, so the layout stretches when the window grows.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 and resized from 800 x 480 to 1100 x 700.

The same topic written by hand is in `11_QML/03_anchors_and_layouts`.

---

## 1. What You Build

A window with a header bar, a sidebar and a content area with a centered card. When you resize the window, the header stays on top and as wide as the window, the sidebar keeps its width and height, and the card stays in the middle of the content area.

| File | Purpose |
|------|---------|
| `App.qml` | The window. It makes `Screen01` fill the window |
| `Screen01.ui.qml` | The scene with header, sidebar and card |
| `assets/leaf.png` | The image from lesson 02 |

---

## 2. Two Places Where the Size Is Decided

A scene that does not follow the window has one of two causes, and usually both must be fixed:

| Place | Wrong | Right |
|-------|-------|-------|
| `App.qml` | `anchors.centerIn: parent` (the scene keeps its own size and sits in the middle) | `anchors.fill: parent` |
| `Screen01.ui.qml` | Items with fixed `x` and `y` | Anchors to the parent and to other items |

Also remove `width: mainScreen.width` and `height: mainScreen.height` from the `Window` in `App.qml`. If the window takes its size from the screen and the screen takes its size from the window, you get a circular dependency. Give the window fixed starting values instead.

---

## 3. Anchors

Every item has anchor lines: `left`, `right`, `top`, `bottom`, `horizontalCenter`, `verticalCenter`. You tie a line of one item to a line of its parent or of a sibling.

| Anchor | Meaning |
|--------|---------|
| `anchors.fill: parent` | All four edges to the parent |
| `anchors.centerIn: parent` | Centered in the parent |
| `anchors.left: parent.left` | Left edge to the left edge of the parent |
| `anchors.top: header.bottom` | Top edge to the bottom edge of the sibling `header` |
| `anchors.margins: 16` | Distance to all anchored edges. There are also `leftMargin`, `topMargin` and so on |

Rules:
- An item can be anchored to its **parent** or to a **sibling** (an item with the same parent). Not to a grandchild or an item in another branch.
- If you anchor `left` and `right`, the width is calculated. Do not also set `width`.
- An anchored edge cannot be set with `x`, `y` at the same time. The anchor wins.

---

## 4. Steps

### 4.1 Create the project

Create `Layout_and_anchors` with the settings of the [area README](../README.md). Import `assets/leaf.png` with the Plus button in Assets.

### 4.2 Fix `App.qml`

Open `App.qml` in the Text Editor and replace it with the version in section 5.1. The important change is `anchors.fill: parent`.

### 4.3 Build the scene

1. In `Screen01.ui.qml`, set the root color to `#eef2f0`.
2. **Header.** Drag a Rectangle. Set `height` 64, color `#1d4d38`. Open the **Layout** section of the Properties panel and click the anchor buttons for top, left and right, each with margin 0.
3. **Sidebar.** Drag a Rectangle, `width` 160, color `#dfe8e3`. Anchor left to the parent, bottom to the parent, and top to the **bottom of the header**. In the Layout section, choose the header as the target of the top anchor.
4. **Content.** Drag an Item (it has no color, it is only an area). Anchor left to the right of the sidebar, right to the parent, top to the bottom of the header, bottom to the parent.
5. **Card.** Drag a Rectangle into the content item, `width` 320, `height` 260, white, `radius` 16. Use the **center in parent** button.
6. **Card content.** Add the image and two texts inside the card. For each, anchor the top (to the card, or to the item above it) with a margin, and use **horizontal center** to the card.
7. **Header content and sidebar.** Add the texts. In the header, select the two texts and use the context menu entry for a **Row Layout**, or write it as in section 5.2. In the sidebar, select the three texts and use the **Column** (stack in column) entry.
8. Press **Alt+P** and drag the corner of the preview window. Everything should follow.

### 4.4 Read the code

Open the Text Editor. Each button you clicked in the Layout section is an `anchors.` line. Compare it with section 5.2.

---

## 5. The Code

### 5.1 `App.qml`

```qml
import QtQuick
import Layout_and_anchors

Window {
    width: 800
    height: 480
    minimumWidth: 560
    minimumHeight: 360

    visible: true
    title: "Layout_and_anchors"

    Screen01 {
        id: mainScreen
        anchors.fill: parent
    }
}
```

`import Layout_and_anchors` is the module of your project. If you used another project name, Design Studio has already written the correct import.

### 5.2 `Screen01.ui.qml`

```qml
import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    // Header: stuck to the top, left and right edges
    Rectangle {
        id: header
        height: 64
        color: "#1d4d38"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            spacing: 12

            Text {
                id: headerTitle
                text: "Layout and anchors"
                font.pixelSize: 22
                color: "#ffffff"
            }

            Item {
                id: spacer
                Layout.fillWidth: true
            }

            Text {
                id: headerInfo
                text: "v1"
                font.pixelSize: 16
                color: "#c9d6cf"
            }
        }
    }

    // Sidebar: fixed width, between the header and the bottom edge
    Rectangle {
        id: sidebar
        width: 160
        color: "#dfe8e3"
        anchors.left: parent.left
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 16
            spacing: 12

            Text {
                text: "Home"
                font.pixelSize: 18
                color: "#1d4d38"
            }

            Text {
                text: "Items"
                font.pixelSize: 18
                color: "#1d4d38"
            }

            Text {
                text: "Settings"
                font.pixelSize: 18
                color: "#1d4d38"
            }
        }
    }

    // Content area: takes all the space that is left
    Item {
        id: content
        anchors.left: sidebar.right
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        Rectangle {
            id: card
            width: 320
            height: 260
            color: "#ffffff"
            radius: 16
            border.color: "#c9d6cf"
            border.width: 2
            anchors.centerIn: parent

            Image {
                id: leaf
                width: 96
                height: 96
                source: "assets/leaf.png"
                fillMode: Image.PreserveAspectFit
                anchors.top: parent.top
                anchors.topMargin: 28
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: title
                text: "Follows the window"
                font.pixelSize: 24
                color: "#1d4d38"
                anchors.top: leaf.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: subtitle
                text: "Nothing has a fixed x or y"
                font.pixelSize: 16
                color: "#5b6b63"
                anchors.top: title.bottom
                anchors.topMargin: 8
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
```

| Part | Meaning |
|------|---------|
| Header | Anchored left, right and top. Only the `height` is set, the width follows the window |
| Sidebar | Only the `width` is set. Top is anchored to `header.bottom`, bottom to the parent, so it fills the height under the header |
| Content | Anchored to `sidebar.right`, the parent edges and `header.bottom`. It has no size of its own |
| Card | `anchors.centerIn: parent` keeps it in the middle of the content area |
| `leaf`, `title`, `subtitle` | A chain: each one is anchored to the bottom of the item above, and centered horizontally in the card |
| `Column` | A positioner that stacks its children with `spacing`, no anchors needed between them |
| `RowLayout` and `Layout.fillWidth` | The empty `spacer` takes all free width, so the version text moves to the right edge |

Numbers measured with Qt 6.12.0:

| Window | Card position in the content area | Content area |
|--------|-----------------------------------|--------------|
| 800 x 480 | `x` 160, `y` 78 | 640 x 416 |
| 1100 x 700 | `x` 310, `y` 188 | 940 x 636 |

The card keeps its 320 x 260 size and stays centered.

---

## 6. Try This

1. Make the window bigger and smaller in the Live Preview. Does anything overlap at the minimum size of 560 x 360?
2. Change the sidebar `width` to 220. The content area shrinks and the card moves, with no other change.
3. Remove `anchors.horizontalCenter` from `title` and add `anchors.left: parent.left` with `anchors.leftMargin: 24`.
4. Set `anchors.fill: parent` and `x: 10` on the same item, and see which one wins.
5. Replace the `Column` in the sidebar with a `Row` and watch the texts line up horizontally.

---

## 7. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| The scene still stays small in the window | `App.qml` still has `anchors.centerIn: parent`. Use `anchors.fill: parent` |
| `Binding loop detected for property "width"` | The window takes its size from the screen, and the screen from the window. Use fixed numbers for the window |
| `Cannot anchor to an item that isn't a parent or sibling` | The target is in another branch. Move the item or anchor to a common parent |
| A width or `x` is ignored | Both sides are anchored, or the anchor overrides `x`. Remove the extra anchor or the fixed value |
| The Layout buttons are grayed out | The item has no parent to anchor to, or it is inside a `Column`, `Row` or layout, which place their children themselves |
| The sidebar goes under the header | Its top anchor is `parent.top` instead of `header.bottom` |

---

## 8. Git

Commit this README, `App.qml`, `Screen01.ui.qml` and `assets/leaf.png`. See the area README for the rules about the project files that stay out of Git.

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Anchors in Qt Quick | <https://doc.qt.io/qt-6/qtquick-positioning-anchors.html> |
| Positioning (`Row`, `Column`, `Grid`) | <https://doc.qt.io/qt-6/qtquick-positioning-topic.html> |
| Qt Quick Layouts | <https://doc.qt.io/qt-6/qtquicklayouts-index.html> |
| `Item` anchors property | <https://doc.qt.io/qt-6/qml-qtquick-item.html#anchors-group> |
| Setting anchors and margins in Qt Design Studio | <https://doc.qt.io/qtdesignstudio/qtquick-positioning.html> |
| Design Studio tutorial: Log In UI, Positioning | <https://doc.qt.io/qtdesignstudio/qtdesignstudio-loginui2-example.html> |
