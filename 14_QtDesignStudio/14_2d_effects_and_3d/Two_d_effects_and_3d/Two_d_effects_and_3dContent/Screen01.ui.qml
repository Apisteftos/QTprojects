import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Timeline
import QtQuick3D

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    // ---------- 2D side: effects on an image ----------
    Rectangle {
        id: effectCard
        width: 360
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 24

        Text {
            id: effectTitle
            text: "2D effects"
            font.pixelSize: 24
            color: "#1d4d38"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 20
        }

        Image {
            id: leaf
            width: 128
            height: 128
            source: "leaf.png"
            fillMode: Image.PreserveAspectFit
            visible: false
            x: 116
            y: 80
        }

        MultiEffect {
            id: leafEffect
            source: leaf
            anchors.fill: leaf
            blurEnabled: true
            blur: blurSlider.value
            blurMax: 32
            shadowEnabled: shadowSwitch.checked
            shadowColor: "#000000"
            shadowOpacity: 0.5
            shadowHorizontalOffset: 6
            shadowVerticalOffset: 10
            shadowBlur: 0.6
            colorization: colorSlider.value
            colorizationColor: "#d9a21b"
        }

        Column {
            id: controlsColumn
            spacing: 12
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 24

            Text {
                id: blurLabel
                text: "Blur: " + Math.round(blurSlider.value * 100) + " %"
                font.pixelSize: 16
                color: "#1d4d38"
            }

            Slider {
                id: blurSlider
                width: parent.width
                from: 0
                to: 1
                value: 0
            }

            Text {
                id: colorLabel
                text: "Color overlay: " + Math.round(colorSlider.value * 100) + " %"
                font.pixelSize: 16
                color: "#1d4d38"
            }

            Slider {
                id: colorSlider
                width: parent.width
                from: 0
                to: 1
                value: 0
            }

            Switch {
                id: shadowSwitch
                text: "Shadow"
                checked: true
            }
        }
    }

    // ---------- 3D side: a small scene ----------
    Rectangle {
        id: sceneCard
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.left: effectCard.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 24
        anchors.rightMargin: 24
        anchors.topMargin: 24
        anchors.bottomMargin: 24

        Text {
            id: sceneTitle
            text: "3D scene"
            font.pixelSize: 24
            color: "#1d4d38"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 20
        }

        View3D {
            id: view3D
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: sceneTitle.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 16

            camera: sceneCamera

            environment: SceneEnvironment {
                clearColor: "#dfe8e3"
                backgroundMode: SceneEnvironment.Color
            }

            PerspectiveCamera {
                id: sceneCamera
                position: Qt.vector3d(0, 200, 300)
                eulerRotation.x: -30
            }

            DirectionalLight {
                id: sceneLight
                eulerRotation.x: -35
                eulerRotation.y: -60
            }

            Model {
                id: cube
                source: "#Cube"
                position: Qt.vector3d(-70, 50, 0)
                materials: DefaultMaterial {
                    diffuseColor: "#2e7d5a"
                }
            }

            Model {
                id: sphere
                source: "#Sphere"
                position: Qt.vector3d(80, 50, 0)
                materials: DefaultMaterial {
                    diffuseColor: "#d9a21b"
                }
            }

            Model {
                id: floorPlane
                source: "#Rectangle"
                eulerRotation.x: -90
                scale: Qt.vector3d(6, 6, 1)
                materials: DefaultMaterial {
                    diffuseColor: "#c9d6cf"
                }
            }
        }
    }

    // ---------- Timeline: the cube turns ----------
    Timeline {
        id: turnTimeline
        startFrame: 0
        endFrame: 1000
        enabled: true

        animations: [
            TimelineAnimation {
                id: turnAnimation
                running: true
                loops: Animation.Infinite
                duration: 6000
                from: 0
                to: 1000
            }
        ]

        KeyframeGroup {
            target: cube
            property: "eulerRotation.y"

            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 1000
                value: 360
            }
        }
    }
}
