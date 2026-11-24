import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// === THE VIEW ===
// Only the look. No rule is written here: the limits, the price and the texts come from the ViewModel.
ApplicationWindow {
    id: root

    // Given by main.cpp (setInitialProperties). "required" = the file does not work without it.
    required property BookingViewModel viewModel

    width: 460
    height: 600
    visible: true
    title: qsTr("Room bookings")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 10

        // --- The form ---
        GroupBox {
            title: qsTr("New booking")
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                TextField {
                    id: guestField
                    objectName: "guestField"
                    Layout.fillWidth: true
                    placeholderText: qsTr("Guest name")
                    text: root.viewModel.guest                       // ViewModel -> View
                    onTextEdited: root.viewModel.guest = text        // View -> ViewModel (only when the USER types)
                }
                Label {
                    objectName: "errorLabel"
                    text: root.viewModel.error
                    visible: text !== ""
                    color: "#c0392b"
                }
                RowLayout {
                    Label { text: qsTr("Nights") }
                    SpinBox {
                        objectName: "nightsBox"
                        from: 0
                        to: 20
                        value: root.viewModel.nights
                        onValueModified: root.viewModel.nights = value
                    }
                    Item { Layout.fillWidth: true }
                    Switch {
                        objectName: "breakfastSwitch"
                        text: qsTr("Breakfast")
                        checked: root.viewModel.breakfast
                        onToggled: root.viewModel.breakfast = checked
                    }
                }
                RowLayout {
                    Label {
                        objectName: "totalLabel"
                        text: qsTr("Total: %1").arg(root.viewModel.totalText)
                        font.bold: true
                        Layout.fillWidth: true
                    }
                    Button {
                        objectName: "saveButton"
                        text: qsTr("Save")
                        enabled: root.viewModel.canSave
                        onClicked: root.viewModel.save()
                    }
                }
            }
        }

        Label {
            objectName: "messageLabel"
            text: root.viewModel.message
            color: "#2e7d32"
            Layout.fillWidth: true
        }

        // --- The saved bookings ---
        RowLayout {
            Label {
                objectName: "countLabel"
                text: qsTr("%1 bookings").arg(root.viewModel.count)
                font.bold: true
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Remove all")
                enabled: root.viewModel.count > 0
                onClicked: root.viewModel.clearAll()
            }
        }
        ListView {
            id: list
            objectName: "bookingList"
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: root.viewModel.bookings
            spacing: 4

            delegate: ItemDelegate {
                id: row
                required property int index
                required property string guest
                required property int nights
                required property bool breakfast
                required property int price

                width: ListView.view.width
                text: qsTr("%1: %2 nights%3, %4").arg(guest).arg(nights).arg(breakfast ? qsTr(" with breakfast") : "").arg(root.viewModel.priceText(price))

                Button {
                    objectName: "removeButton"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: "×"
                    flat: true
                    onClicked: root.viewModel.remove(row.index)
                }
            }

            Label {
                anchors.centerIn: parent
                visible: list.count === 0
                text: qsTr("No bookings yet")
                color: "#777"
            }
        }
    }
}
