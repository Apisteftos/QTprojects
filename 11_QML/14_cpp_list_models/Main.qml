import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 440
    height: 520
    visible: true
    title: qsTr("C++ List Models")
    color: "#eef1f5"

    // The model is a C++ object, created by QML like any other item
    TodoModel { id: todos }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Label {
            text: qsTr("%1 of %2 tasks left").arg(todos.remaining).arg(todos.count)
            font.bold: true
            font.pixelSize: 18
        }

        RowLayout {
            TextField {
                id: input
                Layout.fillWidth: true
                placeholderText: qsTr("New task")
                onAccepted: add.clicked()          // the Enter key
            }
            Button {
                id: add
                text: qsTr("Add")
                enabled: input.text.trim().length > 0
                onClicked: {
                    todos.add(input.text)          // a Q_INVOKABLE method of the model
                    input.clear()
                }
            }
        }

        ListView {
            id: list
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 4
            model: todos

            // The delegate gets the roles of the C++ model by the names from roleNames()
            delegate: Rectangle {
                id: row
                required property int index
                required property string title
                required property bool done
                width: ListView.view.width
                height: 44
                radius: 6
                color: "white"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    anchors.rightMargin: 8
                    CheckBox {
                        checked: row.done
                        // The user's change goes back to the model by a method (which calls setData())
                        onToggled: todos.setDone(row.index, checked)
                    }
                    Label {
                        Layout.fillWidth: true
                        text: row.title
                        font.strikeout: row.done
                        color: row.done ? "#999" : "#1f2d3d"
                    }
                    ToolButton {
                        text: "×"
                        onClicked: todos.remove(row.index)
                    }
                }
            }
        }

        Button {
            text: qsTr("Clear finished tasks")
            Layout.alignment: Qt.AlignRight
            onClicked: todos.clearDone()
        }
    }
}
