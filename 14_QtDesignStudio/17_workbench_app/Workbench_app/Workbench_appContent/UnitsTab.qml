import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Tab 4: unit converter. The calculation is done by the core library.
Item {
    id: root

    property var backend
    property var theme
    property var appState

    // A new category has new units: start with the first two (a metric and an
    // imperial unit). This runs after the unit lists have been replaced,
    // because a ComboBox goes back to its first entry when its model changes.
    function resetUnits() {
        fromBox.currentIndex = 0
        toBox.currentIndex = 1
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
                text: qsTr("Unit converter")
                font.pixelSize: 22
                font.bold: true
                color: theme.text
            }

            GridLayout {
                columns: 3
                columnSpacing: 20
                rowSpacing: 6
                Layout.fillWidth: true

                Label {
                    text: qsTr("Category")
                    color: theme.muted
                }

                Label {
                    text: qsTr("From")
                    color: theme.muted
                }

                Label {
                    text: qsTr("To")
                    color: theme.muted
                }

                ComboBox {
                    id: categoryBox
                    Layout.fillWidth: true
                    model: backend.categories()
                    onCurrentTextChanged: Qt.callLater(root.resetUnits)
                }

                ComboBox {
                    id: fromBox
                    Layout.fillWidth: true
                    model: backend.unitsOf(categoryBox.currentText)
                }

                ComboBox {
                    id: toBox
                    Layout.fillWidth: true
                    model: backend.unitsOf(categoryBox.currentText)
                    currentIndex: 1
                }

                Label {
                    text: qsTr("Value")
                    color: theme.muted
                }

                Item {
                    Layout.columnSpan: 2
                }

                TextField {
                    id: valueField
                    Layout.fillWidth: true
                    text: "12.5"
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    validator: DoubleValidator {
                        locale: "C"
                        notation: DoubleValidator.StandardNotation
                    }
                }

                Button {
                    text: qsTr("Swap")
                    onClicked: {
                        const from = fromBox.currentIndex
                        fromBox.currentIndex = toBox.currentIndex
                        toBox.currentIndex = from
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 110
                radius: 12
                color: theme.soft

                Column {
                    anchors.centerIn: parent
                    spacing: 6

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Result")
                        color: theme.muted
                    }

                    Label {
                        id: resultLabel
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 38
                        font.bold: true
                        color: theme.text
                        text: {
                            const value = backend.convert(categoryBox.currentText, fromBox.currentText,
                                                          toBox.currentText, valueField.text)
                            return value === "-" ? "-" : value + " " + toBox.currentText
                        }
                    }
                }
            }
        }
    }
}
