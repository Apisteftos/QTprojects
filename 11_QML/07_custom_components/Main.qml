import QtQuick

Window {
    id: root
    width: 420
    height: 480
    visible: true
    title: qsTr("Custom Components")
    color: "#eef1f5"

    property int likes: 0

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        // === Card and AppButton are our own types, from Card.qml and AppButton.qml ===
        Card {
            title: qsTr("Likes")
            width: parent.width

            Column {
                spacing: 8
                Text { text: qsTr("This post has %1 likes").arg(root.likes) }
                Row {
                    spacing: 8
                    AppButton { text: qsTr("Like"); onClicked: root.likes += 1 }
                    AppButton {
                        text: qsTr("Unlike")
                        baseColor: "#c0392b"
                        enabled: root.likes > 0
                        onClicked: root.likes -= 1
                    }
                }
            }
        }

        Card {
            title: qsTr("Badges")
            width: parent.width

            Column {
                spacing: 8
                AppButton {
                    text: qsTr("Add a badge")
                    baseColor: "#27ae60"
                    onClicked: {
                        // === Component: a piece of QML that is not created at the start, but on demand ===
                        badge.createObject(badgeRow, { text: "#" + (badgeRow.children.length + 1) })
                    }
                }
                Row {
                    id: badgeRow
                    spacing: 6
                }
            }
        }

        Component {
            id: badge
            Rectangle {
                property string text: ""
                width: 40
                height: 24
                radius: 12
                color: "#e67e22"
                Text {
                    anchors.centerIn: parent
                    text: parent.text
                    color: "white"
                }
            }
        }
    }
}
