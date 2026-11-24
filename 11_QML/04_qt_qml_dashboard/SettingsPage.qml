import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        Label {
            text: "Settings"
            font.pixelSize: 22
            font.bold: true
            color: "#1565c0"
        }

        GroupBox {
            Layout.fillWidth: true
            title: "Update Interval"

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "100ms"; color: "#555" }

                    Slider {
                        id: intervalSlider
                        Layout.fillWidth: true
                        from:     100
                        to:       2000
                        stepSize: 100
                        value:    dataProvider.intervalMs

                        // onMoved — only fires on user interaction
                        // avoids binding loop with value: dataProvider.intervalMs
                        onMoved: dataProvider.intervalMs = value
                    }

                    Label { text: "2000ms"; color: "#555" }
                }

                Label {
                    text: "Current: " + dataProvider.intervalMs + " ms"
                    font.bold: true
                    color: "#1565c0"
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            title: "CPU Alert Threshold"

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "10%"; color: "#555" }

                    Slider {
                        id: thresholdSlider
                        Layout.fillWidth: true
                        from:     10
                        to:       100
                        stepSize: 5
                        value:    dataProvider.cpuThreshold

                        // onMoved — avoids binding loop
                        onMoved: dataProvider.cpuThreshold = value
                    }

                    Label { text: "100%"; color: "#555" }
                }

                Label {
                    text: "Alert when CPU exceeds: " + dataProvider.cpuThreshold.toFixed(0) + "%"
                    font.bold: true
                    color: dataProvider.cpuThreshold < 50 ? "#c62828" : "#1565c0"

                    Behavior on color {
                        ColorAnimation { duration: 300 }
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            title: "Controls"

            RowLayout {
                anchors.fill: parent
                spacing: 12

                Button {
                    text: dataProvider.isRunning ? "⏹ Stop Stream" : "▶ Start Stream"
                    Layout.fillWidth: true
                    onClicked: dataProvider.isRunning
                        ? dataProvider.stop()
                        : dataProvider.start()
                }

                Button {
                    text: "↺ Reset All"
                    Layout.fillWidth: true
                    onClicked: dataProvider.reset()
                }
            }
        }

        Item { Layout.fillHeight: true }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: dataProvider.statusText
            color: "#888"
            font.pixelSize: 12
            font.italic: true
        }
    }
}
