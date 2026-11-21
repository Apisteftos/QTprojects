# 02_items_and_assets

Place items on the canvas, import an image as an asset and change properties in the Properties panel. The same three items (`Rectangle`, `Text`, `Image`) are written by hand in `11_QML/02_items_and_properties`. Here you build them with the mouse and read the QML that Design Studio writes.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was also loaded with Qt 6.12.0.

---

## 1. What You Build

A card on a light background: a leaf icon, a title and a subtitle. The scene has a fixed size of 800 x 480. Making it follow the window size is the topic of `03_layout_and_anchors`.

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The finished scene, to compare with your own result |
| `assets/leaf.png` | The image that you import as an asset (128 x 128, drawn for this tutorial) |

---

## 2. The Views You Use

| View | What it is for |
|------|----------------|
| **Components** | The list of items you can place (Rectangle, Text, Image and so on) |
| **Assets** | The images and other files of the project |
| **Navigator** | The tree of items in the scene, with their `id` and order |
| **2D** | The canvas where you place and move items |
| **Properties** | The values of the selected item (`x`, `y`, `width`, `color`, `source` and so on) |
| **Text Editor** | The generated QML (switch to it to read the code) |

If a view is missing, open it from the **View** menu.

---

## 3. Steps

### 3.1 Create the project

Use the settings from section "A First Project" of the [area README](../README.md), with the name `Items_and_assets`. Keep the project outside the tutorial repository while you work, and copy files in afterwards.

### 3.2 Import the image

The Assets view shows the images and other files that you add to the project folder, either by dragging files into Design Studio or with the **Plus** button.

1. Open the **Assets** view.
2. Click the **Plus** button and choose `assets/leaf.png`.
3. The image appears in the Assets view.

### 3.3 Build the scene

1. Select `Screen01` in the Navigator. In Properties, set **Color** to `#eef2f0`.
2. Drag a **Rectangle** from Components onto the canvas. This is the card. Set `x` 220, `y` 90, `width` 360, `height` 300, `color` `#ffffff` and `radius` 16. Set the border color to `#c9d6cf` and the border width to 2.
3. Drag `leaf.png` from Assets onto the **card** in the Navigator, so that it becomes a child of the card. Design Studio creates an `Image` item and sets its `source` to the file.
4. Set the image `x` 116, `y` 24, `width` 128, `height` 128.
5. Drag a **Text** item onto the card. Set `text` to `Items and assets`, the font size to 28, `width` 360 and the horizontal alignment to center.
6. Add a second **Text** below it with the text `Built in Qt Design Studio` and the font size 16.
7. Press **Alt+P** to start the Live Preview.

### 3.4 Read the code

Switch to the **Text Editor** view and open `Screen01.ui.qml`. Every value you typed in the panel is now a line of QML. Change a number in the text and watch the canvas move.

---

## 4. The Code

This is the scene as Design Studio writes it. Your file will look similar, and the order of properties can differ.

```qml
import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Rectangle {
        id: card
        x: 220
        y: 90
        width: 360
        height: 300
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2

        Image {
            id: leaf
            x: 116
            y: 24
            width: 128
            height: 128
            source: "assets/leaf.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: title
            x: 0
            y: 170
            width: 360
            text: "Items and assets"
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            color: "#1d4d38"
        }

        Text {
            id: subtitle
            x: 0
            y: 216
            width: 360
            text: "Built in Qt Design Studio"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            color: "#5b6b63"
        }
    }
}
```

| Part | Meaning |
|------|---------|
| `Screen01.ui.qml` | A `.ui.qml` file holds the visual part of the design. Design Studio edits it in the 2D view. If you add code there that the 2D view cannot display, Design Studio shows error messages |
| `Rectangle { ... }` as root | The scene itself is an item with a size and a color |
| Items inside `card` | Children use coordinates **relative to their parent**. The image at `x: 116` is 116 pixels from the left edge of the card, not of the window |
| `source: "assets/leaf.png"` | A path relative to the QML file. Design Studio fills it in when you drop the image |
| `id` | The name of the item, shown in the Navigator. Useful later for bindings and anchors |

Compare this with `Main.qml` in `11_QML/02_items_and_properties`: the items and the `property: value` form are the same. Only the way you write them differs.

---

## 5. Try This

1. Drag the card with the mouse in the 2D view and watch `x` and `y` change in the Properties panel.
2. Move the image in the Navigator above the card (drag it out of the card). Its position now means something else, because the parent has changed.
3. Change the image `fillMode` in Properties and set `width` to 200. See what each mode does to the picture.
4. Select the card and change `opacity` to `0.5`. The children become transparent as well.
5. Add a third `Text` and change its font family.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| The image is empty in the Live Preview | The `source` path is wrong or the file is not in the project folder. Re-import it with the Plus button in Assets |
| The image is stretched | Set `fillMode` to `PreserveAspectFit` |
| The text is cut off | A `Text` with a `width` and a long text needs `wrapMode: Text.WordWrap`, or a bigger width |
| An item is not visible | It may be behind another item. Items later in the Navigator are drawn on top, or change `z` |
| The scene does not follow the window | Expected here. The scene has a fixed size. See `03_layout_and_anchors` |

---

## 7. Git

Commit this README, `Screen01.ui.qml` and `assets/leaf.png`. Do not commit the whole generated project or the `.qmlproject.qtds` file (see the [area README](../README.md) and its `.gitignore`).

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| The Assets view | <https://doc.qt.io/qtdesignstudio/quick-assets.html> |
| Importing 2D assets | <https://doc.qt.io/qtdesignstudio/studio-importing-2d.html> |
| Images in Qt Design Studio | <https://doc.qt.io/qtdesignstudio/quick-images.html> |
| `Rectangle` | <https://doc.qt.io/qt-6/qml-qtquick-rectangle.html> |
| `Text` | <https://doc.qt.io/qt-6/qml-qtquick-text.html> |
| `Image` | <https://doc.qt.io/qt-6/qml-qtquick-image.html> |
