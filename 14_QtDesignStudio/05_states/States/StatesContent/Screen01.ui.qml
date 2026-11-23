import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"
    state: "off"

    Rectangle {
        id: panel
        width: 360
        height: 340
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Rectangle {
            id: lamp
            width: 120
            height: 120
            color: "#b8c2bc"
            radius: 60
            anchors.top: parent.top
            anchors.topMargin: 36
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: statusText
            text: "Off"
            font.pixelSize: 28
            color: "#1d4d38"
            anchors.top: lamp.bottom
            anchors.topMargin: 24
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: buttonRow
            spacing: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: offButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Off"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: offArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: dimButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Dim"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: dimArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: onButton
                width: 96
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "On"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: onArea
                    anchors.fill: parent
                }
            }
        }
    }

    Connections {
        target: offArea
        function onClicked() { root.state = "off" }
    }

    Connections {
        target: dimArea
        function onClicked() { root.state = "dim" }
    }

    Connections {
        target: onArea
        function onClicked() { root.state = "on" }
    }

    states: [
        State {
            name: "off"
        },
        State {
            name: "dim"

            PropertyChanges {
                target: lamp
                color: "#ffd54a"
                opacity: 0.45
            }

            PropertyChanges {
                target: statusText
                text: "Dim"
            }
        },
        State {
            name: "on"

            PropertyChanges {
                target: lamp
                color: "#ffd54a"
            }

            PropertyChanges {
                target: statusText
                text: "On"
            }

            PropertyChanges {
                target: panel
                border.color: "#ffd54a"
            }
        }
    ]
}
