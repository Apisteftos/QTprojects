import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// Tab 1: read a text file, search in it, change how it is shown.
Item {
    id: root

    property var backend
    property var theme
    property var appState
    property string fileName: ""

    signal openRequested

    // Called by Main.qml (File > Open Text, Recent menu).
    function load(url) {
        const text = backend.readTextFile(url)
        if (backend.lastError !== "") {
            appState.statusText = backend.lastError
            return
        }
        const path = backend.localPath(url)
        fileName = backend.fileName(path)
        area.text = text
        area.cursorPosition = 0
        backend.addRecent(path)
        appState.statusText = fileName + "  ·  " + backend.countLines(text) + " lines"
    }

    // Selects the next match. Starts again at the top after the last match.
    function findNext() {
        if (searchField.text === "")
            return
        let pos = backend.findNext(area.text, searchField.text, area.selectionEnd, false)
        if (pos < 0)
            pos = backend.findNext(area.text, searchField.text, 0, false)
        if (pos >= 0)
            area.select(pos, pos + searchField.text.length)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 14

        RowLayout {
            spacing: 14

            Button {
                text: qsTr("Open…")
                highlighted: true
                onClicked: root.openRequested()
            }

            Label {
                text: root.fileName === "" ? qsTr("No file open") : root.fileName
                color: theme.text
                font.bold: root.fileName !== ""
                Layout.fillWidth: true
            }

            Label {
                text: searchField.text === "" ? ""
                      : qsTr("%1 matches").arg(backend.countMatches(area.text, searchField.text, false))
                color: theme.muted
            }

            TextField {
                id: searchField
                placeholderText: qsTr("Search…")
                implicitWidth: 200
                onAccepted: root.findNext()
            }

            Button {
                text: qsTr("Find next")
                enabled: searchField.text !== ""
                onClicked: root.findNext()
            }
        }

        RowLayout {
            spacing: 20

            Label {
                text: qsTr("Font size")
                color: theme.muted
            }

            Slider {
                id: sizeSlider
                from: 10
                to: 28
                stepSize: 1
                value: 16
            }

            Label {
                text: Math.round(sizeSlider.value) + " pt"
                color: theme.muted
            }

            CheckBox {
                text: qsTr("Word wrap")
                checked: appState.wordWrap
                onToggled: appState.wordWrap = checked
            }

            Switch {
                id: monoSwitch
                text: qsTr("Monospace")
            }

            Switch {
                id: readOnlySwitch
                text: qsTr("Read only")
                checked: false
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: theme.card
            radius: 12
            border.color: theme.border
            border.width: 2

            ScrollView {
                anchors.fill: parent
                anchors.margins: 6
                clip: true

                TextArea {
                    id: area
                    readOnly: readOnlySwitch.checked
                    selectByMouse: true
                    color: theme.text
                    selectionColor: "#f3d98b"
                    selectedTextColor: "#1d4d38"
                    wrapMode: appState.wordWrap ? TextEdit.Wrap : TextEdit.NoWrap
                    font.pointSize: sizeSlider.value
                    font.family: monoSwitch.checked ? "monospace" : Qt.application.font.family
                    placeholderText: qsTr("Open a text file with File > Open Text, or type here (typed text is not saved, use the Notes tab)")
                    background: null
                }
            }
        }
    }
}
