import QtQuick
import QtQuick.Controls

Window {
    width: 440
    height: 480
    visible: true
    title: qsTr("States and Transitions")
    color: "#eef1f5"

    // === 1. An expanding panel: two states and a transition between them ===
    Rectangle {
        id: panel
        x: 20
        y: 20
        width: 400
        height: 48                    // the height of the default state (the state with no name)
        radius: 8
        color: "white"
        border.color: "#cbd5e1"
        clip: true                    // the body is cut off at the border while the panel is small

        // The header: a click switches the state
        Item {
            id: header
            width: parent.width
            height: 48
            Text {
                x: 16
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Details")
                font.bold: true
            }
            Text {
                id: arrow
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                text: ">"
                font.bold: true
            }
            MouseArea {
                anchors.fill: parent
                onClicked: panel.state = (panel.state === "expanded") ? "" : "expanded"
            }
        }

        Text {
            id: body
            x: 16
            y: header.height + 8
            width: parent.width - 32
            wrapMode: Text.WordWrap
            opacity: 0
            text: qsTr("The panel has two states. In the state \"expanded\" three properties are different: the height of the panel, the rotation of the arrow and the opacity of this text. The transition makes the change smooth.")
        }

        // A state is a named set of changes to properties. The default state "" has no changes.
        states: State {
            name: "expanded"
            PropertyChanges { target: panel; height: 200 }
            PropertyChanges { target: arrow; rotation: 90 }
            PropertyChanges { target: body; opacity: 1 }
        }

        // A transition says HOW to get from one state to the other. "reversible" also plays it backwards.
        transitions: Transition {
            from: ""
            to: "expanded"
            reversible: true
            // The animations of a Transition run at the same time (in parallel) by default
            NumberAnimation { target: panel; property: "height"; duration: 300; easing.type: Easing.OutCubic }
            NumberAnimation { target: arrow; property: "rotation"; duration: 300 }
            NumberAnimation { target: body; property: "opacity"; duration: 300 }
        }
    }

    // === 2. States chosen by a condition: "when" ===
    Slider {
        id: level
        x: 20
        y: 250
        width: 400
        from: 0
        to: 100
        value: 20
    }

    Rectangle {
        id: lamp
        x: 20
        y: 300
        width: 400
        height: 60
        radius: 8
        color: "#27ae60"              // the color of the default state

        Text {
            id: lampText
            anchors.centerIn: parent
            color: "white"
            font.bold: true
            text: qsTr("Level %1: low").arg(Math.round(level.value))
        }

        // The states are checked in the order in which they are written, and the first "when" that is true wins.
        // So the strictest condition (high) comes first.
        states: [
            State {
                name: "high"
                when: level.value >= 70
                PropertyChanges { target: lamp; color: "#c0392b" }
                PropertyChanges { target: lampText; text: qsTr("Level %1: high").arg(Math.round(level.value)) }
            },
            State {
                name: "medium"
                when: level.value >= 40
                PropertyChanges { target: lamp; color: "#e67e22" }
                PropertyChanges { target: lampText; text: qsTr("Level %1: medium").arg(Math.round(level.value)) }
            }
        ]

        // === 3. A Behavior: "whenever this property changes, animate it" - for any reason ===
        Behavior on color {
            ColorAnimation { duration: 250 }
        }
    }

    Text {
        x: 20
        y: 380
        text: qsTr("State of the lamp: \"%1\"").arg(lamp.state === "" ? "default" : lamp.state)
    }
}
