import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ============================================================
// VIEW — declarative QML
//
// Key MVVM concept: no logic here.
// The View only:
//   - Binds to ViewModel properties  (viewModel.statusText)
//   - Calls ViewModel invokables     (viewModel.addTask(...))
//   - Renders what the ViewModel exposes
//
// Bindings are AUTOMATIC — when viewModel emits statsChanged(),
// all Text elements bound to viewModel.statusText update instantly
// with zero manual code.
// ============================================================
ApplicationWindow {
    id:      root
    width:   420
    height:  580
    visible: true
    title:   "Qt MVVM Demo — Task Manager"

    // ── Stats bar ─────────────────────────────────────────────
    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: 8

            Label {
                text: "Task Manager — MVVM"
                font.bold: true
                font.pixelSize: 16
                Layout.fillWidth: true
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 10

        // Status line — bound to ViewModel property
        // Updates automatically when statsChanged() fires
        Label {
            text: viewModel.statusText
            color: "#555"
            font.italic: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }

        // ── Task list ─────────────────────────────────────────
        ListView {
            id: taskList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            // Bound to ViewModel's TaskListModel
            // Roles: title, done, taskId (defined in roleNames())
            model: viewModel.tasks

            delegate: Rectangle {
                width:  taskList.width
                height: 48
                color:  index % 2 === 0 ? "#f8f8f8" : "#ffffff"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    // Checkbox — bound to 'done' role
                    CheckBox {
                        checked: done
                        onClicked: viewModel.toggleDone(taskId)
                    }

                    // Title — strikethrough when done
                    Label {
                        text: title
                        Layout.fillWidth: true
                        color:    done ? "#aaa" : "#222"
                        font.strikeout: done
                        font.pixelSize: 14
                    }

                    // Remove button
                    Button {
                        text: "✕"
                        flat: true
                        onClicked: viewModel.removeTask(taskId)
                    }
                }

                // Divider line
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#e0e0e0"
                }
            }
        }

        // ── Input row ─────────────────────────────────────────
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TextField {
                id: inputField
                placeholderText: "Enter task title..."
                Layout.fillWidth: true

                // Call ViewModel invokable on Enter
                Keys.onReturnPressed: {
                    viewModel.addTask(inputField.text)
                    inputField.clear()
                }
            }

            Button {
                text: "Add"
                onClicked: {
                    viewModel.addTask(inputField.text)
                    inputField.clear()
                }
            }
        }

        // ── Clear all button ──────────────────────────────────
        Button {
            text: "Clear All"
            Layout.fillWidth: true
            onClicked: viewModel.clearAll()
        }
    }
}
