import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "logic.js" as Logic

// A small form: a name, a quantity, a total and an order button.
ColumnLayout {
    id: root

    property int unitPrice: 250                 // in cents
    property string status: ""
    readonly property bool valid: Logic.isValidName(nameField.text)

    signal ordered(string name, int quantity)

    spacing: 8

    TextField {
        id: nameField
        objectName: "nameField"
        placeholderText: qsTr("Name")
        Layout.fillWidth: true
    }
    Label {
        objectName: "hint"
        visible: nameField.text !== "" && !root.valid
        text: qsTr("2 to 20 characters")
        color: "#c0392b"
    }
    Stepper {
        id: stepper
        objectName: "stepper"
        from: 1
        to: 5
        value: 1
    }
    Label {
        objectName: "total"
        text: qsTr("Total: %1").arg(Logic.formatPrice(stepper.value * root.unitPrice))
    }
    Button {
        objectName: "orderButton"
        text: qsTr("Order")
        enabled: root.valid
        onClicked: {
            const name = nameField.text.trim()
            root.status = qsTr("Ordered %1 x for %2").arg(stepper.value).arg(name)
            root.ordered(name, stepper.value)
        }
    }
    Label {
        objectName: "status"
        text: root.status
    }
}
