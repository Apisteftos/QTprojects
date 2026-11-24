import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// A page that gets its data when it is pushed: "stack.push(component, { itemName: "Alpha" })"
Page {
    id: page

    required property string itemName
    property int level: 1
    title: qsTr("Details: %1").arg(itemName)

    signal deeper()
    signal backToStart()

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 12

        Label {
            text: qsTr("This is the page of %1, level %2").arg(page.itemName).arg(page.level)
            font.pixelSize: 18
            Layout.alignment: Qt.AlignHCenter
        }
        Button { text: qsTr("Go deeper"); Layout.fillWidth: true; onClicked: page.deeper() }
        Button { text: qsTr("Back to the start"); Layout.fillWidth: true; onClicked: page.backToStart() }
    }
}
