import QtQuick
import QtQuick.Controls

// One number with a title. It counts to a new value, and its colours follow the level.
Rectangle {
    id: card

    property string title: ""
    property real value: 0
    property string unit: ""
    property int decimals: 0
    property string level: "ok"

    // "shown" is the number on the screen. It follows "value" with an animation.
    property real shown: value
    Behavior on shown {
        NumberAnimation {
            duration: 400
            easing.type: Easing.OutCubic
        }
    }

    implicitWidth: 130
    implicitHeight: 90
    radius: 12
    color: "white"
    border.width: 2
    border.color: "#d5dbe3"

    // === States: the level decides the colours. A Transition makes the change smooth ===
    states: [
        State {
            name: "warning"
            when: card.level === "warning"
            PropertyChanges {
                target: card
                color: "#fff4d6"
                border.color: "#e0a800"
            }
        },
        State {
            name: "critical"
            when: card.level === "critical"
            PropertyChanges {
                target: card
                color: "#fde2df"
                border.color: "#c0392b"
            }
        }
    ]
    transitions: Transition {
        ColorAnimation {
            duration: 300
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 4
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: card.title
            color: "#5b6573"
        }
        Label {
            objectName: "number"
            anchors.horizontalCenter: parent.horizontalCenter
            text: card.shown.toFixed(card.decimals) + " " + card.unit
            font.pixelSize: 24
            font.bold: true
            color: "#1d232b"
        }
    }
}
