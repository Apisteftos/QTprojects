import QtQuick

Rectangle {
    id: root
    width: 800
    height: 400
    color: "#f3efe6"

    Text {
        id: titleText
        text: "Settings"
        font.pixelSize: 36
        color: "#1d4d38"
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: infoText
        text: "Nothing to set yet."
        font.pixelSize: 18
        color: "#5b6b63"
        anchors.top: titleText.bottom
        anchors.topMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
