import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Tab 5: mix a color with three sliders, or type a hex code.
Item {
    id: root

    property var backend
    property var theme
    property var appState

    readonly property string hex: backend.toHex(redSlider.value, greenSlider.value, blueSlider.value)
    readonly property string rgbText: backend.toRgbText(redSlider.value, greenSlider.value, blueSlider.value)

    function setColor(text) {
        const rgb = backend.parseHex(text)
        if (rgb.length !== 3)
            return false
        redSlider.value = rgb[0]
        greenSlider.value = rgb[1]
        blueSlider.value = rgb[2]
        return true
    }

    function refreshField() {
        hexField.text = root.hex
    }

    // One labelled slider for one color channel.
    component ChannelSlider: ColumnLayout {
        id: channel

        property var theme
        property string title
        property alias value: slider.value

        signal moved

        Layout.fillWidth: true
        spacing: 2

        RowLayout {
            Label {
                text: channel.title
                font.bold: true
                color: channel.theme.text
                Layout.fillWidth: true
            }

            Label {
                text: Math.round(slider.value)
                color: channel.theme.muted
            }
        }

        Slider {
            id: slider
            Layout.fillWidth: true
            from: 0
            to: 255
            stepSize: 1
            onMoved: channel.moved()
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        Frame {
            Layout.preferredWidth: 440
            Layout.fillHeight: true
            background: Rectangle {
                color: theme.card
                radius: 14
                border.color: theme.border
                border.width: 2
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 12

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 12
                    color: root.hex
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: rgbSwitch.checked ? root.rgbText : root.hex
                    font.pixelSize: 28
                    font.bold: true
                    color: theme.text
                }

                Switch {
                    id: rgbSwitch
                    text: qsTr("Show as RGB")
                }
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            background: Rectangle {
                color: theme.card
                radius: 14
                border.color: theme.border
                border.width: 2
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 12

                ChannelSlider {
                    theme: root.theme
                    id: redSlider
                    title: qsTr("Red")
                    value: 46
                    onMoved: root.refreshField()
                }

                ChannelSlider {
                    theme: root.theme
                    id: greenSlider
                    title: qsTr("Green")
                    value: 125
                    onMoved: root.refreshField()
                }

                ChannelSlider {
                    theme: root.theme
                    id: blueSlider
                    title: qsTr("Blue")
                    value: 90
                    onMoved: root.refreshField()
                }

                Label {
                    text: qsTr("Hex")
                    font.bold: true
                    color: theme.text
                }

                RowLayout {
                    spacing: 12

                    TextField {
                        id: hexField
                        text: root.hex
                        implicitWidth: 160
                        maximumLength: 7
                        // Red text while the typed code is not a color.
                        color: backend.parseHex(text).length === 3 ? theme.text : "#c0563b"
                        onEditingFinished: {
                            if (!root.setColor(text))
                                text = root.hex
                        }
                    }

                    Button {
                        text: qsTr("Copy")
                        onClicked: {
                            clipboard.text = root.hex
                            clipboard.selectAll()
                            clipboard.copy()
                            appState.statusText = qsTr("Copied %1").arg(root.hex)
                        }
                    }

                    TextEdit {
                        id: clipboard
                        visible: false
                    }
                }

                Label {
                    text: qsTr("Presets")
                    color: theme.muted
                }

                Row {
                    spacing: 10

                    Repeater {
                        model: ["#2e7d5a", "#d9a21b", "#3a7fb0", "#b05fa0", "#8a6b4f"]

                        delegate: Rectangle {
                            required property string modelData
                            width: 38
                            height: 30
                            radius: 6
                            color: modelData

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    root.setColor(parent.modelData)
                                    root.refreshField()
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
