import QtQuick
import QtQuick.Controls

Window {
    width: 600
    height: 460
    visible: true
    title: qsTr("Property Bindings")
    color: "#eef1f5"

    // A slider drives all three boxes. (Controls such as Slider are the topic of lesson 06.)
    // "from", "to" and "value" are properties like any other.
    Slider {
        id: slider
        from: 20
        to: 160
        value: 80
        x: 16
        y: 12
        width: parent.width - 32
    }

    // === 1. A binding: the expression after the colon is kept alive ===
    // Qt sees that "width" depends on "slider.value" and evaluates the line again every time it changes.
    // "height: width / 2" is a binding on a binding: it depends on this box's own width.
    Text {
        x: 16
        y: 60
        text: qsTr("1. Binding:  width: slider.value;  height: width / 2")
        font.bold: true
    }
    Rectangle {
        id: boundBox
        x: 16
        y: 84
        width: slider.value
        height: width / 2
        color: "#2e86de"
    }
    Text {
        x: 220
        y: 84
        text: qsTr("size: %1 x %2").arg(Math.round(boundBox.width)).arg(Math.round(boundBox.height))
    }

    // === 2. An assignment breaks a binding ===
    Text {
        x: 16
        y: 160
        text: qsTr("2. A plain assignment replaces the binding")
        font.bold: true
    }
    Rectangle {
        id: brokenBox
        x: 16
        y: 184
        width: slider.value
        height: 40
        color: "#27ae60"
    }
    Row {
        x: 220
        y: 184
        spacing: 8
        Button {
            text: qsTr("Assign width = 100")
            // An assignment in JavaScript: the binding "width: slider.value" is gone for good.
            onClicked: brokenBox.width = 100
        }
        Button {
            text: qsTr("Restore binding")
            // Qt.binding() makes a new binding from a function, so the box follows the slider again.
            onClicked: brokenBox.width = Qt.binding(function () { return slider.value })
        }
    }
    Text {
        x: 220
        y: 232
        text: qsTr("width: %1").arg(Math.round(brokenBox.width))
    }

    // === 3. The Binding type: a binding that can be switched on and off ===
    Text {
        x: 16
        y: 280
        text: qsTr("3. Binding { when: ... }")
        font.bold: true
    }
    Rectangle {
        id: gatedBox
        x: 16
        y: 304
        width: 100              // the normal value, used while the Binding is off
        height: 40
        color: "#e67e22"
    }
    Binding {
        target: gatedBox
        property: "width"
        value: slider.value
        when: follow.checked    // active only while the check box is ticked
    }
    CheckBox {
        id: follow
        x: 220
        y: 304
        text: qsTr("Follow the slider")
    }
    Text {
        x: 220
        y: 352
        text: qsTr("width: %1").arg(Math.round(gatedBox.width))
    }
}
