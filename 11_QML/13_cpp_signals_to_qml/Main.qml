import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 460
    height: 520
    visible: true
    title: qsTr("C++ Signals to QML")
    color: "#eef1f5"

    // The alarms are collected here. QML reacts to the C++ signal and writes into a model.
    ListModel { id: alarms }

    // === Connections: react to the signals of a C++ object ===
    // "Sensor" is the singleton. It needs no id and no declaration: it is used by its name everywhere.
    Connections {
        target: Sensor
        // The handler gets the arguments of the signal by name: alarm(double value, int reading)
        function onAlarm(value, reading) {
            alarms.insert(0, { message: qsTr("Reading %1: %2 is above the limit").arg(reading).arg(value.toFixed(1)) })
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // === A property with NOTIFY: bindings update at every reading ===
        Text {
            text: Sensor.value.toFixed(1) + " °C"
            font.pixelSize: 48
            Layout.alignment: Qt.AlignHCenter
            color: Sensor.value > Sensor.threshold ? "#c0392b" : "#1f2d3d"
        }
        Rectangle {
            Layout.fillWidth: true
            height: 20
            radius: 10
            color: "white"
            Rectangle {
                height: parent.height
                radius: 10
                width: parent.width * Math.max(0, Math.min(1, Sensor.value / 40))
                color: Sensor.value > Sensor.threshold ? "#c0392b" : "#2e86de"
            }
        }
        Text {
            text: qsTr("Readings: %1").arg(Sensor.readings)
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8
            Button {
                text: Sensor.running ? qsTr("Stop") : qsTr("Start")
                onClicked: Sensor.running ? Sensor.stop() : Sensor.start()
            }
            Button { text: qsTr("One reading"); enabled: !Sensor.running; onClicked: Sensor.advance() }
            Button { text: qsTr("Clear"); onClicked: alarms.clear() }
        }

        RowLayout {
            Label { text: qsTr("Interval (ms)") }
            Slider {
                Layout.fillWidth: true
                from: 50
                to: 1000
                stepSize: 50
                value: Sensor.interval
                onMoved: Sensor.interval = value        // writing a C++ property
            }
            Label { text: Sensor.interval; Layout.preferredWidth: 36 }
        }
        RowLayout {
            Label { text: qsTr("Alarm above") }
            Slider {
                Layout.fillWidth: true
                from: 10
                to: 35
                stepSize: 1
                value: Sensor.threshold
                onMoved: Sensor.threshold = value
            }
            Label { text: Sensor.threshold.toFixed(0); Layout.preferredWidth: 36 }
        }

        Label { text: qsTr("Alarms"); font.bold: true }
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 4
            model: alarms
            delegate: Text {
                required property string message    // the role of the model (not named "text": that is a property of Text)
                text: message
                color: "#c0392b"
            }
        }
    }
}
