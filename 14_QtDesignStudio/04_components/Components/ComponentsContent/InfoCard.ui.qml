import QtQuick

Rectangle {
    id: root
    width: 220
    height: 240
    color: "#ffffff"
    radius: 16
    border.color: "#c9d6cf"
    border.width: 2

    property alias title: titleText.text
    property alias subtitle: subtitleText.text
    property alias accent: accentBar.color

    Rectangle {
        id: accentBar
        height: 8
        radius: 4
        color: "#2e7d5a"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 16
    }

    Image {
        id: icon
        width: 80
        height: 80
        source: "assets/leaf.png"
        fillMode: Image.PreserveAspectFit
        anchors.top: accentBar.bottom
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: titleText
        text: "Title"
        font.pixelSize: 22
        color: "#1d4d38"
        anchors.top: icon.bottom
        anchors.topMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: subtitleText
        text: "Subtitle"
        font.pixelSize: 15
        color: "#5b6b63"
        anchors.top: titleText.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
