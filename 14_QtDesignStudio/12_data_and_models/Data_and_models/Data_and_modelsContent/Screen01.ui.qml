import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    ListModel {
        id: plantModel

        ListElement {
            name: "Fern"
            count: 12
            tint: "#2e7d5a"
        }

        ListElement {
            name: "Cactus"
            count: 3
            tint: "#7aa35a"
        }

        ListElement {
            name: "Orchid"
            count: 7
            tint: "#b05fa0"
        }

        ListElement {
            name: "Moss"
            count: 20
            tint: "#4f8a6b"
        }

        ListElement {
            name: "Bonsai"
            count: 1
            tint: "#8a6b4f"
        }
    }

    Rectangle {
        id: listCard
        width: 360
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 24

        Text {
            id: listTitle
            text: "Plants (" + plantModel.count + ")"
            font.pixelSize: 24
            color: "#1d4d38"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 20
        }

        ListView {
            id: plantList
            clip: true
            model: plantModel
            currentIndex: 0
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: listTitle.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 16

            delegate: Rectangle {
                id: plantRow
                width: plantList.width
                height: 52
                radius: 8
                color: ListView.isCurrentItem ? "#dfe8e3" : "#ffffff"

                Rectangle {
                    id: swatch
                    width: 24
                    height: 24
                    radius: 12
                    color: model.tint
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: nameText
                    text: model.name
                    font.pixelSize: 20
                    color: "#1d4d38"
                    anchors.left: swatch.right
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: countText
                    text: model.count
                    font.pixelSize: 18
                    color: "#5b6b63"
                    anchors.right: parent.right
                    anchors.rightMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                    id: rowArea
                    anchors.fill: parent
                    onClicked: plantList.currentIndex = index
                }
            }
        }
    }

    Rectangle {
        id: detailCard
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.left: listCard.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 24
        anchors.rightMargin: 24
        anchors.topMargin: 24
        anchors.bottomMargin: 24

        Rectangle {
            id: bigSwatch
            width: 80
            height: 80
            radius: 40
            color: plantList.currentIndex >= 0 ? plantModel.get(plantList.currentIndex).tint : "#dfe8e3"
            anchors.top: parent.top
            anchors.topMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: detailName
            text: plantList.currentIndex >= 0 ? plantModel.get(plantList.currentIndex).name : "Nothing selected"
            font.pixelSize: 30
            color: "#1d4d38"
            anchors.top: bigSwatch.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: detailCount
            text: plantList.currentIndex >= 0 ? plantModel.get(plantList.currentIndex).count + " in stock" : ""
            font.pixelSize: 18
            color: "#5b6b63"
            anchors.top: detailName.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: buttonRow
            spacing: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: addButton
                width: 120
                height: 44
                color: "#2e7d5a"
                radius: 8

                Text {
                    text: "Add"
                    font.pixelSize: 18
                    color: "#ffffff"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: addArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: removeButton
                width: 120
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Remove"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: removeArea
                    anchors.fill: parent
                }
            }
        }
    }

    Connections {
        target: addArea
        function onClicked() {
            plantModel.append({"name": "New plant", "count": 1, "tint": "#c9a227"})
            plantList.currentIndex = plantModel.count - 1
        }
    }

    Connections {
        target: removeArea
        function onClicked() {
            if (plantList.currentIndex >= 0)
                plantModel.remove(plantList.currentIndex)
        }
    }
}
