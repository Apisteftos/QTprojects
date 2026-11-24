import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "logic.js" as Logic

// A number with a button for "less" and a button for "more".
RowLayout {
    id: root

    property int value: 1
    property int from: 0
    property int to: 10

    signal limitReached(string which)      // "min" or "max"

    function increment() {
        if (value >= to) {
            limitReached("max")
            return
        }
        value += 1
    }

    function decrement() {
        if (value <= from) {
            limitReached("min")
            return
        }
        value -= 1
    }

    function setValue(newValue) {
        value = Logic.clamp(newValue, from, to)
    }

    Button {
        objectName: "minus"
        text: "-"
        enabled: root.value > root.from
        onClicked: root.decrement()
    }
    Label {
        objectName: "valueLabel"
        text: root.value
        horizontalAlignment: Text.AlignHCenter
        Layout.minimumWidth: 40
    }
    Button {
        objectName: "plus"
        text: "+"
        enabled: root.value < root.to
        onClicked: root.increment()
    }
}
