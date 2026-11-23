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
