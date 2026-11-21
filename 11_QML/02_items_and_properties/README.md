# 02_items_and_properties

Items, properties, `id` and `parent`: the three ideas every QML file is built from. You build a small card with text, an image, a progress bar and a few shapes, using only `Rectangle`, `Text` and `Image`.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

---

## 1. What You Build

A window with two areas:

- A **card** (a white `Rectangle` with a border) that contains a heading, a subtitle, an image, a progress bar, a percentage text and four small rectangles that show `opacity`, `rotation`, `scale` and `visible`.
- A **stack** of three overlapping squares that show how the `z` property decides which item is drawn on top.

All interface code is in `Main.qml`. `main.cpp` only starts the engine and loads that file.

---

## 2. Items: the Building Blocks

Everything you see in a Qt Quick window is an **item**. `Item` is the base type: it has a position and a size, but it draws nothing. The visible types build on it:

| Type | What it draws | Properties you use first |
|------|---------------|--------------------------|
| `Rectangle` | A filled box, optionally with a border and rounded corners | `color`, `radius`, `border.color`, `border.width` |
| `Text` | A line or block of text | `text`, `color`, `font.pixelSize`, `font.bold` |
| `Image` | A picture from a file | `source`, `fillMode` |
| `Item` | Nothing (a container) | `x`, `y`, `width`, `height` |

Items form a **tree**. An item written inside another item is its **child**, and the outer item is the child's **parent**:

```qml
Rectangle {            // parent
    Text { text: "Hi" }   // child
}
```

A child is positioned **relative to its parent**: `x: 16` means 16 pixels from the left edge of the parent, not of the window. Move the parent and all its children move with it.

---

## 3. Properties

An item is described by its **properties**. You set one with `name: value`:

```qml
Rectangle {
    width: 200
    height: 80
    color: "#2e86de"
    radius: 8
}
```

`width`, `height`, `color` and `radius` are **built-in** properties. You can also add your own with the `property` keyword:

```qml
property <type> <name>: <value>
```

```qml
property string cardTitle: "Project Notes"
property int itemCount: 12
readonly property int margin: 24
```

### Property types

| Type | Holds | Example value |
|------|-------|---------------|
| `int` | Whole number | `12` |
| `real` | Number with decimals | `0.65` |
| `string` | Text | `"Project Notes"` |
| `bool` | `true` or `false` | `true` |
| `color` | A color name or `"#rrggbb"` | `"#2e86de"` |
| `url` | A file or web address | `"badge.png"` |
| `var` | Any value, also lists | `["low", "medium", "high"]` |

Rules to remember:

- Property names start with a **lowercase** letter.
- Strings and colors are written in **quotes**: `color: "red"`, not `color: red`.
- `readonly` means the value is set once and cannot be assigned later.

### Enums

An `enum` gives names to whole numbers, so the code says what a value means:

```qml
enum Level {
    Low,        // 0
    Medium,     // 1
    High        // 2
}
property int level: Main.Level.Medium
```

The enum belongs to the type that the file defines. `Main.qml` defines the type `Main`, so the value is written `Main.Level.Medium`. Enum value names start with a capital letter.

---

## 4. `id` and `parent`

Two names let items refer to each other.

**`id`** gives an item a name that other items **in the same file** can use:

```qml
Rectangle { id: track; width: 300 }
Text { text: "Track is " + track.width + " wide" }
```

- An `id` starts with a lowercase letter or `_`, and it is unique inside the file.
- It is not a property. You cannot write `track.id`, and other QML files cannot see it.

**`parent`** always means the item that contains the current one. It is useful when you do not need a name:

```qml
Rectangle {
    id: fill
    width: parent.width * 0.5    // parent is the track
    height: parent.height
}
```

Use `parent` for a quick reference to the surrounding item, and an `id` when you need to reach an item that is not the parent (for example `root.accent`).

---

## 5. Common Item Properties

Every visible type inherits these from `Item`:

| Property | Meaning | Default |
|----------|---------|---------|
| `x`, `y` | Position relative to the parent | `0` |
| `width`, `height` | Size | `0` for a plain `Item` or `Rectangle` |
| `z` | Stacking order: the higher value is drawn on top | `0` |
| `opacity` | `0.0` (invisible) to `1.0` (opaque). Children inherit it | `1.0` |
| `visible` | `false` hides the item and its children completely | `true` |
| `rotation` | Rotation in degrees around the item's center | `0` |
| `scale` | Size factor, drawn around the item's center | `1.0` |
| `clip` | `true` cuts off anything drawn outside the item | `false` |

Items with the **same** `z` are drawn in the order they appear in the file, so the later one is on top.

---

## 6. The Code

### CMakeLists.txt

It is the generated file from the chapter README, with the new names. The only addition is the `RESOURCES` list, which puts `badge.png` into the module so that `Image` can find it:

```cmake
qt_add_qml_module(app02_items_and_properties
    URI items_and_properties
    QML_FILES
        Main.qml
    RESOURCES
        badge.png
)
```

### main.cpp

```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("items_and_properties", "Main");

    return app.exec();
}
```

`loadFromModule("items_and_properties", "Main")` loads the type `Main` from the module whose URI is `items_and_properties`. The URI here must match the one in `CMakeLists.txt`.

### Main.qml

```qml
import QtQuick

Window {
    id: root
    width: 520
    height: 480
    visible: true
    title: qsTr("Items and Properties")
    color: "#eef1f5"

    // === Custom properties: "property <type> <name>: <value>" ===
    property string cardTitle: "Project Notes"
    property int itemCount: 12
    property real progress: 0.65
    property color accent: "#2e86de"
    property bool highlighted: true
    readonly property int margin: 24

    // === An enum, and a property that stores one of its values ===
    enum Level {
        Low,
        Medium,
        High
    }
    property int level: Main.Level.Medium

    // === The card: a Rectangle with children ===
    Rectangle {
        id: card
        x: root.margin
        y: root.margin
        width: root.width - 2 * root.margin
        height: 250
        radius: 10
        color: "white"
        border.color: root.highlighted ? root.accent : "#cccccc"
        border.width: 2

        // Image: the file comes from RESOURCES in CMakeLists.txt
        Image {
            id: badge
            source: "badge.png"
            width: 56
            height: 56
            fillMode: Image.PreserveAspectFit
            x: parent.width - width - 16       // "parent" is the card
            y: 16
        }

        Text {
            id: heading
            text: root.cardTitle
            font.pixelSize: 24
            font.bold: true
            color: root.accent
            x: 16
            y: 16
        }

        Text {
            id: subtitle
            text: root.itemCount + " items, level " + levelNames[root.level]
            color: "#555555"
            x: 16
            y: 52

            readonly property var levelNames: ["low", "medium", "high"]
        }

        // Progress bar: a track Rectangle with a fill Rectangle inside
        Rectangle {
            id: track
            x: 16
            y: 110
            width: parent.width - 32
            height: 14
            radius: 7
            color: "#dde3ea"

            Rectangle {
                id: fill
                width: parent.width * root.progress   // "parent" is the track
                height: parent.height
                radius: parent.radius
                color: root.accent
            }
        }

        Text {
            // "id" lets one item read the properties of another item
            text: Math.round(fill.width / track.width * 100) + " % done"
            color: "#555555"
            x: 16
            y: 134
        }

        // Common Item properties: opacity, rotation, scale, visible
        Rectangle {
            x: 16
            y: 175
            width: 60
            height: 40
            color: root.accent
            opacity: 0.4
        }
        Rectangle {
            x: 100
            y: 175
            width: 60
            height: 40
            color: root.accent
            rotation: 15
        }
        Rectangle {
            x: 190
            y: 175
            width: 60
            height: 40
            color: root.accent
            scale: 0.6
        }
        Rectangle {
            x: 280
            y: 175
            width: 60
            height: 40
            color: root.accent
            visible: false          // not drawn at all
        }
    }

    // === Stacking order: the "z" property ===
    Item {
        id: stack
        x: root.margin
        y: 300
        width: root.width - 2 * root.margin
        height: 120

        Rectangle {
            x: 0
            y: 0
            width: 90
            height: 90
            color: "#e74c3c"
            z: 1
        }
        Rectangle {
            x: 40
            y: 20
            width: 90
            height: 90
            color: "#27ae60"
            z: 3                    // the highest z is drawn on top
        }
        Rectangle {
            x: 80
            y: 40
            width: 90
            height: 90
            color: "#f1c40f"
            z: 2
        }

        Text {
            text: "z: 1, 3, 2"
            x: 200
            y: 40
            color: "#555555"
        }
    }
}
```

### What to look at in `Main.qml`

| Part | What it shows |
|------|---------------|
| `id: root` on the `Window` | Every other item reaches the window's properties as `root.something` |
| `property string cardTitle` and the other custom properties | Own properties with their types, and `readonly` for a fixed value |
| `enum Level` and `property int level` | An enum, and how its value is written (`Main.Level.Medium`) |
| `x: root.margin`, `y: root.margin` | Positioning with explicit coordinates |
| `width: root.width - 2 * root.margin` | A value calculated from other properties (chapter 04 explains why it updates) |
| `x: parent.width - width - 16` in the `Image` | `parent` is the card, and `width` is the image's own width |
| `fill.width / track.width` | One item reading another through its `id` |
| `border.color: root.highlighted ? ... : ...` | A condition (`? :`) as a property value |
| The four small rectangles | `opacity`, `rotation`, `scale` and `visible` |
| The three overlapping squares | `z` decides the drawing order |

---

## 7. Build and Run

In Qt Creator, open `CMakeLists.txt`, pick a Qt 6.8 or newer kit and press **Run**. From a terminal, in this folder:

```bash
cmake -S . -B build
cmake --build build
./build/app02_items_and_properties
```

Things to try:

- Change `accent` to another color and see the heading, the border and the progress bar change together.
- Set `highlighted` to `false` to get a gray border.
- Change `progress` to `0.9`, then change `level` to `Main.Level.High`.
- Swap the `z` values of the green and yellow squares.
- Remove `RESOURCES badge.png` from `CMakeLists.txt` and see what the console says.

---

## 8. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| An item does not appear at all | A `Rectangle` or `Item` has no `width` and `height`, so its size is `0` | Set both, or size it from its parent |
| The image does not appear, and the console says it cannot open the file | `badge.png` is not listed under `RESOURCES`, or the name has the wrong upper or lower case (Linux is case-sensitive) | Add the file to `RESOURCES` in `qt_add_qml_module` and check the spelling |
| `ReferenceError: red is not defined` | A color or string was written without quotes | Write `color: "red"` |
| `ReferenceError: root is not defined` | The `id` lives in another QML file, and ids work only inside their own file | Pass the value in as a property (chapter 07) |
| `Cannot assign to non-existent property` | A typo in a property name, or the property does not exist for that type | Check the name against the type's documentation page |
| An error about a property name | The custom property name starts with a capital letter | Start property names with a lowercase letter |
| The text is there but cannot be seen | Text and background have the same color | Change `color` of the `Text` |

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| `Item` (`x`, `y`, `z`, `opacity`, `visible`, `parent`) | <https://doc.qt.io/qt-6/qml-qtquick-item.html> |
| `Rectangle` | <https://doc.qt.io/qt-6/qml-qtquick-rectangle.html> |
| `Text` | <https://doc.qt.io/qt-6/qml-qtquick-text.html> |
| `Image` (`source`, `fillMode`) | <https://doc.qt.io/qt-6/qml-qtquick-image.html> |
| Custom properties, `readonly`, `id` | <https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html> |
| Value types (`int`, `real`, `string`, `color`, ...) | <https://doc.qt.io/qt-6/qtqml-typesystem-valuetypes.html> |
| Object declarations | <https://doc.qt.io/qt-6/qtqml-syntax-basics.html> |
| Module | `import QtQuick` (Qt Quick) |
| Minimum Qt version | Qt 6.8 |
