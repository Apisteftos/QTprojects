import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ============================================================
// ChartsPage — live line charts using Canvas
//
// Draws live updating charts from QVariantList history data.
// No Qt Charts module needed — pure QML Canvas API.
//
// Demonstrates:
//   - Canvas drawing — lines, fills, text, grid
//   - Connections to C++ signals
//   - Repeater for dynamic chart generation
// ============================================================
Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Live Charts — Last 30 Samples"
            font.pixelSize: 18
            font.bold: true
            color: "#1565c0"
        }

        // CPU Chart
        ChartCanvas {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title:    "CPU Load (%)"
            data:     dataProvider.cpuHistory
            maxValue: 100
            lineColor: dataProvider.cpuLoad > 80 ? "#c62828" : "#1565c0"
            fillColor: dataProvider.cpuLoad > 80 ? "#ffcdd2" : "#e3f2fd"
        }

        // Memory Chart
        ChartCanvas {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title:    "Memory Usage (MB)"
            data:     dataProvider.memoryHistory
            maxValue: 800
            lineColor: "#2e7d32"
            fillColor: "#e8f5e9"
        }

        // Network Chart
        ChartCanvas {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title:    "Network (KB/s)"
            data:     dataProvider.networkHistory
            maxValue: 1000
            lineColor: "#6a1b9a"
            fillColor: "#f3e5f5"
        }
    }
}
