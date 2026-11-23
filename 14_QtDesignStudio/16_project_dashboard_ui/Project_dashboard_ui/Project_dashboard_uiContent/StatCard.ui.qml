import QtQuick

Rectangle {
    id: root
    width: 220
    height: 110
    color: "#ffffff"
    radius: 16
    border.color: "#c9d6cf"
    border.width: 2

    property alias label: labelText.text
    property alias value: valueText.text
    property alias accent: accentBar.color

    Rectangle {
        id: accentBar
        width: 8
        color: "#2e7d5a"
        radius: 4
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 14
    }

    Text {
        id: labelText
        text: "Label"
        font.pixelSize: 15
        color: "#5b6b63"
        anchors.left: accentBar.right
        anchors.leftMargin: 16
        anchors.top: parent.top
        anchors.topMargin: 22
    }

    Text {
        id: valueText
        text: "0"
        font.pixelSize: 32
        color: "#1d4d38"
        anchors.left: accentBar.right
        anchors.leftMargin: 16
        anchors.top: labelText.bottom
        anchors.topMargin: 6
    }
}
