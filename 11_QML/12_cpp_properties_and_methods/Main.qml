import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 420
    height: 400
    visible: true
    title: qsTr("C++ Properties and Methods")
    color: "#eef1f5"

    // === Counter is a C++ class (counter.h). QML creates an object of it like any other item ===
    Counter {
        id: counter
        step: 2           // sets the C++ property through setStep()
        limit: 10
    }

    property string message: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 12

        // === Reading a property: a binding. It updates because valueChanged() is emitted in C++ ===
        Text {
            text: counter.value
            font.pixelSize: 56
            Layout.alignment: Qt.AlignHCenter
        }
        ProgressBar {
            Layout.fillWidth: true
            from: 0
            to: counter.limit
            value: counter.value
        }
        // === A calculated property ("summary") can be used in a binding: it has a NOTIFY signal ===
        Text {
            text: counter.summary
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8
            Button {
                text: qsTr("+%1").arg(counter.step)
                onClicked: {
                    // the return value of a C++ method is a normal JavaScript value
                    if (!counter.increment())
                        message = qsTr("The limit %1 is reached").arg(counter.limit)
                    else
                        message = ""
                }
            }
            Button { text: qsTr("-%1").arg(counter.step); onClicked: counter.decrement() }
            Button { text: qsTr("Reset"); onClicked: { counter.reset(); message = "" } }
        }

        // === Calling a method: Q_INVOKABLE functions are called like JavaScript functions, in a handler ===
        Button {
            text: qsTr("Describe")
            Layout.alignment: Qt.AlignHCenter
            onClicked: described.text = counter.describe(qsTr("coins"))   // the answer is the value of this moment
        }
        Text {
            id: described
            Layout.alignment: Qt.AlignHCenter
            color: "#2e86de"
        }

        // === Writing a property from QML: it goes to setStep() / setLimit() ===
        RowLayout {
            Label { text: qsTr("Step") }
            SpinBox {
                from: 1
                to: 5
                value: counter.step
                onValueModified: counter.step = value
            }
            Item { Layout.fillWidth: true }
            Label { text: qsTr("Limit") }
            SpinBox {
                from: 1
                to: 50
                value: counter.limit
                onValueModified: counter.limit = value
            }
        }

        Text {
            text: message
            color: "#c0392b"
            Layout.alignment: Qt.AlignHCenter
        }
        Text {
            text: counter.atLimit ? qsTr("At the limit") : qsTr("Below the limit")
            color: counter.atLimit ? "#c0392b" : "#27ae60"
            Layout.alignment: Qt.AlignHCenter
        }
        Item { Layout.fillHeight: true }
    }
}
