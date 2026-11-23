# 09_bindings_and_properties

Let one value drive many things. A **custom property** stores a value, and **bindings** make other properties follow it automatically. Here a single property, `level`, controls a number, a progress bar, a color and a warning text. The two buttons only change `level`.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings, and the buttons were clicked with a test script (section 4).

The same idea written by hand is in `11_QML/04_property_bindings`.

---

## 1. What You Build

A card with a percentage, a bar and two buttons.

| What you see | How it is bound |
|--------------|-----------------|
| The text `40 %` | `text` is bound to `root.level + " %"` |
| The width of the bar | `width` is bound to `track.width * root.level / 100` |
| The color of the text and the bar | Both are bound to the custom property `accent`, which is bound to `warning` |
| The text `High` | `visible` is bound to `root.warning`, where `warning` is `level > 80` |

The buttons `-10` and `+10` change `level` and nothing else. All the other changes follow by themselves.

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The scene with the custom properties, bindings and connections |
| `App.qml` | The window |

---

## 2. Terms

| Term | Meaning |
|------|---------|
| **Custom property** | A property you add to a component. It has a name, a type and a value |
| **Binding** | A property is set to an expression instead of a fixed value. When a value in the expression changes, the property updates by itself |
| **Assignment** | Setting a property to a fixed value. It **removes** the binding of that property |

Custom property types in Design Studio:

| Type | Holds |
|------|-------|
| `int` | A whole number |
| `real` | A number with a decimal point |
| `string` | Text |
| `bool` | `true` or `false` |
| `color` | A color name or a hex value |
| `url` | A file path or a web address |
| `variant` | Any value: numbers, strings, objects, arrays |
| `alias` | A reference to another property (lesson 04) |
| `vector2d`, `vector3d`, `vector4d` | Values with x, y, (z), (w) parts |

---

## 3. Steps

### 3.1 Create the project

Create `Bindings_and_properties` with the settings of the [area README](../README.md). Keep the `import` line that Design Studio generated in `App.qml`.

### 3.2 Build the scene without bindings

In `Screen01.ui.qml` build the card, the percentage text, the `High` text, the track (a gray rounded Rectangle, 320 x 20), the fill (a green rounded Rectangle on the track) and two buttons with a Mouse Area each. Use the ids `percentText`, `warningText`, `track`, `fill`, `minusArea` and `plusArea`.

Position the track and the fill with `x` and `y`, not with anchors, because the fill `width` will be bound.

### 3.3 Add custom properties

You can add custom properties in two places.

**In the Properties view:**
1. Select the root item `Screen01`.
2. Find **Local Custom Properties** and click the **Plus** button.
3. In the Add New Property dialog set the **Name** and the **Type**.

**In the Connections view:**
1. Select the item and open the **Properties** tab of the Connections view.
2. Click the **Plus** button.
3. In the custom property editor choose the **Type**, set the **Name** and enter a **Value**.

Add these three to the root item:

| Name | Type | Value |
|------|------|-------|
| `level` | `int` | `40` |
| `warning` | `bool` | the expression `level > 80` (a binding, see below) |
| `accent` | `color` | the expression `warning ? "#c0392b" : "#2e7d5a"` |

`warning` and `accent` are not fixed values. They contain an expression, so they are bindings. To set an expression in a property field, open the **Action** menu next to the field and choose **Set Binding**, then type the expression in the Binding Editor.

### 3.4 Add bindings

You can add bindings in two ways.

**In the Properties view:** click the **Action** button next to a property, choose **Set Binding** and enter the expression:

| Item | Property | Expression |
|------|----------|------------|
| `percentText` | `text` | `root.level + " %"` |
| `percentText` | `color` | `root.accent` |
| `fill` | `width` | `track.width * root.level / 100` |
| `fill` | `color` | `root.accent` |
| `warningText` | `visible` | `root.warning` |

**In the Connections view:** select the target item, open the **Bindings** tab and click the **Plus** button. In the Bindings editor choose the source component and its property under **From**, and the target property under **To**. This way is good for a direct link such as one color following another.

Properties with a binding are shown differently from fixed values in the Properties panel. Look at the Text Editor to see the expressions.

### 3.5 Connect the buttons

Add two connections as in lesson 08, using **Manual Edit** for the JavaScript:

| Item | Signal | Code |
|------|--------|------|
| `minusArea` | `clicked` | `root.level = Math.max(0, root.level - 10)` |
| `plusArea` | `clicked` | `root.level = Math.min(100, root.level + 10)` |

`Math.max` and `Math.min` keep the level between 0 and 100.

### 3.6 Preview

Press **Alt+P** and click the buttons. The number, the bar, the color and the `High` text follow `level`.

To try it without the Live Preview, change the value of `level` in the Properties view or in the Text Editor and watch the 2D view.

---

## 4. The Code

```qml
import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    // custom properties of this component
    property int level: 40
    property bool warning: level > 80
    property color accent: warning ? "#c0392b" : "#2e7d5a"

    Rectangle {
        id: panel
        width: 400
        height: 300
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Text {
            id: percentText
            text: root.level + " %"
            font.pixelSize: 48
            color: root.accent
            anchors.top: parent.top
            anchors.topMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: warningText
            text: "High"
            font.pixelSize: 18
            color: "#c0392b"
            visible: root.warning
            anchors.top: percentText.bottom
            anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: track
            width: 320
            height: 20
            color: "#dfe8e3"
            radius: 10
            x: 40
            y: 140

            Rectangle {
                id: fill
                width: track.width * root.level / 100
                height: parent.height
                color: root.accent
                radius: 10
            }
        }

        Row {
            id: buttonRow
            spacing: 16
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: minusButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "-10"
                    font.pixelSize: 20
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: minusArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: plusButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "+10"
                    font.pixelSize: 20
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: plusArea
                    anchors.fill: parent
                }
            }
        }
    }

    Connections {
        target: minusArea
        function onClicked() { root.level = Math.max(0, root.level - 10) }
    }

    Connections {
        target: plusArea
        function onClicked() { root.level = Math.min(100, root.level + 10) }
    }
}
```

| Part | Meaning |
|------|---------|
| `property int level: 40` | A custom property with a fixed start value. Only the buttons change it |
| `property bool warning: level > 80` | A bound property. It is `true` while the level is above 80 |
| `property color accent: warning ? ... : ...` | Bound to `warning`. It is red when `warning` is true, green otherwise |
| `width: track.width * root.level / 100` | The bar is as wide as `level` percent of the track |
| `visible: root.warning` | The `High` text is shown only while `warning` is true |
| `root.level = ...` | An **assignment**, which is fine here because `level` has no binding of its own |

What the test script saw with Qt 6.12.0:

| Action | `level` | Text | Fill width | Color | `High` visible |
|--------|---------|------|-----------|-------|----------------|
| Start | 40 | `40 %` | 128 | green | no |
| After 4 clicks on `+10` | 80 | `80 %` | 256 | green | no |
| After 5 clicks | 90 | `90 %` | 288 | red | yes |
| After 6 clicks | 100 | `100 %` | 320 | red | yes |
| One more `+10` | 100 | `100 %` | 320 | red | yes |
| Then `-10` | 90 | `90 %` | 288 | red | yes |

The level stays at 100 at the top, and the colors change by themselves when it crosses 80.

---

## 5. Try This

1. Change the limit `80` in the `warning` binding to `50`. Everything that depends on it follows.
2. Add a `string` custom property `unit` with the value `%` and use it in the binding of `percentText`: `root.level + " " + root.unit`.
3. **Break a binding:** in the Properties view type a fixed number into the `width` of `fill`. The Action menu now shows that the binding is gone, and the bar no longer follows `level`. Use the Action menu to **Set Binding** again to restore it.
4. Bind the opacity of `track` to `root.level / 100 * 0.5 + 0.5`.
5. Add a second custom property `step` (`int`, value `10`) and use it in the button code instead of the fixed `10`.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| The bar does not follow `level` | The `width` was set to a fixed number, which removed the binding. Set the binding again |
| `ReferenceError: root is not defined` | The root item has no id `root`, or the binding is in a different file. Use the id of the item that owns the property |
| `Binding loop detected` | Two properties depend on each other, for example `width` bound to something that depends on `width`. Remove one of the bindings |
| The color property shows a text error | A color binding must return a color. Use `"#c0392b"` with quotes in the expression, not a bare word |
| The `High` text never appears | `warning` is never true, because the condition is wrong or `level` is not changing. Check `level` in the Properties view |
| A property of an instance is not editable | It is bound in the component. Override it on the instance with a fixed value, or export it with an alias (lesson 04) |

---

## 7. Git

Commit this README, `Screen01.ui.qml` and `App.qml`. See the area README for the files that stay out of Git.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Adding bindings between properties | <https://doc.qt.io/qtdesignstudio/quick-property-bindings.html> |
| Specifying custom properties | <https://doc.qt.io/qtdesignstudio/quick-dynamic-properties.html> |
| The Connections view | <https://doc.qt.io/qtdesignstudio/qtquick-connection-view.html> |
| Creating UI logic | <https://doc.qt.io/qtdesignstudio/qtquick-creating-ui-logic.html> |
| Property binding in QML | <https://doc.qt.io/qt-6/qtqml-syntax-propertybinding.html> |
| Property attributes (custom properties) | <https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html> |
