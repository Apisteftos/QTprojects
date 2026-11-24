import QtQuick

// A card with a title. Everything that is written inside "Card { ... }" is put into the body.
Rectangle {
    id: root

    property alias title: titleText.text
    default property alias content: body.data   // the "default property": children of Card go here
    property int padding: 12

    implicitWidth: 300
    implicitHeight: titleText.height + body.childrenRect.height + 2 * padding + 8
    radius: 8
    color: "white"
    border.color: "#cbd5e1"

    Text {
        id: titleText
        x: root.padding
        y: root.padding
        font.bold: true
        font.pixelSize: 16
    }

    Item {
        id: body
        x: root.padding
        y: titleText.y + titleText.height + 8
        width: root.width - 2 * root.padding
        height: childrenRect.height
    }
}
