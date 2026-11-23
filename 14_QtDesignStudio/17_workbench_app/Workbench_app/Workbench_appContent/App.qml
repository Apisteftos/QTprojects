import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Workbench_backend

ApplicationWindow {
    id: window

    // Large at launch: 80 % of the free screen, but not smaller than 1100 x 700 and not larger than 1600 x 1000.
    width: Math.max(1100, Math.min(1600, Screen.desktopAvailableWidth * 0.8))
    height: Math.max(700, Math.min(1000, Screen.desktopAvailableHeight * 0.8))
    minimumWidth: 800
    minimumHeight: 520
    visible: true
    title: "Workbench"
    color: appTheme.window

    palette.window: appTheme.window
    palette.windowText: appTheme.text
    palette.base: appTheme.card
    palette.text: appTheme.text
    palette.button: appTheme.soft
    palette.buttonText: appTheme.text
    palette.highlight: appTheme.accent
    palette.highlightedText: "#ffffff"
    palette.mid: appTheme.border
    palette.placeholderText: appTheme.muted

    // Colors and shared settings. The tabs get them as properties.
    Theme {
        id: appTheme
    }

    AppState {
        id: appStore
    }

    WorkbenchBackend {
        id: appBackend
    }

    // Opens a file in the tab that fits its type.
    function openPath(path) {
        const kind = appBackend.fileKind(path)
        const url = appBackend.fileUrl(path)
        if (kind === "text") {
            tabBar.currentIndex = 0
            textTab.load(url)
        } else if (kind === "video") {
            tabBar.currentIndex = 1
            videoTab.load(url)
        } else if (kind === "image") {
            tabBar.currentIndex = 2
            imageTab.load(url)
        } else {
            appStore.statusText = "Unknown file type: " + appBackend.fileName(path)
        }
    }

    // ---------- Actions (used by the menus and by the shortcuts) ----------
    Action {
        id: openTextAction
        text: qsTr("Open Text…")
        shortcut: "Ctrl+O"
        onTriggered: textDialog.open()
    }

    Action {
        id: openVideoAction
        text: qsTr("Open Video…")
        shortcut: "Ctrl+Shift+O"
        onTriggered: videoDialog.open()
    }

    Action {
        id: openImageAction
        text: qsTr("Open Image…")
        shortcut: "Ctrl+I"
        onTriggered: imageDialog.open()
    }

    Action {
        id: quitAction
        text: qsTr("Quit")
        shortcut: "Ctrl+Q"
        onTriggered: Qt.quit()
    }

    Action {
        id: fullScreenAction
        text: qsTr("Full Screen")
        shortcut: "F11"
        onTriggered: window.visibility = window.visibility === Window.FullScreen ? Window.Windowed
                                                                                   : Window.FullScreen
    }

    Action {
        id: unitsAction
        text: qsTr("Unit Converter")
        shortcut: "Ctrl+U"
        onTriggered: tabBar.currentIndex = 3
    }

    Action {
        id: preferencesAction
        text: qsTr("Preferences…")
        shortcut: "Ctrl+,"
        onTriggered: preferencesDialog.open()
    }

    Action {
        id: shortcutsAction
        text: qsTr("Keyboard Shortcuts")
        shortcut: "F1"
        onTriggered: shortcutsDialog.open()
    }

    // ---------- Menu bar ----------
    menuBar: MenuBar {
        Menu {
            title: qsTr("File")

            MenuItem {
                action: openTextAction
            }

            MenuItem {
                action: openVideoAction
            }

            MenuItem {
                action: openImageAction
            }

            MenuSeparator {}

            Menu {
                id: recentMenu
                title: qsTr("Recent")
                enabled: appBackend.recentFiles.length > 0

                Instantiator {
                    model: appBackend.recentFiles
                    delegate: MenuItem {
                        required property string modelData
                        text: appBackend.fileName(modelData)
                        onTriggered: window.openPath(modelData)
                    }
                    onObjectAdded: (index, object) => recentMenu.insertItem(index, object)
                    onObjectRemoved: (index, object) => recentMenu.removeItem(object)
                }

                MenuSeparator {}

                MenuItem {
                    text: qsTr("Clear list")
                    onTriggered: appBackend.clearRecent()
                }
            }

            MenuSeparator {}

            MenuItem {
                action: quitAction
            }
        }

        Menu {
            title: qsTr("View")

            // The check marks are refreshed when the menu opens, because the same
            // settings can also be changed in the Preferences dialog.
            onAboutToShow: {
                lightItem.checked = !appTheme.dark
                darkItem.checked = appTheme.dark
                statusBarItem.checked = appStore.showStatusBar
            }

            Menu {
                title: qsTr("Theme")

                MenuItem {
                    id: lightItem
                    text: qsTr("Light")
                    checkable: true
                    autoExclusive: true
                    checked: !appTheme.dark
                    onTriggered: appTheme.dark = false
                }

                MenuItem {
                    id: darkItem
                    text: qsTr("Dark")
                    checkable: true
                    autoExclusive: true
                    checked: appTheme.dark
                    onTriggered: appTheme.dark = true
                }
            }

            MenuItem {
                id: statusBarItem
                text: qsTr("Status Bar")
                checkable: true
                checked: appStore.showStatusBar
                onToggled: appStore.showStatusBar = checked
            }

            MenuSeparator {}

            Menu {
                title: qsTr("Go to Tab")

                MenuItem {
                    text: qsTr("Text")
                    onTriggered: tabBar.currentIndex = 0
                }

                MenuItem {
                    text: qsTr("Video")
                    onTriggered: tabBar.currentIndex = 1
                }

                MenuItem {
                    text: qsTr("Image")
                    onTriggered: tabBar.currentIndex = 2
                }

                MenuItem {
                    text: qsTr("Units")
                    onTriggered: tabBar.currentIndex = 3
                }

                MenuItem {
                    text: qsTr("Color")
                    onTriggered: tabBar.currentIndex = 4
                }

                MenuItem {
                    text: qsTr("Notes")
                    onTriggered: tabBar.currentIndex = 5
                }

                MenuItem {
                    text: qsTr("Timer")
                    onTriggered: tabBar.currentIndex = 6
                }
            }

            MenuItem {
                action: fullScreenAction
            }
        }

        Menu {
            title: qsTr("Tools")

            MenuItem {
                action: unitsAction
            }

            Menu {
                title: qsTr("Options")

                onAboutToShow: {
                    wrapItem.checked = appStore.wordWrap
                    autosaveItem.checked = appStore.autosave
                }

                MenuItem {
                    id: wrapItem
                    text: qsTr("Word Wrap")
                    checkable: true
                    checked: appStore.wordWrap
                    onToggled: appStore.wordWrap = checked
                }

                MenuItem {
                    id: autosaveItem
                    text: qsTr("Autosave")
                    checkable: true
                    checked: appStore.autosave
                    onToggled: appStore.autosave = checked
                }
            }

            MenuSeparator {}

            MenuItem {
                action: preferencesAction
            }
        }

        Menu {
            title: qsTr("Help")

            MenuItem {
                action: shortcutsAction
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("About Workbench")
                onTriggered: aboutDialog.open()
            }

            MenuItem {
                text: qsTr("About Qt")
                onTriggered: Qt.aboutQt()
            }
        }
    }

    // ---------- Tabs ----------
    header: TabBar {
        id: tabBar

        TabButton {
            text: qsTr("Text")
        }

        TabButton {
            text: qsTr("Video")
        }

        TabButton {
            text: qsTr("Image")
        }

        TabButton {
            text: qsTr("Units")
        }

        TabButton {
            text: qsTr("Color")
        }

        TabButton {
            text: qsTr("Notes")
        }

        TabButton {
            text: qsTr("Timer")
        }
    }

    StackLayout {
        id: pages
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        TextTab {
            id: textTab
            backend: appBackend
            theme: appTheme
            appState: appStore
            onOpenRequested: textDialog.open()
        }

        VideoTab {
            id: videoTab
            backend: appBackend
            theme: appTheme
            appState: appStore
            onOpenRequested: videoDialog.open()
        }

        ImageTab {
            id: imageTab
            backend: appBackend
            theme: appTheme
            appState: appStore
            onOpenRequested: imageDialog.open()
        }

        UnitsTab {
            backend: appBackend
            theme: appTheme
            appState: appStore
        }

        ColorTab {
            backend: appBackend
            theme: appTheme
            appState: appStore
        }

        NotesTab {
            backend: appBackend
            theme: appTheme
            appState: appStore
        }

        TimerTab {
            backend: appBackend
            theme: appTheme
            appState: appStore
        }
    }

    // ---------- Status bar ----------
    footer: ToolBar {
        visible: appStore.showStatusBar
        height: visible ? implicitHeight : 0

        Label {
            text: appStore.statusText
            color: appTheme.muted
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 14
        }
    }

    // ---------- Dialogs ----------
    FileDialog {
        id: textDialog
        title: qsTr("Open a text file")
        nameFilters: ["Text files (*.txt *.md *.log *.csv *.json)", "All files (*)"]
        onAccepted: window.openPath(appBackend.localPath(selectedFile))
    }

    FileDialog {
        id: videoDialog
        title: qsTr("Open a video")
        nameFilters: ["Video files (*.mp4 *.mkv *.avi *.mov *.webm)", "All files (*)"]
        onAccepted: window.openPath(appBackend.localPath(selectedFile))
    }

    FileDialog {
        id: imageDialog
        title: qsTr("Open an image")
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp *.gif)", "All files (*)"]
        onAccepted: window.openPath(appBackend.localPath(selectedFile))
    }

    Dialog {
        id: preferencesDialog
        title: qsTr("Preferences")
        modal: true
        standardButtons: Dialog.Close
        anchors.centerIn: parent

        ColumnLayout {
            spacing: 8

            Switch {
                text: qsTr("Dark theme")
                checked: appTheme.dark
                onToggled: appTheme.dark = checked
            }

            CheckBox {
                text: qsTr("Word wrap in the text viewer")
                checked: appStore.wordWrap
                onToggled: appStore.wordWrap = checked
            }

            CheckBox {
                text: qsTr("Autosave notes")
                checked: appStore.autosave
                onToggled: appStore.autosave = checked
            }

            CheckBox {
                text: qsTr("Show the status bar")
                checked: appStore.showStatusBar
                onToggled: appStore.showStatusBar = checked
            }
        }
    }

    Dialog {
        id: shortcutsDialog
        title: qsTr("Keyboard Shortcuts")
        modal: true
        standardButtons: Dialog.Close
        anchors.centerIn: parent

        Label {
            text: "Ctrl+O   Open text\nCtrl+Shift+O   Open video\nCtrl+I   Open image\n"
                  + "Ctrl+U   Unit converter\nCtrl+,   Preferences\nF11   Full screen\n"
                  + "F1   This list\nCtrl+Q   Quit"
        }
    }

    Dialog {
        id: aboutDialog
        title: qsTr("About Workbench")
        modal: true
        standardButtons: Dialog.Ok
        anchors.centerIn: parent

        Screen01 {
        }
    }
}
