import QtQuick

// A stand-in ("mock") for the real application logic.
// The developer later replaces it with a C++ class that has the same name and the same members.
QtObject {
    id: backend

    property int level: 40
    property bool running: false
    readonly property string status: running ? "Running" : "Idle"

    function start() {
        running = true
    }

    function stop() {
        running = false
    }

    property Timer ticker: Timer {
        interval: 400
        repeat: true
        running: backend.running
        onTriggered: backend.level = (backend.level + 7) % 101
    }
}
