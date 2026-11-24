import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ============================================================
// AboutPage — demonstrates QML animations
//
// Shows:
//   - SequentialAnimation
//   - NumberAnimation on opacity and scale
//   - RotationAnimation
//   - ParallelAnimation
// ============================================================
Item {
    id: root

    // Trigger entrance animation when page becomes visible
    onVisibleChanged: {
        if (visible) entranceAnim.start();
    }

    SequentialAnimation {
        id: entranceAnim

        // Reset
        PropertyAction  { target: content; property: "opacity"; value: 0 }
        PropertyAction  { target: content; property: "scale";   value: 0.8 }

        // Fade + scale in
        ParallelAnimation {
            NumberAnimation {
                target:   content
                property: "opacity"
                to:       1.0
                duration: 400
                easing.type: Easing.OutCubic
            }
            NumberAnimation {
                target:   content
                property: "scale"
                to:       1.0
                duration: 400
                easing.type: Easing.OutBack
            }
        }
    }

    Item {
        id: content
        anchors.fill: parent
        opacity: 0

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20
            width: Math.min(parent.width * 0.8, 500)

            // Spinning logo
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width:  80
                height: 80
                radius: 40
                color:  "#1565c0"

                Text {
                    anchors.centerIn: parent
                    text: "Qt"
                    color: "white"
                    font.pixelSize: 28
                    font.bold: true
                }

                RotationAnimation on rotation {
                    from:     0
                    to:       360
                    duration: 3000
                    loops:    Animation.Infinite
                    running:  root.visible
                }
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Qt QML Dashboard Demo"
                font.pixelSize: 24
                font.bold: true
                color: "#1565c0"
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Qt6 C++ Tutorial Repository"
                font.pixelSize: 14
                color: "#666"
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#e0e0e0"
            }

            // Feature list
            Repeater {
                model: [
                    "✅ MVVM pattern — Q_PROPERTY bindings",
                    "✅ TabBar + SwipeView navigation",
                    "✅ Canvas API — custom charts + gauges",
                    "✅ Behavior animations — smooth transitions",
                    "✅ SequentialAnimation + ParallelAnimation",
                    "✅ C++ ViewModel feeding live QML data",
                    "✅ Alert system with animated overlay",
                    "✅ Reusable QML components"
                ]

                Label {
                    Layout.fillWidth: true
                    text: modelData
                    font.pixelSize: 13
                    color: "#333"

                    // Staggered entrance animation
                    opacity: 0
                    Component.onCompleted: {
                        Qt.callLater(function() {
                            fadeIn.start();
                        });
                    }

                    NumberAnimation on opacity {
                        id: fadeIn
                        from:     0
                        to:       1
                        duration: 300
                        // Stagger based on index
                        running:  false
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#e0e0e0"
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "github.com/Apisteftos/QTprojects"
                font.pixelSize: 12
                color: "#1565c0"
                font.underline: true
            }
        }
    }
}
