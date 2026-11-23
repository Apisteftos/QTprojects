import QtQuick
import QtQuick.Timeline

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Rectangle {
        id: card
        width: 320
        height: 260
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Image {
            id: leaf
            x: 112
            y: 28
            width: 96
            height: 96
            source: "leaf.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: titleText
            x: 0
            y: 144
            width: 320
            text: "Timeline intro"
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter
            color: "#1d4d38"
        }

        Text {
            id: subtitleText
            x: 0
            y: 188
            width: 320
            text: "Keyframes in Qt Design Studio"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            color: "#5b6b63"
        }
    }

    Timeline {
        id: timeline
        startFrame: 0
        endFrame: 1000
        enabled: true

        animations: [
            TimelineAnimation {
                id: introAnimation
                running: true
                loops: 1
                duration: 2000
                from: 0
                to: 1000
            }
        ]

        KeyframeGroup {
            target: card
            property: "opacity"

            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 250
                value: 1
            }
        }

        KeyframeGroup {
            target: leaf
            property: "scale"

            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 500
                value: 1
                easing.type: Easing.OutBack
            }
        }

        KeyframeGroup {
            target: leaf
            property: "rotation"

            Keyframe {
                frame: 0
                value: -180
            }

            Keyframe {
                frame: 500
                value: 0
                easing.type: Easing.OutCubic
            }
        }

        KeyframeGroup {
            target: titleText
            property: "opacity"

            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 400
                value: 0
            }

            Keyframe {
                frame: 700
                value: 1
            }
        }

        KeyframeGroup {
            target: titleText
            property: "y"

            Keyframe {
                frame: 0
                value: 174
            }

            Keyframe {
                frame: 400
                value: 174
            }

            Keyframe {
                frame: 700
                value: 144
                easing.type: Easing.OutCubic
            }
        }

        KeyframeGroup {
            target: subtitleText
            property: "opacity"

            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 600
                value: 0
            }

            Keyframe {
                frame: 900
                value: 1
            }
        }
    }
}
