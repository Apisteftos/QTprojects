import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    // Header: stuck to the top, left and right edges
    Rectangle {
        id: header
        height: 64
        color: "#1d4d38"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            spacing: 12

            Text {
                id: headerTitle
                text: "Layout and anchors"
                font.pixelSize: 22
                color: "#ffffff"
            }

            Item {
                id: spacer
                Layout.fillWidth: true
            }

            Text {
                id: headerInfo
                text: "v1"
                font.pixelSize: 16
                color: "#c9d6cf"
            }
        }
    }

    // Sidebar: fixed width, between the header and the bottom edge
    Rectangle {
        id: sidebar
        width: 160
        color: "#dfe8e3"
        anchors.left: parent.left
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 16
            spacing: 12

            Text {
                text: "Home"
                font.pixelSize: 18
                color: "#1d4d38"
            }

            Text {
                text: "Items"
                font.pixelSize: 18
                color: "#1d4d38"
            }

            Text {
                text: "Settings"
                font.pixelSize: 18
                color: "#1d4d38"
            }
        }
    }

    // Content area: takes all the space that is left
    Item {
        id: content
        anchors.left: sidebar.right
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        Rectangle {
            id: card
            width: 320
            height: 260
            color: "#ffffff"
            radius: 16
            border.color: "#c9d6cf"
            border.width: 2
            anchors.centerIn: parent

            Image {
                id: leaf
                width: 96
                height: 96
                source: "assets/leaf.png"
                fillMode: Image.PreserveAspectFit
                anchors.top: parent.top
                anchors.topMargin: 28
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: title
                text: "Follows the window"
                font.pixelSize: 24
                color: "#1d4d38"
                anchors.top: leaf.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: subtitle
                text: "Nothing has a fixed x or y"
                font.pixelSize: 16
                color: "#5b6b63"
                anchors.top: title.bottom
                anchors.topMargin: 8
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
