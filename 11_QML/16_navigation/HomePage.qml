import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// The first page of the stack. It does not know the stack: it only sends a signal.
Page {
    id: page
    title: qsTr("Home")

    signal openDetail(string name)
    signal openTabs()

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 12

        Label {
            text: qsTr("Choose where to go")
            font.pixelSize: 20
            Layout.alignment: Qt.AlignHCenter
        }
        Button { text: qsTr("Details: Alpha"); Layout.fillWidth: true; onClicked: page.openDetail("Alpha") }
        Button { text: qsTr("Details: Beta"); Layout.fillWidth: true; onClicked: page.openDetail("Beta") }
        Button { text: qsTr("Tabs and swipe pages"); Layout.fillWidth: true; onClicked: page.openTabs() }
    }
}
