import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Tab 7: a countdown. The time logic is in the core library (wb::Countdown).
Item {
    id: root

    property var backend
    property var theme
    property var appState

    readonly property var presetMinutes: [25, 5, 15, 1]

    Connections {
        target: root.backend

        function onCountdownFinished() {
            appState.statusText = qsTr("Time is over")
        }
    }

    Frame {
        width: Math.min(parent.width - 48, 700)
        anchors.centerIn: parent
        padding: 32
        background: Rectangle {
            color: theme.card
            radius: 14
            border.color: theme.border
            border.width: 2
        }

        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 18

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: backend.countdownText
                font.pixelSize: 84
                font.bold: true
                color: theme.text
            }

            ProgressBar {
                Layout.fillWidth: true
                value: backend.countdownProgress
            }

            RowLayout {
                spacing: 20

                Label {
                    text: qsTr("Minutes")
                    color: theme.muted
                }

                Slider {
                    id: minutesSlider
                    Layout.fillWidth: true
                    from: 1
                    to: 60
                    stepSize: 1
                    value: 5
                    onMoved: backend.setCountdownMinutes(Math.round(value))
                }

                Label {
                    text: Math.round(minutesSlider.value) + " min"
                    color: theme.muted
                }

                ComboBox {
                    id: presetBox
                    model: [qsTr("Focus 25 min"), qsTr("Short break 5 min"), qsTr("Long break 15 min"), qsTr("Quick 1 min")]
                    currentIndex: -1
                    displayText: currentIndex < 0 ? qsTr("Preset") : currentText
                    onActivated: index => {
                        minutesSlider.value = root.presetMinutes[index]
                        backend.setCountdownMinutes(root.presetMinutes[index])
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 16

                Button {
                    id: runButton
                    text: checked ? qsTr("Stop") : qsTr("Start")
                    checkable: true
                    highlighted: checked
                    implicitWidth: 160
                    onToggled: checked ? backend.startCountdown() : backend.stopCountdown()
                }

                // Keeps the button in step with the real state (it also stops by itself at 0).
                Binding {
                    target: runButton
                    property: "checked"
                    value: backend.countdownRunning
                }

                Button {
                    text: qsTr("Reset")
                    implicitWidth: 160
                    onClicked: backend.resetCountdown()
                }
            }
        }
    }
}
