import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

// Tab 6: a note with a title. Save opens a dialog, so you choose the folder and the file name.
Item {
    id: root

    property var backend
    property var theme
    property var appState
    property bool dirty: false
    property string savedAt: ""
    property string notePath: ""   // the file chosen in the dialog, empty until the first save

    // Opens the dialog. The file name is suggested from the title.
    function saveAs() {
        const folder = backend.notesFolder()
        saveDialog.currentFolder = backend.fileUrl(folder)
        saveDialog.currentFile = backend.fileUrl(folder + "/" + backend.suggestedNoteName(titleField.text))
        saveDialog.open()
    }

    function saveTo(path) {
        if (backend.saveTextFile(path, notesArea.text)) {
            notePath = path
            dirty = false
            savedAt = Qt.formatTime(new Date(), "hh:mm")
            appState.statusText = qsTr("Saved %1 to %2").arg(savedAt).arg(path)
        } else {
            appState.statusText = backend.lastError
        }
    }

    // The Save button: the first time it asks where, later it writes to the same file.
    function save() {
        if (notePath === "")
            saveAs()
        else
            saveTo(notePath)
    }

    FileDialog {
        id: saveDialog
        title: qsTr("Save note")
        fileMode: FileDialog.SaveFile
        defaultSuffix: "txt"
        nameFilters: [qsTr("Text files (*.txt)"), qsTr("Markdown (*.md)"), qsTr("All files (*)")]
        onAccepted: root.saveTo(backend.localPath(selectedFile))
    }

    // Autosave: three seconds after the last change, but only once a file was chosen.
    Timer {
        id: autosaveTimer
        interval: 3000
        running: appState.autosave && root.dirty && root.notePath !== ""
        onTriggered: root.saveTo(root.notePath)
    }

    Frame {
        anchors.fill: parent
        anchors.margins: 24
        background: Rectangle {
            color: theme.card
            radius: 14
            border.color: theme.border
            border.width: 2
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                text: qsTr("Title")
                color: theme.muted
            }

            TextField {
                id: titleField
                Layout.fillWidth: true
                text: qsTr("Garden plan")
                onTextEdited: root.dirty = true
            }

            Label {
                text: qsTr("Notes")
                color: theme.muted
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                TextArea {
                    id: notesArea
                    wrapMode: TextEdit.Wrap
                    selectByMouse: true
                    color: theme.text
                    placeholderText: qsTr("Write something…")
                    background: Rectangle {
                        color: "transparent"
                        radius: 8
                        border.color: theme.border
                        border.width: 1.5
                    }
                    onTextChanged: root.dirty = true
                }
            }

            RowLayout {
                spacing: 14

                CheckBox {
                    text: qsTr("Autosave")
                    checked: appState.autosave
                    onToggled: appState.autosave = checked
                }

                Label {
                    text: root.notePath === "" ? qsTr("Not saved yet")
                          : (root.dirty ? qsTr("Unsaved changes") : qsTr("Saved %1 · %2").arg(root.savedAt).arg(backend.fileName(root.notePath)))
                    color: theme.muted
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("%1 characters").arg(notesArea.length)
                    color: theme.muted
                }

                Button {
                    text: qsTr("Clear")
                    onClicked: notesArea.clear()
                }

                Button {
                    text: qsTr("Save as…")
                    onClicked: root.saveAs()
                }

                Button {
                    text: qsTr("Save")
                    highlighted: true
                    onClicked: root.save()
                }
            }
        }
    }
}
