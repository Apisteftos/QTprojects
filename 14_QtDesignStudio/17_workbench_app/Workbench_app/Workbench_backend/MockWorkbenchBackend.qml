import QtQuick

// A MOCK of the C++ class WorkbenchBackend (App/workbench_backend.h).
// Qt Design Studio cannot load C++ classes, so for the live preview it finds
// this file instead (the folder "mocks" is an import path in the .qmlproject).
// The real application never uses this file: the C++ class is registered under
// the same module name, "Workbench.Backend".
// It has the same members. The timer, the unit conversion, the colors and the search
// work in simple QML, so the app is usable when you press Run in Design Studio.
// It is not the tested logic: that is the C++ core library.
QtObject {
    id: mock

    property var recentFiles: ["/sample/notes.txt", "/sample/holiday.mp4", "/sample/leaf.png"]
    property string lastError: ""
    property string countdownText: "05:00"
    property real countdownProgress: 0
    property bool countdownRunning: false

    property int totalSeconds: 300
    property int leftSeconds: 300

    function clock(sec) {
        const m = Math.floor(sec / 60), r = sec % 60
        return String(m).padStart(2, "0") + ":" + String(r).padStart(2, "0")
    }

    function refresh() {
        countdownText = clock(leftSeconds)
        countdownProgress = totalSeconds > 0 ? 1 - leftSeconds / totalSeconds : 0
    }

    property Timer secondTimer: Timer {
        interval: 1000
        repeat: true
        onTriggered: {
            mock.leftSeconds = Math.max(0, mock.leftSeconds - 1)
            mock.refresh()
            if (mock.leftSeconds === 0) {
                stop()
                mock.countdownRunning = false
                mock.countdownFinished()
            }
        }
    }

    signal countdownFinished

    // Files
    function fileUrl(path) { return Qt.url("file://" + path) }
    function localPath(url) { return url.toString().replace("file://", "") }
    function fileName(path) { return path.split("/").pop() }
    function fileKind(path) {
        const ext = path.split(".").pop().toLowerCase()
        if (["txt", "md", "log"].indexOf(ext) >= 0) return "text"
        if (["mp4", "mkv", "mov"].indexOf(ext) >= 0) return "video"
        if (["png", "jpg", "jpeg"].indexOf(ext) >= 0) return "image"
        return "unknown"
    }
    function readTextFile(url) {
        lastError = ""
        try {
            const xhr = new XMLHttpRequest()
            xhr.open("GET", url, false)
            xhr.send()
            if (xhr.responseText !== "")
                return xhr.responseText
        } catch (e) {}
        // Qt blocks reading local files from QML unless QML_XHR_ALLOW_FILE_READ=1 is set.
        lastError = "Preview: cannot read the file (set QML_XHR_ALLOW_FILE_READ=1)"
        return ""
    }
    function addRecent(path) {}
    function clearRecent() {}

    // Text
    function findNext(text, query, from, caseSensitive) {
        if (query === "") return -1
        return caseSensitive ? text.indexOf(query, from) : text.toLowerCase().indexOf(query.toLowerCase(), from)
    }
    function countMatches(text, query, caseSensitive) {
        if (query === "") return 0
        return caseSensitive ? text.split(query).length - 1 : text.toLowerCase().split(query.toLowerCase()).length - 1
    }
    function countLines(text) { return text === "" ? 0 : text.split("\n").length }
    function formatClock(seconds) { return clock(seconds) }

    // Units (factors to the first unit of the list, temperature through Celsius)
    readonly property var unitTable: ({
        "Length": { "Meter": 1, "Foot": 0.3048, "Kilometer": 1000, "Mile": 1609.344, "Centimeter": 0.01, "Inch": 0.0254 },
        "Weight": { "Kilogram": 1, "Pound": 0.45359237, "Gram": 0.001, "Ounce": 0.028349523125 }
    })
    function categories() { return ["Length", "Weight", "Temperature"] }
    function unitsOf(category) {
        if (category === "Temperature") return ["Celsius", "Fahrenheit", "Kelvin"]
        return Object.keys(unitTable[category])
    }
    function convert(category, from, to, value) {
        const x = parseFloat(value)
        if (isNaN(x)) return ""
        let r
        if (category === "Temperature") {
            const c = from === "Fahrenheit" ? (x - 32) * 5 / 9 : from === "Kelvin" ? x - 273.15 : x
            r = to === "Fahrenheit" ? c * 9 / 5 + 32 : to === "Kelvin" ? c + 273.15 : c
        } else {
            r = x * unitTable[category][from] / unitTable[category][to]
        }
        return String(Math.round(r * 1e6) / 1e6)
    }

    // Colors
    function toHex(r, g, b) {
        const h = v => Math.max(0, Math.min(255, Math.round(v))).toString(16).padStart(2, "0")
        return ("#" + h(r) + h(g) + h(b)).toUpperCase()
    }
    function toRgbText(r, g, b) { return "rgb(" + r + ", " + g + ", " + b + ")" }
    function parseHex(text) {
        const m = /^#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})$/.exec(text)
        return m ? [parseInt(m[1], 16), parseInt(m[2], 16), parseInt(m[3], 16)] : []
    }

    // Notes
    function notesFolder() { return "/tmp" }
    function suggestedNoteName(title) {
        const name = title.trim().toLowerCase().replace(/[^a-z0-9]+/g, "-").replace(/^-+|-+$/g, "")
        return (name === "" ? "note" : name) + ".txt"
    }
    function saveTextFile(path, text) {
        try {
            const xhr = new XMLHttpRequest()
            // Asynchronous: a synchronous PUT creates an empty file.
            xhr.open("PUT", Qt.url("file://" + path))
            xhr.send(text)
            lastError = ""
            return true
        } catch (e) {}
        // Qt blocks writing local files from QML unless QML_XHR_ALLOW_FILE_WRITE=1 is set.
        lastError = "Preview: cannot write the file (set QML_XHR_ALLOW_FILE_WRITE=1)"
        return false
    }

    // Countdown
    function setCountdownMinutes(minutes) {
        secondTimer.stop()
        countdownRunning = false
        totalSeconds = minutes * 60
        leftSeconds = totalSeconds
        refresh()
    }
    function startCountdown() {
        if (leftSeconds === 0)
            leftSeconds = totalSeconds
        countdownRunning = true
        secondTimer.start()
        refresh()
    }
    function stopCountdown() {
        secondTimer.stop()
        countdownRunning = false
    }
    function resetCountdown() {
        secondTimer.stop()
        countdownRunning = false
        leftSeconds = totalSeconds
        refresh()
    }
}
