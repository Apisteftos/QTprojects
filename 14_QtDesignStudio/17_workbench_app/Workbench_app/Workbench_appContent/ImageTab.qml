import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

// Tab 3: show an image. Zoom, rotate, grayscale.
Item {
    id: root

    property var backend
    property var theme
    property var appState
    property string fileName: ""

    signal openRequested

    function load(url) {
        const path = backend.localPath(url)
        fileName = backend.fileName(path)
        picture.source = url
        rotationAngle = 0
        backend.addRecent(path)
        appState.statusText = fileName
    }

    property int rotationAngle: 0

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 14

            RowLayout {
                spacing: 14

                Button {
                    text: qsTr("Open…")
                    highlighted: true
                    onClicked: root.openRequested()
                }

                Label {
                    text: root.fileName === "" ? qsTr("No image open") : root.fileName
                    color: theme.text
                    font.bold: root.fileName !== ""
                }
            }

            Rectangle {
                id: frame
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 12
                clip: true
                color: darkSwitch.checked ? "#10201a" : theme.card
                border.color: theme.border
                border.width: 2

                Item {
                    id: stage
                    anchors.centerIn: parent
                    width: picture.width
                    height: picture.height
                    rotation: root.rotationAngle

                    Image {
                        id: picture
                        readonly property real ratio: sourceSize.height > 0 ? sourceSize.width / sourceSize.height : 1

                        // Fit: scale to the free space. Otherwise: use the zoom value.
                        width: fitBox.checked ? Math.min(frame.width - 24, (frame.height - 24) * ratio)
                                              : sourceSize.width * zoomSlider.value / 100
                        height: fitBox.checked ? width / ratio : sourceSize.height * zoomSlider.value / 100
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        visible: false
                    }

                    MultiEffect {
                        anchors.fill: picture
                        source: picture
                        saturation: grayButton.checked ? -1.0 : 0.0
                    }
                }

                Label {
                    anchors.centerIn: parent
                    text: qsTr("Open an image with File > Open Image…")
                    color: theme.muted
                    visible: picture.source == ""
                }
            }
        }

        Frame {
            Layout.preferredWidth: 280
            Layout.fillHeight: true
            background: Rectangle {
                color: theme.card
                radius: 12
                border.color: theme.border
                border.width: 2
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Label {
                    text: qsTr("Zoom")
                    font.bold: true
                    color: theme.text
                }

                RowLayout {
                    Slider {
                        id: zoomSlider
                        Layout.fillWidth: true
                        from: 10
                        to: 400
                        stepSize: 10
                        value: 100
                        enabled: !fitBox.checked
                    }

                    Label {
                        text: Math.round(zoomSlider.value) + " %"
                        color: theme.muted
                    }
                }

                CheckBox {
                    id: fitBox
                    text: qsTr("Fit to window")
                }

                Label {
                    text: qsTr("Rotate")
                    font.bold: true
                    color: theme.text
                }

                RowLayout {
                    Button {
                        text: qsTr("-90°")
                        onClicked: root.rotationAngle = (root.rotationAngle - 90) % 360
                    }

                    Button {
                        text: qsTr("+90°")
                        onClicked: root.rotationAngle = (root.rotationAngle + 90) % 360
                    }
                }

                Label {
                    text: qsTr("Effects")
                    font.bold: true
                    color: theme.text
                }

                Button {
                    id: grayButton
                    text: qsTr("Grayscale")
                    checkable: true
                    highlighted: checked
                }

                Switch {
                    id: darkSwitch
                    text: qsTr("Dark background")
                }

                Item {
                    Layout.fillHeight: true
                }

                Button {
                    text: qsTr("Reset view")
                    onClicked: {
                        zoomSlider.value = 100
                        fitBox.checked = false
                        grayButton.checked = false
                        root.rotationAngle = 0
                    }
                }
            }
        }
    }
}
