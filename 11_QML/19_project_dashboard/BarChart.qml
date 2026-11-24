import QtQuick

// Bars side by side. The bars are made once; only their heights change, and Behavior animates that.
Item {
    id: chart

    property var values: []             // the numbers, the newest at the end
    property int capacity: 24           // how many bars
    property real maximum: 100          // the value of a full bar
    property real warningAt: 60
    property real criticalAt: 85

    readonly property real gap: 3

    Row {
        anchors.fill: parent
        spacing: chart.gap

        Repeater {
            // A number as the model: "capacity" delegates, with the values 0, 1, 2 ... in "index"
            model: chart.capacity

            delegate: Item {
                id: slot
                required property int index
                // The newest value is at the right: with fewer values than bars, the left bars stay empty.
                readonly property int offset: chart.capacity - chart.values.length
                readonly property real v: index >= offset ? chart.values[index - offset] : 0

                width: (chart.width - (chart.capacity - 1) * chart.gap) / chart.capacity
                height: chart.height

                Rectangle {
                    objectName: "bar"
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: chart.height * Math.min(slot.v, chart.maximum) / chart.maximum
                    radius: 3
                    color: slot.v >= chart.criticalAt ? "#c0392b"
                         : slot.v >= chart.warningAt ? "#e0a800"
                         : "#2e86de"

                    Behavior on height {
                        NumberAnimation {
                            duration: 250
                        }
                    }
                }
            }
        }
    }
}
