import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 320
    height: 320
    visible: true
    title: qsTr("Testing QML")

    OrderForm {
        anchors.fill: parent
        anchors.margins: 16
    }
}
