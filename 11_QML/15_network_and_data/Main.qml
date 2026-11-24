import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 480
    height: 560
    visible: true
    title: qsTr("Network and Data")
    color: "#eef1f5"

    // The backend is a C++ object. The URL of the data is a normal property.
    StationClient {
        id: stations
        // source: "https://example.org/stations.json"   // a real server would be set here
    }

    // Load once when the window is ready
    Component.onCompleted: stations.load()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        RowLayout {
            Label { text: qsTr("Weather stations"); font.bold: true; font.pixelSize: 18; Layout.fillWidth: true }
            BusyIndicator { running: stations.loading; implicitWidth: 28; implicitHeight: 28 }
            Button { text: qsTr("Reload"); enabled: !stations.loading; onClicked: stations.load() }
        }

        // Status line: loading, error, or the result
        Label {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: stations.loading ? qsTr("Loading ...")
                : stations.error !== "" ? qsTr("Error: %1").arg(stations.error)
                : qsTr("%1 stations, average %2 °C, updated %3").arg(stations.count).arg(stations.average.toFixed(1)).arg(stations.updated)
            color: stations.error !== "" ? "#c0392b" : "#555"
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 4
            model: stations
            delegate: Rectangle {
                id: row
                required property string name
                required property string city
                required property real temperature
                width: ListView.view.width
                height: 48
                radius: 6
                color: "white"
                Column {
                    x: 12
                    anchors.verticalCenter: parent.verticalCenter
                    Label { text: row.name; font.bold: true }
                    Label { text: row.city; font.pixelSize: 12; color: "#777" }
                }
                Label {
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    text: row.temperature.toFixed(1) + " °C"
                    color: row.temperature < 5 ? "#2e86de" : "#e67e22"
                    font.bold: true
                }
            }
        }

        // Try another source: a file, a resource or a web address
        RowLayout {
            TextField {
                id: address
                Layout.fillWidth: true
                placeholderText: qsTr("URL of a JSON file")
                text: stations.source
                selectByMouse: true
            }
            Button {
                text: qsTr("Load URL")
                onClicked: {
                    stations.source = address.text
                    stations.load()
                }
            }
        }
    }
}
