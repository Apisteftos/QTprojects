import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ============================================================
// DashboardPage — live metrics overview
//
// Shows three metric cards with animated value transitions.
// Each card uses a Canvas-drawn gauge arc.
//
// Key QML concepts:
//   - Canvas for custom drawing (gauge arcs)
//   - Behavior { NumberAnimation } for smooth transitions
//   - States + Transitions for color changes
// ============================================================
Item {
    id: root

    // ── Header ─────────────────────────────────────────────
    Rectangle {
        id: header
        anchors.top:   parent.top
        anchors.left:  parent.left
        anchors.right: parent.right
        height: 60
        color: "#1565c0"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 16

            Column {
                Label {
                    text: "System Dashboard"
                    color: "white"
                    font.pixelSize: 20
                    font.bold: true
                }
                Label {
                    text: dataProvider.statusText
                    color: "#bbdefb"
                    font.pixelSize: 12
                }
            }

            Item { Layout.fillWidth: true }

            // Start / Stop button
            Button {
                text: dataProvider.isRunning ? "⏹ Stop" : "▶ Start"
                onClicked: dataProvider.isRunning
                    ? dataProvider.stop()
                    : dataProvider.start()
            }

            Button {
                text: "↺ Reset"
                onClicked: dataProvider.reset()
            }
        }
    }

    // ── Metric cards ────────────────────────────────────────
    GridLayout {
        anchors.top:    header.bottom
        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.bottom: statsBar.top
        anchors.margins: 16
        columns: 3
        rowSpacing: 16
        columnSpacing: 16

        MetricCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title:     "CPU Load"
            value:     dataProvider.cpuLoad
            unit:      "%"
            maxValue:  100
            accentColor: dataProvider.cpuLoad > 80 ? "#c62828" :
                       dataProvider.cpuLoad > 60 ? "#f57c00" : "#1565c0"
            icon:      "💻"
        }

        MetricCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title:     "Memory"
            value:     dataProvider.memoryMb
            unit:      "MB"
            maxValue:  800
            accentColor: dataProvider.memoryMb > 600 ? "#c62828" :
                       dataProvider.memoryMb > 400 ? "#f57c00" : "#2e7d32"
            icon:      "🧠"
        }

        MetricCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title:     "Network"
            value:     dataProvider.networkKbps
            unit:      "KB/s"
            maxValue:  1000
            color:     "#6a1b9a"
            icon:      "🌐"
        }
    }

    // ── Status bar ─────────────────────────────────────────
    Rectangle {
        id: statsBar
        anchors.bottom: parent.bottom
        anchors.left:   parent.left
        anchors.right:  parent.right
        height: 36
        color: "#f5f5f5"
        border.color: "#e0e0e0"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 24

            Label {
                text: "Samples: " + dataProvider.sampleCount
                font.pixelSize: 12
                color: "#555"
            }
            Label {
                text: "Interval: " + dataProvider.intervalMs + "ms"
                font.pixelSize: 12
                color: "#555"
            }
            Label {
                text: "CPU threshold: " + dataProvider.cpuThreshold + "%"
                font.pixelSize: 12
                color: "#555"
            }
            Item { Layout.fillWidth: true }
            Label {
                text: dataProvider.hasAlert ? "⚠ ALERT ACTIVE" : "✓ Normal"
                font.pixelSize: 12
                font.bold: dataProvider.hasAlert
                color: dataProvider.hasAlert ? "#c62828" : "#2e7d32"
            }
        }
    }
}
