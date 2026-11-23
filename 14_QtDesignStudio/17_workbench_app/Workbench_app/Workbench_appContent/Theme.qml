import QtQuick

// The colors of the application. App.qml sets `dark` from the View > Theme menu.
QtObject {
    property bool dark: false

    readonly property color window: dark ? "#14211b" : "#eef2f0"
    readonly property color card: dark ? "#1d2b24" : "#ffffff"
    readonly property color text: dark ? "#e4efe9" : "#1d4d38"
    readonly property color muted: dark ? "#9bb0a5" : "#5b6b63"
    readonly property color border: dark ? "#34483e" : "#c9d6cf"
    readonly property color soft: dark ? "#2a3c33" : "#dfe8e3"
    readonly property color accent: "#2e7d5a"
}
