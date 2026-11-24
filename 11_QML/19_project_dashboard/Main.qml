import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 520
    height: 520
    visible: true
    title: qsTr("Dashboard")
    color: "#eef1f5"

    // === The backend: a C++ class that QML creates like any other element ===
    Dashboard {
        id: dashboard
        interval: intervalSlider.value
        Component.onCompleted: running = true       // start with live data
    }

    // === The alarm: a signal from C++ starts an animation ===
    Connections {
        target: dashboard
        function onAlarmRaised(level, message) {
            toastText.text = message
            toast.accent = level === "critical" ? "#c0392b" : "#e0a800"
            toastAnimation.restart()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // --- Header ---
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: qsTr("Machine dashboard")
                font.pixelSize: 22
                font.bold: true
                Layout.fillWidth: true
            }
            Switch {
                objectName: "liveSwitch"
                text: qsTr("Live")
                checked: dashboard.running
                onToggled: dashboard.running = checked
            }
            Button {
                text: qsTr("One step")
                enabled: !dashboard.running
                onClicked: dashboard.advance()
            }
            Button {
                text: qsTr("Reset")
                onClicked: dashboard.reset()
            }
        }

        // --- Three cards. Each is a MetricCard; all bind to the backend ---
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            MetricCard {
                Layout.fillWidth: true
                title: qsTr("Load")
                value: dashboard.load
                unit: "%"
                decimals: 1
                level: dashboard.level
            }
            MetricCard {
                Layout.fillWidth: true
                title: qsTr("Temperature")
                value: dashboard.temperature
                unit: "°C"
                decimals: 1
                level: dashboard.level
            }
            MetricCard {
                Layout.fillWidth: true
                title: qsTr("Requests")
                value: dashboard.requests
                unit: "/s"
                level: dashboard.level
            }
        }

        // --- The history as bars ---
        Label {
            text: qsTr("Load of the last %1 measurements").arg(dashboard.capacity)
            color: "#5b6573"
        }
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 12
            color: "white"
            border.color: "#d5dbe3"
            border.width: 2

            BarChart {
                anchors.fill: parent
                anchors.margins: 12
                values: dashboard.history
                capacity: dashboard.capacity
            }
        }

        // --- The speed ---
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: qsTr("Interval: %1 ms").arg(intervalSlider.value)
                Layout.preferredWidth: 130
            }
            Slider {
                id: intervalSlider
                objectName: "intervalSlider"
                from: 100
                to: 2000
                stepSize: 100
                value: 500
                Layout.fillWidth: true
            }
        }
    }

    // === The message: it fades in, waits and fades out. A SequentialAnimation does the three steps ===
    Rectangle {
        id: toast
        property color accent: "#e0a800"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 8
        width: toastText.implicitWidth + 32
        height: toastText.implicitHeight + 16
        radius: 8
        color: accent
        opacity: 0

        Label {
            id: toastText
            objectName: "toastText"
            anchors.centerIn: parent
            color: "white"
            font.bold: true
        }

        SequentialAnimation {
            id: toastAnimation
            NumberAnimation { target: toast; property: "opacity"; to: 1; duration: 150 }
            PauseAnimation { duration: 1800 }
            NumberAnimation { target: toast; property: "opacity"; to: 0; duration: 400 }
        }
    }
}
