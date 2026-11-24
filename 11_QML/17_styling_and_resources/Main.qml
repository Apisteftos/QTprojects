import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 460
    height: 680
    visible: true
    title: qsTr("Styling and resources")

    // The style itself (Material) comes from qtquickcontrols2.conf. These lines only
    // change the theme and the accent. A style ignores the attached properties of other styles.
    readonly property var accents: [Material.Teal, Material.Orange, Material.Purple]
    Material.theme: darkSwitch.checked ? Material.Dark : Material.Light
    Material.accent: accents[accentBox.currentIndex]

    // === 2. The own look: a singleton gets the same switch ===
    Binding { target: Theme; property: "dark"; value: darkSwitch.checked }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            anchors.margins: Theme.spacing
            spacing: Theme.spacing

            // --- Header with an image from the resources ---
            RowLayout {
                Layout.margins: Theme.spacing
                spacing: Theme.spacing
                Image {
                    id: logo
                    source: "images/logo.png"            // relative to this file, found in the module
                    sourceSize.width: 96                 // decode at most this size: saves memory for big files
                    sourceSize.height: 96
                    Layout.preferredWidth: 48
                    Layout.preferredHeight: 48
                    fillMode: Image.PreserveAspectFit
                }
                Label {
                    text: qsTr("Styles and resources")
                    font.pixelSize: Theme.fontLarge
                    font.bold: true
                    Layout.fillWidth: true
                }
            }

            // --- Standard controls: they follow the style ---
            GroupBox {
                title: qsTr("Controls")
                Layout.fillWidth: true
                Layout.leftMargin: Theme.spacing
                Layout.rightMargin: Theme.spacing

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8
                    Switch { id: darkSwitch; text: qsTr("Dark mode") }
                    RowLayout {
                        Label { text: qsTr("Accent") }
                        ComboBox { id: accentBox; model: ["Teal", "Orange", "Purple"] }
                    }
                    RowLayout {
                        Button { text: qsTr("Button") }
                        Button { text: qsTr("Flat"); flat: true }
                        Button { text: qsTr("Highlighted"); highlighted: true }
                    }
                    CheckBox { text: qsTr("Check box"); checked: true }
                    Slider { Layout.fillWidth: true; value: 0.4 }
                    TextField { Layout.fillWidth: true; placeholderText: qsTr("Text field") }
                }
            }

            // --- The own look: the values come from Theme, not from the style ---
            Rectangle {
                id: card
                Layout.fillWidth: true
                Layout.leftMargin: Theme.spacing
                Layout.rightMargin: Theme.spacing
                implicitHeight: cardRow.implicitHeight + 2 * Theme.spacing
                radius: Theme.radius
                color: Theme.card

                RowLayout {
                    id: cardRow
                    anchors.fill: parent
                    anchors.margins: Theme.spacing
                    Label {
                        text: qsTr("A card with the colours of Theme")
                        color: Theme.text
                        Layout.fillWidth: true
                    }
                    // A control with its own background: this works in every style
                    Button {
                        id: ownButton
                        text: qsTr("Own button")
                        contentItem: Label {
                            text: ownButton.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                        }
                        background: Rectangle {
                            radius: Theme.radius
                            color: ownButton.down ? Qt.darker(Theme.brand, 1.3) : Theme.brand
                        }
                    }
                }
            }

            // --- Images: status, an error, an icon on a button ---
            GroupBox {
                title: qsTr("Images")
                Layout.fillWidth: true
                Layout.leftMargin: Theme.spacing
                Layout.rightMargin: Theme.spacing

                RowLayout {
                    anchors.fill: parent
                    spacing: Theme.spacing
                    Image {
                        id: missing
                        source: "images/missing.png"      // the file does not exist
                        Layout.preferredWidth: 48
                        Layout.preferredHeight: 48
                    }
                    Label {
                        id: missingLabel
                        text: missing.status === Image.Ready ? qsTr("Image ready")
                            : missing.status === Image.Error ? qsTr("Image error")
                            : qsTr("Image loading")
                        Layout.fillWidth: true
                    }
                    Button {
                        text: qsTr("With icon")
                        icon.source: "images/logo.png"
                        icon.color: "transparent"         // keep the colours of the file
                    }
                }
            }

            // --- Fonts ---
            GroupBox {
                title: qsTr("Fonts")
                Layout.fillWidth: true
                Layout.leftMargin: Theme.spacing
                Layout.rightMargin: Theme.spacing

                ColumnLayout {
                    anchors.fill: parent
                    ComboBox {
                        id: fontBox
                        model: ["Sans Serif", "Serif", "Monospace"]
                    }
                    Label {
                        id: sample
                        text: qsTr("Sample text")
                        font.family: fontBox.currentText
                        font.pixelSize: Theme.fontLarge
                        font.italic: true
                    }
                    Label {
                        text: qsTr("%1 font families on this computer").arg(Qt.fontFamilies().length)
                        color: "#777"
                    }
                }
            }
        }
    }
}
