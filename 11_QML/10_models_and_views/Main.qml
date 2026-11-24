import QtQuick
import QtQuick.Controls

Window {
    width: 480
    height: 520
    visible: true
    title: qsTr("Models and Views")
    color: "#eef1f5"

    // === The model: the data. A ListModel is a list of objects with named roles (here: name and quantity) ===
    ListModel {
        id: fruits
        ListElement { name: "Apple"; quantity: 4 }
        ListElement { name: "Banana"; quantity: 12 }
        ListElement { name: "Cherry"; quantity: 30 }
        ListElement { name: "Dates"; quantity: 8 }
    }

    property int nextNumber: 1

    // === A second view of the same model: a Repeater makes one item for each row, with no scrolling ===
    Flow {
        id: chips
        x: 20
        y: 16
        width: parent.width - 40
        spacing: 6
        Repeater {
            model: fruits
            delegate: Rectangle {
                required property string name           // the delegate gets the roles as properties
                required property int quantity
                width: chipText.width + 20
                height: 26
                radius: 13
                color: "#2e86de"
                Text {
                    id: chipText
                    anchors.centerIn: parent
                    text: parent.name + " " + parent.quantity
                    color: "white"
                }
            }
        }
    }

    // === The view: a ListView shows the model and creates the delegate only for visible rows ===
    ListView {
        id: list
        x: 20
        y: 110
        width: parent.width - 40
        height: 280
        clip: true                         // cut the items at the border while they are scrolled
        spacing: 6
        model: fruits
        currentIndex: 0

        header: Text {
            height: 28
            text: qsTr("Shopping list (%1 items)").arg(fruits.count)
            font.bold: true
        }

        // The highlight is one item that moves to the current row
        highlight: Rectangle { color: "#fff3cd"; radius: 6 }
        highlightMoveDuration: 150

        // === The delegate: the picture of ONE row. It is created again for every row of the model ===
        delegate: Item {
            id: row
            required property int index             // the number of the row (0, 1, 2 ...)
            required property string name
            required property int quantity
            width: ListView.view.width               // an attached property: the ListView that shows this delegate
            height: 40

            Text {
                x: 12
                anchors.verticalCenter: parent.verticalCenter
                text: row.name
                font.pixelSize: 16
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: "x " + row.quantity
                color: "#555"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: list.currentIndex = row.index
            }
        }

        ScrollBar.vertical: ScrollBar { }
    }

    // === Change the model: the views follow by themselves ===
    Row {
        x: 20
        y: 410
        spacing: 8
        Button {
            text: qsTr("Add")
            onClicked: fruits.append({ name: "Item " + nextNumber, quantity: nextNumber++ })
        }
        Button {
            text: qsTr("More")
            enabled: list.currentIndex >= 0
            onClicked: fruits.setProperty(list.currentIndex, "quantity", fruits.get(list.currentIndex).quantity + 1)
        }
        Button {
            text: qsTr("Remove")
            enabled: list.currentIndex >= 0
            onClicked: fruits.remove(list.currentIndex)
        }
        Button {
            text: qsTr("Move up")
            enabled: list.currentIndex > 0
            onClicked: {
                fruits.move(list.currentIndex, list.currentIndex - 1, 1)
                list.currentIndex -= 1
            }
        }
    }
    Text {
        x: 20
        y: 460
        // currentItem is the delegate of the current row. Its properties are always up to date.
        // (fruits.get(i).name would NOT update when the row is removed: see the README, section 5.)
        text: list.currentItem ? qsTr("Selected: %1").arg(list.currentItem.name) : qsTr("Nothing selected")
    }
}
