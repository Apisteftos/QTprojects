pragma Singleton
import QtQuick

// One place for the values of the own look: colours, sizes, fonts.
// "pragma Singleton" + QT_QML_SINGLETON_TYPE in CMakeLists.txt: every file can use "Theme.card".
QtObject {
    property bool dark: false              // Main.qml sets it (with a Binding)

    readonly property int spacing: 12
    readonly property int radius: 10
    readonly property int fontLarge: 22
    readonly property int fontNormal: 14

    readonly property color brand: "#1f9d8f"
    readonly property color card: dark ? "#2b2f36" : "#eef2f7"
    readonly property color text: dark ? "#e8ecf1" : "#1d232b"
}
