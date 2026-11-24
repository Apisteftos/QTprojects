# 07_custom_components

A QML file is a **type**. Put a rectangle with a text into `AppButton.qml`, and you can write `AppButton { }` everywhere in the same module. This lesson builds two reusable types, `AppButton` and `Card`, and uses `Component` to create items on demand.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with two small changes for that old version, which are not part of this folder). An automated check clicked the buttons, checked the enabled state, the sizes and the badges that are created at run time (13 checks, all passed), and a screenshot was looked at. Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A window with two cards:

- **Likes**: a text and two `AppButton`s. **Unlike** is red and is off while there are no likes.
- **Badges**: a button that adds an orange badge to a row each time it is clicked. The badges are created from a `Component`.

---

## 2. A QML File Is a Type

```
07_custom_components/
    Main.qml        the window
    AppButton.qml   the type AppButton
    Card.qml        the type Card
```

Rules:

- The file name is the type name. It **must start with a capital letter**: `AppButton.qml`, not `appButton.qml`.
- The files of one module are visible to each other without an `import`. `Main.qml` uses `Card` and `AppButton` directly. (A type from another module needs an `import`.)
- **Every file must be listed in `CMakeLists.txt`**, in `QML_FILES` of `qt_add_qml_module`:

```cmake
qt_add_qml_module(app07_custom_components
    URI custom_components
    QML_FILES
        Main.qml
        AppButton.qml
        Card.qml
)
```

A new file that is not in the list is not packaged into the program, and the type is "not found" at run time. (In Qt Creator, a new QML file is added to the list by the wizard.)

The root item of the file is the type that you extend. `AppButton.qml` starts with a `Rectangle`, so an `AppButton` **is** a `Rectangle` and has all its properties (`color`, `width`, `radius`, `anchors`...).

---

## 3. The Interface of a Type

Inside the file, the details are private. The outside sees only what you declare on the **root** item:

```qml
Rectangle {
    id: root
    property alias text: label.text      // the interface
    property color baseColor: "#2e86de"
    signal clicked()

    Text { id: label }                   // an implementation detail
}
```

| Tool | Meaning |
|------|---------|
| `property <type> name: value` | A new property. Users can set it and bind it |
| **`property alias name: other.property`** | A property that **is** a property of an inner object. `AppButton { text: "Like" }` sets `label.text`, which the user of the type cannot see |
| `signal name(...)` | An event of the type (lesson 05). The button emits `clicked` when its inner `MouseArea` is clicked |
| `function name() { }` | A method that users can call |
| `readonly property` | A value that only the type itself changes |

An `id` is visible **only in its own file**. The users of `AppButton` cannot write `label`. This is the privacy of a type: the file can be changed inside as long as the interface stays the same. The root item gets the id `root` by habit.

**`implicitWidth` and `implicitHeight`** are the size a type wants. If the user sets no `width`, the layouts use the implicit size. `AppButton` computes its implicit width from the text: `label.implicitWidth + 32`. This is why buttons with different texts have different widths and the text always fits.

**`enabled`** is a property of every item: a disabled item and its children do not get mouse events. The button uses it for the color (`!enabled ? gray : ...`) and for its `MouseArea`.

---

## 4. `default property`: Children of Your Own Type

In `Main.qml`, a `Card` contains a `Column` as a child:

```qml
Card {
    title: "Likes"
    Column { ... }          // this becomes a child of the "body" item inside Card
}
```

Normally, children of an object go into its own `data` list. `Card.qml` declares

```qml
default property alias content: body.data
```

so the children that are written in `Card { ... }` are put into the inner item `body`. This is how a type becomes a **container** that places its content in the right place. Without it, the `Column` would be placed in the corner of the card, over the title. The height of the card is computed from `body.childrenRect.height`, so the card grows with its content.

---

## 5. `Component`: Create Items on Demand

```qml
Component {
    id: badge
    Rectangle { property string text; ... }
}

onClicked: badge.createObject(badgeRow, { text: "#3" })
```

A `Component` is a **template**: its content is not created at the start, it is only described. `createObject(parent, properties)` makes a new item from it, with the given parent and start values. Use this when the number of items is not known when you write the file.

Where else you meet a `Component`:

- The file of a type is a component too (`Qt.createComponent("Card.qml")` loads it).
- `Loader { sourceComponent: badge }` creates an item and can remove it (lesson 16).
- Delegates of lists (lesson 10) are components that the list creates for every row.
- Items created with `createObject` are **not** removed automatically. Call `destroy()` on them when they are not needed. They are removed when their parent is destroyed.

For a list of items that comes from data, `Repeater` and `ListView` (lesson 10) are better than `createObject`.

---

## 6. Inline Components

Lesson 03 and 05 used `component Box: Rectangle { ... }` **inside** a file. That is the same idea for a type that is small and needed in one file only. Use a separate file when a type is used in more than one file, or when it is more than about thirty lines.

---

## 7. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app07_custom_components
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Add a third `AppButton` with another `baseColor` and a long text. Watch its width.
2. Add a `property alias subtitle` to `Card` that is a smaller `Text` under the title.
3. Remove the line with `default property alias content` and look at the result.
4. In `Main.qml`, try to read `label.text` of a button. What does the console say?
5. Add a `Card` with the title **Badges 2** that uses the same `badge` `Component`.
6. Make the badge `Component` a file `Badge.qml` of its own (and add it to `QML_FILES`).

---

## 8. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `AppButton is not a type` | The file is not in `QML_FILES`, its name starts with a small letter, or CMake was not run again | Add it, rename it, run CMake again |
| The new file is not found, but exists | It is in a sub-folder | The folder name becomes part of the path; keep files in the same folder, or add an `import` |
| `ReferenceError: label is not defined` | An `id` of another file | Use the `alias` or a property |
| Children of the type appear in the wrong place | No `default property alias` | Add it (section 4) |
| The button has size 0 | No `width` and no `implicitWidth` | Give the type an implicit size |
| The alias shows nothing | The alias points to a wrong object, or the target has no such property | Check the right side: `label.text` |
| `Cannot assign to non-existent property "x"` | Typing mistake in the user of the type | Check the properties of the root item |
| The program uses more and more memory | Items made with `createObject` are never destroyed | `destroy()` them |

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Defining object types with QML | <https://doc.qt.io/qt-6/qtqml-documents-definetypes.html> |
| Property attributes (`alias`, `default`, `readonly`) | <https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html> |
| `Component` | <https://doc.qt.io/qt-6/qml-qtqml-component.html> |
| Dynamic object creation | <https://doc.qt.io/qt-6/qtqml-javascript-dynamicobjectcreation.html> |
| `qt_add_qml_module` | <https://doc.qt.io/qt-6/qt-add-qml-module.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Imports | `import QtQuick` |
| Minimum Qt version | Qt 6.8 |
