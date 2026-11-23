# 04_components

Turn a group of items into your own reusable component. In `03_layout_and_anchors` you built one card. Here you build it once, as a component named `InfoCard`, and place it three times with different texts and colors. Change the component once and all three cards change.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings.

The same idea written by hand is in `11_QML/07_custom_components`.

---

## 1. What You Build

Three cards in a row. Each card has an accent bar, an icon, a title and a subtitle. They are all instances of one component, and only the texts and the accent color differ.

| File | Purpose |
|------|---------|
| `InfoCard.ui.qml` | The component: the card with its three public properties |
| `Screen01.ui.qml` | The scene with three `InfoCard` instances in a `Row` |
| `App.qml` | The window (same as in lesson 03) |
| `leaf.png` | The image from lesson 02 |

---

## 2. Component and Instance

| Term | Meaning |
|------|---------|
| **Component** | The definition, a QML file. The file name is the type name: `InfoCard.ui.qml` gives the type `InfoCard` |
| **Instance** | One use of the type in a scene: `InfoCard { title: "Plants" }` |
| **Property alias** | A property of the component that points to a property of an item inside it. It is the public interface of the component |

You can create custom components with a project wizard template, or by moving a component instance into a separate file. The second way is the one used here.

Design Studio lists your components under **Components > My Components** only if the file name begins with a capital letter. This is also a QML rule: type names start with a capital letter.

---

## 3. Steps

### 3.1 Create the project

Create `Card_components` with the settings of the [area README](../README.md). Import `leaf.png` with the Plus button in Assets. Use `App.qml` from lesson 03, but keep the `import` line that Design Studio generated for your project name.

### 3.2 Build one card as normal items

In `Screen01.ui.qml`, build one card the way you did in lesson 03: a white Rectangle (`220 x 240`, `radius` 16) with a thin colored Rectangle on top (the accent bar), an Image, and two Text items. Anchor the children to the card.

### 3.3 Move it into a component

1. Right-click the card in the Navigator or the 2D view and select **Create Component**.
2. In the dialog, name the component `InfoCard` and keep the suggested file location (the folder of `Screen01.ui.qml`).
3. Design Studio creates `InfoCard.ui.qml` and replaces the card in the scene with an `InfoCard` instance.

To change the component itself, right-click an instance and select **Go into Component**. The breadcrumb at the top of the 2D view shows where you are, and you can go back to `Screen01` from there.

### 3.4 Export the properties

A component hides its inner items. To change the texts of one instance from outside, export them as property aliases.

1. Go into the component.
2. In the Navigator, find the **Export** button of the title text and of the subtitle text and click it. Design Studio writes a `property alias` line.
3. Rename the aliases to `title` and `subtitle`, and do the same for the accent bar color, named `accent`.

An alias is a property that you can use from outside the component. In the Text Editor it looks like an ordinary property, but with the `alias` keyword instead of a type. The docs recommend that content that is data-driven should be exported as a public property.

You cannot use the root item of the component as an alias target.

### 3.5 Use the component

1. Go back to `Screen01.ui.qml`.
2. Drag two more `InfoCard` instances from **Components > My Components** into the scene, or copy and paste the first one.
3. Select each instance and set `title`, `subtitle` and `accent` in the Properties panel. The new properties appear there.
4. Put the three instances into a **Row** with `spacing` 24 and center it with anchors (see lesson 03).
5. Press **Alt+P**.

### 3.6 Change all instances at once

Go into the component and change the `radius` of the card or the font size of the title. Go back: all three instances changed.

---

## 4. The Code

### 4.1 `InfoCard.ui.qml`

```qml
import QtQuick

Rectangle {
    id: root
    width: 220
    height: 240
    color: "#ffffff"
    radius: 16
    border.color: "#c9d6cf"
    border.width: 2

    property alias title: titleText.text
    property alias subtitle: subtitleText.text
    property alias accent: accentBar.color

    Rectangle {
        id: accentBar
        height: 8
        radius: 4
        color: "#2e7d5a"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 16
    }

    Image {
        id: icon
        width: 80
        height: 80
        source: "leaf.png"
        fillMode: Image.PreserveAspectFit
        anchors.top: accentBar.bottom
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: titleText
        text: "Title"
        font.pixelSize: 22
        color: "#1d4d38"
        anchors.top: icon.bottom
        anchors.topMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: subtitleText
        text: "Subtitle"
        font.pixelSize: 15
        color: "#5b6b63"
        anchors.top: titleText.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
```

| Part | Meaning |
|------|---------|
| File name `InfoCard.ui.qml` | Defines the type `InfoCard` |
| `property alias title: titleText.text` | The public property `title` is the `text` of the inner item `titleText`. Setting `title` from outside sets that text |
| The default values (`"Title"`, `#2e7d5a`) | What an instance shows when it does not set the property |
| Anchors inside | Children are tied to the card, so the component keeps its look at any size |

### 4.2 `Screen01.ui.qml`

```qml
import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Row {
        id: cardRow
        spacing: 24
        anchors.centerIn: parent

        InfoCard {
            id: cardOne
            title: "Plants"
            subtitle: "12 items"
        }

        InfoCard {
            id: cardTwo
            title: "Water"
            subtitle: "3 items"
            accent: "#2f6fb0"
        }

        InfoCard {
            id: cardThree
            title: "Sun"
            subtitle: "7 items"
            accent: "#d9a21b"
        }
    }
}
```

| Part | Meaning |
|------|---------|
| `InfoCard { ... }` | An instance. No import is needed because the file is in the same folder |
| `title`, `subtitle`, `accent` | The aliases from the component, set per instance |
| `cardOne` has no `accent` | It uses the default color from the component |
| `Row` | A positioner that places the three cards side by side with `spacing` |

---

## 5. Try This

1. Add a fourth property alias, `icon`, for the `source` of the image, and give each card a different image.
2. Go into the component and change `radius` to 40. All instances change.
3. Set `opacity: 0.5` on only one instance in `Screen01`. Only that card changes.
4. Rename `InfoCard.ui.qml` to `infoCard.ui.qml` and see that Design Studio no longer lists it under My Components.
5. Add a `Text` with `text: root.title + "!"` inside the component. A binding like this still works in a `.ui.qml` file.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| The component is not in My Components | The file name does not start with a capital letter, or the file is not saved in the project |
| `QML module not found (Card_components)` | The `import` line in `App.qml` must name **your** project. If your project is called `Components`, the line is `import Components`. Do not paste the import from the tutorial, keep the one Design Studio generated |
| `InfoCard is not a type` | The component file is in another folder, so the instance needs an `import`, or the module has no `qmldir` entry. Keep both files in the same folder |
| The new property is missing in Properties | The alias was not exported, or you are on an instance of the old version. Save and reopen the scene |
| The image is empty in the instances | The `source` path is relative to the component file. Check where Assets put `leaf.png` (usually next to the QML files) and fix the path in `InfoCard.ui.qml` |
| Changing one instance changes the others | You changed the component, not the instance. Check the breadcrumb at the top of the 2D view |
| The alias option is disabled | Export an inner item, not the root item of the component |

---

## 7. Git

Commit this README, `InfoCard.ui.qml`, `Screen01.ui.qml`, `App.qml` and `leaf.png`. See the area README for the files that stay out of Git.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Creating custom components | <https://doc.qt.io/qtdesignstudio/quick-components-creating.html> |
| Components overview and guidelines | <https://doc.qt.io/qtdesignstudio/quick-components.html> |
| Navigator: property aliases and Export | <https://doc.qt.io/qtdesignstudio/qtquick-navigator.html> |
| Creating component instances | <https://doc.qt.io/qtdesignstudio/quick-component-instances.html> |
| Property aliases in QML | <https://doc.qt.io/qt-6/qml-QtQml-qmlpropertyalias.html> |
| Defining object types in QML | <https://doc.qt.io/qt-6/qtqml-documents-definetypes.html> |
