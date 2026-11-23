# 10_controls_and_styles

Use ready-made controls (switch, slider, check box, combo box, text field, button) and change the look of all of them at once with a **style**. Until now every button was a Rectangle with a Mouse Area. Qt Quick Controls give you real buttons with states, focus, keyboard handling and a consistent look.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 with the styles Basic, Fusion, Material and Universal, without warnings. The picture in section 4 was rendered by this test.

The same controls written by hand are in `11_QML/06_quick_controls`. Styling is also part of `11_QML/17_styling_and_resources`.

---

## 1. What You Build

A settings card with six controls. Everything is connected with bindings, so no connection code is needed.

| Control | Id | What it does |
|---------|----|--------------|
| `Switch` | `notifySwitch` | Turns notifications on or off |
| `CheckBox` | `agreeCheck` | Must be ticked before saving |
| `ComboBox` | `modeBox` | Chooses Quiet, Normal or Loud |
| `Slider` | `volumeSlider` | Sets the volume from 0 to 100 |
| `TextField` | `nameField` | Enter a name |
| `Button` | `saveButton` | `enabled` is bound to `agreeCheck.checked` |

A text at the bottom shows a summary that is bound to the switch, the combo box and the slider.

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The scene with the controls |
| `App.qml` | The window |
| `styles.png` | The same scene in four styles |

---

## 2. Terms

| Term | Meaning |
|------|---------|
| **Control** | A ready-made interactive component from the module `QtQuick.Controls` |
| **Style** | A set of looks for all controls: Basic, Fusion, Material, Universal and others |
| **Basic style** | A simple, light all-round style with the best performance |
| **Fusion style** | A platform-independent style with a desktop look |
| **Material style** | Based on the Google Material Design guidelines. It needs more system resources than Basic |
| **Universal style** | Based on the Microsoft Universal Design guidelines. It needs more system resources than Basic |
| **`qtquickcontrols2.conf`** | A configuration file in the project that names the style and its settings |

The properties that you set on a control are the same in every style. Only the look changes. That is why you design with the controls once and choose the style at the end.

---

## 3. Steps

### 3.1 Create the project

Create `Controls_and_styles` with the settings of the [area README](../README.md). Choose the style **Basic**. Keep the `import` line that Design Studio generated in `App.qml`.

### 3.2 Add the controls

1. In the **Components** view find the Qt Quick Controls (Switch, CheckBox and so on). Use the search field of the Components view to find a control by name.
2. Drag the controls onto the card: a Switch, a CheckBox, a ComboBox, a Slider, a TextField and a Button. Put the left three in a **Column** and the right three in another **Column**, so the spacing stays even.
3. Set the ids from section 1.
4. For each control set the text or the starting value in the Properties panel:

| Control | Properties |
|---------|------------|
| `notifySwitch` | `text` `Notifications`, `checked` on |
| `agreeCheck` | `text` `I agree to the terms` |
| `modeBox` | `model` `["Quiet", "Normal", "Loud"]`, `currentIndex` 1 |
| `volumeSlider` | `from` 0, `to` 100, `stepSize` 1, `value` 40 |
| `nameField` | `placeholderText` `Your name` |
| `saveButton` | `text` `Save` |

### 3.3 Add the bindings

Set the bindings with the **Action** menu next to a property and **Set Binding** (lesson 09):

| Item | Property | Expression |
|------|----------|------------|
| `volumeText` | `text` | `"Volume: " + Math.round(volumeSlider.value)` |
| `saveButton` | `enabled` | `agreeCheck.checked` |
| `summaryText` | `text` | see section 4 |

Press **Alt+P**. The button stays gray until the check box is ticked, and the text follows the slider.

### 3.4 Change the style

The style of a project is stored in `qtquickcontrols2.conf`, which is in the content folder of every Design Studio project. It looks like this:

```ini
[Controls]
Style=Basic
```

To see another style:

1. Use the **Style** selector in the bottom bar of Design Studio (it sits next to the **Kit** selector), or
2. edit `Style=` in `qtquickcontrols2.conf` and reopen the preview.

Try `Fusion`, `Material` and `Universal`. The scene stays the same and the controls change.

Style settings go into a section named after the style:

```ini
[Controls]
Style=Material

[Material]
Theme=Dark
Accent=Teal
```

The other styles have their own sections too. See the configuration file page in the reference for the keys.

---

## 4. The Code

```qml
import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Rectangle {
        id: panel
        width: 680
        height: 380
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Text {
            id: titleText
            x: 32
            y: 24
            text: "Settings"
            font.pixelSize: 28
            color: "#1d4d38"
        }

        Column {
            id: leftColumn
            x: 32
            y: 84
            width: 280
            spacing: 18

            Switch {
                id: notifySwitch
                text: "Notifications"
                checked: true
            }

            CheckBox {
                id: agreeCheck
                text: "I agree to the terms"
            }

            ComboBox {
                id: modeBox
                width: 240
                model: ["Quiet", "Normal", "Loud"]
                currentIndex: 1
            }
        }

        Column {
            id: rightColumn
            x: 352
            y: 84
            width: 296
            spacing: 18

            Text {
                id: volumeText
                text: "Volume: " + Math.round(volumeSlider.value)
                font.pixelSize: 18
                color: "#1d4d38"
            }

            Slider {
                id: volumeSlider
                width: 280
                from: 0
                to: 100
                stepSize: 1
                value: 40
            }

            TextField {
                id: nameField
                width: 280
                placeholderText: "Your name"
            }

            Button {
                id: saveButton
                text: "Save"
                enabled: agreeCheck.checked
            }
        }

        Text {
            id: summaryText
            x: 32
            y: 318
            width: 616
            text: (notifySwitch.checked ? "Notifications on" : "Notifications off")
                  + ", mode " + modeBox.currentText
                  + ", volume " + Math.round(volumeSlider.value)
            font.pixelSize: 16
            color: "#5b6b63"
        }
    }
}
```

| Part | Meaning |
|------|---------|
| `import QtQuick.Controls` | Brings in the controls. The style is chosen at run time, from `qtquickcontrols2.conf` |
| `Switch`, `CheckBox`, `ComboBox`, `Slider`, `TextField`, `Button` | The controls. Each has its own properties: `checked`, `currentIndex`, `value`, `text` |
| `enabled: agreeCheck.checked` | A binding. The button is disabled until the box is ticked |
| `model: ["Quiet", "Normal", "Loud"]` | A list of strings as the model of the combo box |
| `modeBox.currentText` | The text of the selected entry |
| `Math.round(volumeSlider.value)` | The slider value is a real number, so it is rounded for the text |

Test with Qt 6.12.0: in the Basic style `saveButton.enabled` was `false` at the start and `true` after `agreeCheck.checked` was set.

The same scene in four styles:

![The scene in Basic, Fusion, Material and Universal style](styles.png)

The text of the controls looks small because the pictures were rendered without a screen. On a normal screen the controls are larger.

---

## 5. Try This

1. Change `Style=Basic` to `Material` in `qtquickcontrols2.conf`, and add `[Material]` with `Theme=Dark`. Look at the card.
2. Change the `Slider` range to `from: 10`, `to: 50`.
3. Add a second `ComboBox` and give it a model of three numbers.
4. Bind the `visible` of the `TextField` to `notifySwitch.checked`.
5. Click the button with the check box ticked and use a connection (lesson 08) to print the name from `nameField.text`.
6. Add a `Button` with `flat: true` and compare it in the four styles.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| The controls look the same in every style | The preview does not read the configuration you changed. Save `qtquickcontrols2.conf` and restart the preview |
| A control is missing in the Components view | Search for it by name, or check that the project uses Qt Quick Controls (the preset has the module) |
| The button stays gray | `enabled` is bound to something that is `false`. Tick the check box, or check the binding |
| `module "QtQuick.Controls" is not installed` | The Qt version of the kit does not contain Qt Quick Controls. Install the module with the Maintenance Tool |
| Custom colors have no effect in a style | A style can ignore a property that it draws itself. Use the settings of the style in `qtquickcontrols2.conf`, or style the control itself |
| Control sizes differ between styles | Every style has its own padding and fonts. Anchor and size the layout with some room |

---

## 7. Git

Commit this README, `Screen01.ui.qml`, `App.qml`, `styles.png` and `qtquickcontrols2.conf` if you changed it. See the area README for the files that stay out of Git.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Styling Qt Quick Controls (available styles, selection) | <https://doc.qt.io/qt-6/qtquickcontrols-styles.html> |
| Qt Quick Controls overview | <https://doc.qt.io/qt-6/qtquickcontrols-index.html> |
| Qt Quick Controls configuration file | <https://doc.qt.io/qt-6/qtquickcontrols-configuration.html> |
| `Switch` | <https://doc.qt.io/qt-6/qml-qtquick-controls-switch.html> |
| `Slider` | <https://doc.qt.io/qt-6/qml-qtquick-controls-slider.html> |
| `ComboBox` | <https://doc.qt.io/qt-6/qml-qtquick-controls-combobox.html> |
