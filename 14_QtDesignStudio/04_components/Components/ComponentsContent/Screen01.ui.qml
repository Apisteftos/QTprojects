import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Row {
        id: cardRow
        spacing: 24
        anchors.centerIn: parent

        InfoCard {
            id: cardOne
            title: "Plants"
            subtitle: "12 items"
        }

        InfoCard {
            id: cardTwo
            title: "Water"
            subtitle: "3 items"
            accent: "#2f6fb0"
        }

        InfoCard {
            id: cardThree
            title: "Sun"
            subtitle: "7 items"
            accent: "#d9a21b"
        }
    }
}
