import QtQuick

Rectangle {
    id: root
    width: 800
    height: 400
    color: "#e6efe9"

    property alias itemName: nameText.text

    Text {
        id: titleText
        text: "Details"
        font.pixelSize: 36
        color: "#1d4d38"
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: card
        width: 320
        height: 140
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.top: titleText.bottom
        anchors.topMargin: 28
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: nameText
            text: "Item"
            font.pixelSize: 26
            color: "#1d4d38"
            anchors.centerIn: parent
        }
    }
}
