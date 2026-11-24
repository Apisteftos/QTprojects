import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "validation.js" as Validation     // a JavaScript file is imported with its path and a name

ApplicationWindow {
    id: root
    width: 460
    height: 460
    visible: true
    title: qsTr("JavaScript in QML")

    // Bindings call the JavaScript functions. When the text changes, the binding runs again.
    readonly property string nameMessage: Validation.nameError(nameField.text)
    readonly property string emailMessage: Validation.emailError(emailField.text)
    readonly property string ageMessage: Validation.ageError(ageField.text)
    readonly property string passwordMessage: Validation.passwordError(passwordField.text)

    // A JavaScript function written in QML: it can use the ids and the properties of this file
    function allValid() {
        return nameMessage === "" && emailMessage === "" && ageMessage === "" && passwordMessage === ""
    }

    property string result: ""

    // A QML signal handler for the start of the object: runs once, when the whole file is created
    Component.onCompleted: {
        nameField.forceActiveFocus()
        console.log("The form is ready, valid:", allValid())
    }

    // The label and an error line for each field: the error line is empty until the user types something
    component Field: ColumnLayout {
        property alias label: caption.text
        property alias input: holder.data
        property string message: ""
        property bool touched: false
        Layout.fillWidth: true
        spacing: 2
        Label { id: caption }
        Item {
            id: holder
            Layout.fillWidth: true
            implicitHeight: children.length > 0 ? children[0].implicitHeight : 0
        }
        Label {
            text: touched ? message : ""
            color: "#c0392b"
            font.pixelSize: 12
            Layout.preferredHeight: 16
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 6

        Label { text: qsTr("Sign up"); font.bold: true; font.pixelSize: 20 }

        Field {
            label: qsTr("Name")
            message: root.nameMessage
            touched: nameField.text.length > 0
            input: TextField { id: nameField; width: parent.width }
        }
        Field {
            label: qsTr("E-mail")
            message: root.emailMessage
            touched: emailField.text.length > 0
            input: TextField { id: emailField; width: parent.width }
        }
        Field {
            label: qsTr("Age")
            message: root.ageMessage
            touched: ageField.text.length > 0
            input: TextField { id: ageField; width: parent.width }
        }
        Field {
            label: qsTr("Password")
            message: root.passwordMessage
            touched: passwordField.text.length > 0
            input: TextField { id: passwordField; width: parent.width; echoMode: TextInput.Password }
        }

        Button {
            text: qsTr("Submit")
            enabled: root.allValid()
            Layout.alignment: Qt.AlignRight
            onClicked: root.result = qsTr("Welcome, %1! (%2, age %3)").arg(nameField.text.trim()).arg(emailField.text).arg(parseInt(ageField.text))
        }
        Label {
            text: root.result
            color: "#27ae60"
            font.bold: true
        }
        Item { Layout.fillHeight: true }
    }
}
