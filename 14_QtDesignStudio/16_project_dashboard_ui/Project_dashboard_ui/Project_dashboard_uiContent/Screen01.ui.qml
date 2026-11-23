import QtQuick

Rectangle {
    id: root
    width: 960
    height: 600
    color: "#eef2f0"
    state: "overview"

    // The plant that is selected in the chart or in the list
    property int selectedIndex: 0

    ListModel {
        id: plantModel

        ListElement {
            name: "Fern"
            water: 80
            tint: "#2e7d5a"
        }

        ListElement {
            name: "Cactus"
            water: 25
            tint: "#7aa35a"
        }

        ListElement {
            name: "Orchid"
            water: 55
            tint: "#b05fa0"
        }

        ListElement {
            name: "Moss"
            water: 90
            tint: "#4f8a6b"
        }

        ListElement {
            name: "Bonsai"
            water: 40
            tint: "#8a6b4f"
        }
    }

    // ---------- Sidebar ----------
    Rectangle {
        id: sidebar
        width: 200
        color: "#1d4d38"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Text {
            id: appTitle
            text: "Plant Care"
            font.pixelSize: 26
            color: "#ffffff"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 24
        }

        Rectangle {
            id: navOverview
            height: 48
            radius: 8
            color: "#2e7d5a"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: appTitle.bottom
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            anchors.topMargin: 36

            Text {
                text: "Overview"
                font.pixelSize: 18
                color: "#ffffff"
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: overviewArea
                anchors.fill: parent
            }
        }

        Rectangle {
            id: navPlants
            height: 48
            radius: 8
            color: "#1d4d38"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: navOverview.bottom
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            anchors.topMargin: 8

            Text {
                text: "Plants"
                font.pixelSize: 18
                color: "#ffffff"
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: plantsArea
                anchors.fill: parent
            }
        }
    }

    // ---------- Page 1: Overview ----------
    Item {
        id: overviewPage
        anchors.left: sidebar.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 24
        visible: opacity > 0

        Row {
            id: statRow
            spacing: 16
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            StatCard {
                id: totalCard
                width: (statRow.width - 32) / 3
                label: "Plants"
                value: plantModel.count
            }

            StatCard {
                id: nameCard
                width: (statRow.width - 32) / 3
                label: "Selected"
                value: plantList.currentItem ? plantList.currentItem.plantName : "-"
                accent: "#d9a21b"
            }

            StatCard {
                id: waterCard
                width: (statRow.width - 32) / 3
                label: "Water level"
                value: plantList.currentItem ? plantList.currentItem.waterValue + " %" : "-"
                accent: "#3a7fb0"
            }
        }

        Rectangle {
            id: chartCard
            color: "#ffffff"
            radius: 16
            border.color: "#c9d6cf"
            border.width: 2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: statRow.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: 24

            Text {
                id: chartTitle
                text: "Water level per plant"
                font.pixelSize: 22
                color: "#1d4d38"
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 20
            }

            Row {
                id: barRow
                spacing: 32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 24

                Repeater {
                    id: barRepeater
                    model: plantModel

                    delegate: Item {
                        id: barItem
                        width: 72
                        height: 260

                        Rectangle {
                            id: bar
                            width: parent.width
                            height: 200 * model.water / 100
                            radius: 8
                            color: model.tint
                            border.color: "#1d4d38"
                            border.width: index === root.selectedIndex ? 3 : 0
                            anchors.bottom: barLabel.top
                            anchors.bottomMargin: 8
                        }

                        Text {
                            id: barLabel
                            text: model.name
                            font.pixelSize: 16
                            color: "#1d4d38"
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottom: parent.bottom
                        }

                        MouseArea {
                            id: barArea
                            anchors.fill: parent
                            onClicked: root.selectedIndex = index
                        }
                    }
                }
            }
        }
    }

    // ---------- Page 2: Plants ----------
    Item {
        id: plantsPage
        anchors.left: sidebar.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 24
        opacity: 0
        visible: opacity > 0

        Rectangle {
            id: listCard
            color: "#ffffff"
            radius: 16
            border.color: "#c9d6cf"
            border.width: 2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: actionRow.top
            anchors.bottomMargin: 16

            ListView {
                id: plantList
                clip: true
                model: plantModel
                currentIndex: root.selectedIndex
                anchors.fill: parent
                anchors.margins: 16

                delegate: Rectangle {
                    id: plantRow
                    width: plantList.width
                    height: 60
                    radius: 8
                    color: ListView.isCurrentItem ? "#dfe8e3" : "#ffffff"

                    property string plantName: model.name
                    property int waterValue: model.water

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

                    Rectangle {
                        id: miniTrack
                        width: 220
                        height: 12
                        radius: 6
                        color: "#dfe8e3"
                        anchors.right: percentText.left
                        anchors.rightMargin: 16
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            id: miniFill
                            width: parent.width * model.water / 100
                            height: parent.height
                            radius: 6
                            color: model.water < 30 ? "#c0563b" : "#2e7d5a"
                        }
                    }

                    Text {
                        id: percentText
                        text: model.water + " %"
                        font.pixelSize: 18
                        color: "#5b6b63"
                        width: 56
                        horizontalAlignment: Text.AlignRight
                        anchors.right: parent.right
                        anchors.rightMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    MouseArea {
                        id: rowArea
                        anchors.fill: parent
                        onClicked: root.selectedIndex = index
                    }
                }
            }
        }

        Row {
            id: actionRow
            spacing: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom

            Rectangle {
                id: waterButton
                width: 140
                height: 44
                color: "#2e7d5a"
                radius: 8

                Text {
                    text: "Water"
                    font.pixelSize: 18
                    color: "#ffffff"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: waterArea
                    anchors.fill: parent
                }
            }

            Rectangle {
                id: dryButton
                width: 140
                height: 44
                color: "#dfe8e3"
                radius: 8

                Text {
                    text: "Dry out"
                    font.pixelSize: 18
                    color: "#1d4d38"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: dryArea
                    anchors.fill: parent
                }
            }
        }
    }

    states: [
        State {
            name: "overview"

            PropertyChanges {
                target: overviewPage
                opacity: 1
            }

            PropertyChanges {
                target: plantsPage
                opacity: 0
            }

            PropertyChanges {
                target: navOverview
                color: "#2e7d5a"
            }

            PropertyChanges {
                target: navPlants
                color: "#1d4d38"
            }
        },
        State {
            name: "plants"

            PropertyChanges {
                target: overviewPage
                opacity: 0
            }

            PropertyChanges {
                target: plantsPage
                opacity: 1
            }

            PropertyChanges {
                target: navOverview
                color: "#1d4d38"
            }

            PropertyChanges {
                target: navPlants
                color: "#2e7d5a"
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"

            NumberAnimation {
                properties: "opacity"
                duration: 250
            }

            ColorAnimation {
                duration: 250
            }
        }
    ]

    // ---------- Actions ----------
    Connections {
        target: overviewArea
        function onClicked() {
            root.state = "overview"
        }
    }

    Connections {
        target: plantsArea
        function onClicked() {
            root.state = "plants"
        }
    }

    Connections {
        target: waterArea
        function onClicked() {
            plantModel.setProperty(root.selectedIndex, "water", 100)
        }
    }

    Connections {
        target: dryArea
        function onClicked() {
            plantModel.setProperty(root.selectedIndex, "water",
                                   Math.max(0, plantModel.get(root.selectedIndex).water - 20))
        }
    }
}
