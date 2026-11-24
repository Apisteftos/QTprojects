import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// === THE VIEW ===
// It READS the model directly (the list, the numbers) and REPORTS every input to the controller.
// It changes no data itself.
ApplicationWindow {
    id: root

    required property ScoreModel scores              // the Model: to show
    required property ScoreController controller     // the Controller: to tell what the user did

    width: 420
    height: 560
    visible: true
    title: qsTr("Score board")

    // The controller says "rejected": the view decides how to show it (here: a red line for 3 seconds)
    Connections {
        target: root.controller
        function onRejected(reason) {
            errorLabel.text = reason
            errorTimer.restart()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 10

        // --- Input: a name ---
        RowLayout {
            Layout.fillWidth: true
            TextField {
                id: nameField
                objectName: "nameField"
                Layout.fillWidth: true
                placeholderText: qsTr("Name of a new player")
                onAccepted: addButton.clicked()                // the Enter key
            }
            Button {
                id: addButton
                objectName: "addButton"
                text: qsTr("Add")
                onClicked: {
                    if (root.controller.addPlayer(nameField.text))
                        nameField.clear()                      // only when the controller accepted the name
                }
            }
        }

        Label {
            id: errorLabel
            objectName: "errorLabel"
            color: "#c0392b"
            visible: text !== ""
            Timer {
                id: errorTimer
                interval: 3000
                onTriggered: errorLabel.text = ""
            }
        }

        // --- Output: the model, shown directly ---
        ListView {
            id: list
            objectName: "playerList"
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 6
            model: root.scores

            delegate: Rectangle {
                id: row
                required property int index
                required property string name
                required property int points
                required property bool leading

                width: ListView.view.width
                height: 56
                radius: 10
                color: leading ? "#fff4d6" : "#eef2f7"
                border.color: leading ? "#e0a800" : "#d5dbe3"
                border.width: 2

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Label {
                        text: (row.leading ? "★ " : "") + row.name
                        font.bold: row.leading
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                    Button {
                        objectName: "minusButton"
                        text: "−"
                        onClicked: root.controller.takePoint(row.index)
                    }
                    Label {
                        objectName: "pointsLabel"
                        text: row.points
                        font.pixelSize: 22
                        Layout.minimumWidth: 36
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Button {
                        objectName: "plusButton"
                        text: "+"
                        onClicked: root.controller.givePoint(row.index)
                    }
                    Button {
                        objectName: "removeButton"
                        text: "×"
                        flat: true
                        onClicked: root.controller.removePlayer(row.index)
                    }
                }
            }

            Label {
                anchors.centerIn: parent
                visible: list.count === 0
                text: qsTr("No players yet")
                color: "#777"
            }
        }

        // --- Output: numbers of the model; Input: one command ---
        RowLayout {
            Layout.fillWidth: true
            Label {
                objectName: "summaryLabel"
                text: qsTr("%1 players, %2 points").arg(root.scores.count).arg(root.scores.total)
                font.bold: true
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("New round")
                enabled: root.scores.total > 0
                onClicked: root.controller.newRound()
            }
        }
    }
}
