import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    // ---- The interface of this screen (the developer uses only these) ----
    property int level: 40
    property string statusText: "Idle"
    property alias startArea: startArea
    property alias stopArea: stopArea

    Rectangle {
        id: panel
        width: 420
        height: 300
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Text {
            id: titleText
            text: "Level"
            font.pixelSize: 24
            color: "#1d4d38"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 28
        }

        Text {
            id: levelText
            text: root.level + " %"
            font.pixelSize: 24
            color: "#1d4d38"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 28
        }

        Rectangle {
            id: track
            height: 24
            color: "#dfe8e3"
            radius: 12
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: titleText.bottom
            anchors.leftMargin: 28
            anchors.rightMargin: 28
            anchors.topMargin: 20

            Rectangle {
                id: fill
                width: parent.width * root.level / 100
                height: parent.height
                color: "#2e7d5a"
                radius: 12
            }
        }

        Text {
            id: statusLabel
            text: root.statusText
            font.pixelSize: 20
            color: "#4a6b5b"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: track.bottom
            anchors.topMargin: 28
        }

        Row {
            id: buttonRow
            spacing: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: startButton
                width: 110
                height: 44
                color: "#2e7d5a"
                radius: 8

                Text {
                    text: "Start"
                    font.pixelSize: 18
                    color: "#ffffff"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: startArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: stopButton
                width: 110
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Stop"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: stopArea
                    anchors.fill: parent
                }
            }
        }
    }
}
