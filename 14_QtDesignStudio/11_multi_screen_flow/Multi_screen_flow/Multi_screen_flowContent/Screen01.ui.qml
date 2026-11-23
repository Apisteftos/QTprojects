import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"
    state: "home"

    HomeScreen {
        id: homeScreen
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: navBar.top
    }

    DetailsScreen {
        id: detailsScreen
        itemName: "Item 42"
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: navBar.top
    }

    SettingsScreen {
        id: settingsScreen
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: navBar.top
    }

    Rectangle {
        id: navBar
        height: 80
        color: "#ffffff"
        border.color: "#c9d6cf"
        border.width: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Row {
            id: tabRow
            spacing: 16
            anchors.centerIn: parent

        Rectangle {
            id: homeTab
            width: 120
            height: 40
            color: "#2e7d5a"
            radius: 8
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: homeLabel
                text: "Home"
                font.pixelSize: 18
                color: "#ffffff"
                anchors.centerIn: parent
            }

            MouseArea {
                id: homeArea
                anchors.fill: parent
            }
        }

        Rectangle {
            id: detailsTab
            width: 120
            height: 40
            color: "#dfe8e3"
            radius: 8
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: detailsLabel
                text: "Details"
                font.pixelSize: 18
                color: "#1d4d38"
                anchors.centerIn: parent
            }

            MouseArea {
                id: detailsArea
                anchors.fill: parent
            }
        }

        Rectangle {
            id: settingsTab
            width: 120
            height: 40
            color: "#dfe8e3"
            radius: 8
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: settingsLabel
                text: "Settings"
                font.pixelSize: 18
                color: "#1d4d38"
                anchors.centerIn: parent
            }

            MouseArea {
                id: settingsArea
                anchors.fill: parent
            }
        }
        }
    }

    Connections {
        target: homeArea
        function onClicked() { root.state = "home" }
    }

    Connections {
        target: detailsArea
        function onClicked() { root.state = "details" }
    }

    Connections {
        target: settingsArea
        function onClicked() { root.state = "settings" }
    }

    Connections {
        target: homeScreen.openArea
        function onClicked() { root.state = "details" }
    }

    states: [
        State {
            name: "home"
        },
        State {
            name: "details"

            PropertyChanges {
                target: homeScreen
                visible: false
            }

            PropertyChanges {
                target: detailsScreen
                visible: true
            }

            PropertyChanges {
                target: detailsTab
                color: "#2e7d5a"
            }

            PropertyChanges {
                target: detailsLabel
                color: "#ffffff"
            }

            PropertyChanges {
                target: homeTab
                color: "#dfe8e3"
            }

            PropertyChanges {
                target: homeLabel
                color: "#1d4d38"
            }
        },
        State {
            name: "settings"

            PropertyChanges {
                target: homeScreen
                visible: false
            }

            PropertyChanges {
                target: settingsScreen
                visible: true
            }

            PropertyChanges {
                target: settingsTab
                color: "#2e7d5a"
            }

            PropertyChanges {
                target: settingsLabel
                color: "#ffffff"
            }

            PropertyChanges {
                target: homeTab
                color: "#dfe8e3"
            }

            PropertyChanges {
                target: homeLabel
                color: "#1d4d38"
            }
        }
    ]
}
