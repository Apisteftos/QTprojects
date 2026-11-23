import QtQuick

Rectangle {
    id: root
    width: 800
    height: 400
    color: "#eef2f0"

    property alias openArea: openArea

    Text {
        id: titleText
        text: "Home"
        font.pixelSize: 36
        color: "#1d4d38"
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: infoText
        text: "Welcome. Open the details of an item."
        font.pixelSize: 18
        color: "#5b6b63"
        anchors.top: titleText.bottom
        anchors.topMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: openButton
        width: 200
        height: 48
        color: "#2e7d5a"
        radius: 8
        anchors.top: infoText.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: "Open details"
            font.pixelSize: 18
            color: "#ffffff"
            anchors.centerIn: parent
        }

        MouseArea {
            id: openArea
            anchors.fill: parent
        }
    }
}
