import QtQuick
import QtQuick.Timeline

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"
    state: "off"

    Rectangle {
        id: panel
        width: 400
        height: 360
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Rectangle {
            id: lamp
            width: 100
            height: 100
            color: "#b8c2bc"
            radius: 50
            x: 150
            y: 28
        }

        Text {
            id: statusText
            text: "Off"
            font.pixelSize: 28
            color: "#1d4d38"
            anchors.top: lamp.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: messageText
            text: "Click a button"
            font.pixelSize: 16
            color: "#5b6b63"
            anchors.top: statusText.bottom
            anchors.topMargin: 12
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: buttonRow
            spacing: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: toggleButton
                width: 108
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Toggle"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: toggleArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: popButton
                width: 108
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Pop"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: popArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: helloButton
                width: 108
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Hello"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: helloArea
                    anchors.fill: parent
                }
            }
        }
    }

    Connections {
        target: toggleArea
        function onClicked() {
            if (root.state === "on")
                root.state = "off"
            else
                root.state = "on"
        }
    }

    Connections {
        target: popArea
        function onClicked() { popAnimation.restart() }
    }

    Connections {
        target: helloArea
        function onClicked() { messageText.text = "Hello from a signal" }
    }

    Connections {
        target: helloArea
        function onClicked() { console.log("Hello button clicked") }
    }

    Timeline {
        id: popTimeline
        startFrame: 0
        endFrame: 1000
        currentFrame: 1000
        enabled: true

        animations: [
            TimelineAnimation {
                id: popAnimation
                running: false
                loops: 1
                duration: 600
                from: 0
                to: 1000
            }
        ]

        KeyframeGroup {
            target: lamp
            property: "scale"

            Keyframe {
                frame: 0
                value: 0.6
            }

            Keyframe {
                frame: 1000
                value: 1
                easing.type: Easing.OutBack
            }
        }
    }

    states: [
        State {
            name: "off"
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
        }
    ]
}
