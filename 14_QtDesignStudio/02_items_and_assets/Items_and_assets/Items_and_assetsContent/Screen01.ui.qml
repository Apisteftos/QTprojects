import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Rectangle {
        id: card
        x: 220
        y: 90
        width: 360
        height: 300
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2

        Image {
            id: leaf
            x: 116
            y: 24
            width: 128
            height: 128
            source: "assets/leaf.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: title
            x: 0
            y: 170
            width: 360
            text: "Items and assets"
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            color: "#1d4d38"
        }

        Text {
            id: subtitle
            x: 0
            y: 216
            width: 360
            text: "Built in Qt Design Studio"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            color: "#5b6b63"
        }
    }
}
