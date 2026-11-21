import QtQuick

Window {
    id: root
    width: 520
    height: 480
    visible: true
    title: qsTr("Items and Properties")
    color: "#eef1f5"

    // === Custom properties: "property <type> <name>: <value>" ===
    property string cardTitle: "Project Notes"
    property int itemCount: 12
    property real progress: 0.65
    property color accent: "#2e86de"
    property bool highlighted: true
    readonly property int margin: 24

    // === An enum, and a property that stores one of its values ===
    enum Level {
        Low,
        Medium,
        High
    }
    property int level: Main.Level.Medium

    // === The card: a Rectangle with children ===
    Rectangle {
        id: card
        x: root.margin
        y: root.margin
        width: root.width - 2 * root.margin
        height: 250
        radius: 10
        color: "white"
        border.color: root.highlighted ? root.accent : "#cccccc"
        border.width: 2

        // Image: the file comes from RESOURCES in CMakeLists.txt
        Image {
            id: badge
            source: "badge.png"
            width: 56
            height: 56
            fillMode: Image.PreserveAspectFit
            x: parent.width - width - 16       // "parent" is the card
            y: 16
        }

        Text {
            id: heading
            text: root.cardTitle
            font.pixelSize: 24
            font.bold: true
            color: root.accent
            x: 16
            y: 16
        }


        Text {
            id: subtitle
            text: root.itemCount + " items, level " + levelNames[root.level]
            color: "#555555"
            x: 16
            y: 52

            readonly property var levelNames: ["low", "medium", "high"]
        }

        // Progress bar: a track Rectangle with a fill Rectangle inside
        Rectangle {
            id: track
            x: 16
            y: 110
            width: parent.width - 32
            height: 14
            radius: 7
            color: "#dde3ea"

            Rectangle {
                id: fill
                width: parent.width * root.progress   // "parent" is the track
                height: parent.height
                radius: parent.radius
                color: root.accent
            }
        }

        Text {
            // "id" lets one item read the properties of another item
            text: Math.round(fill.width / track.width * 100) + " % done"
            color: "#555555"
            x: 16
            y: 134
        }

        // Common Item properties: opacity, rotation, scale, visible
        Rectangle {
            x: 16
            y: 175
            width: 60
            height: 40
            color: root.accent
            opacity: 0.4
        }
        Rectangle {
            x: 19
            y: 175
            width: 60
            height: 40
            color: root.accent
            opacity: 0.4
        }
        Rectangle {
            x: 100
            y: 175
            width: 60
            height: 40
            color: root.accent
            rotation: 15
        }
        Rectangle {
            x: 190
            y: 175
            width: 60
            height: 40
            color: root.accent
            scale: 0.6
        }
        Rectangle {
            x: 280
            y: 175
            width: 60
            height: 40
            color: root.accent
            visible: false          // not drawn at all
        }
    }

    // === Stacking order: the "z" property ===
    Item {
        id: stack
        x: root.margin
        y: 300
        width: root.width - 2 * root.margin
        height: 120

        Rectangle {
            x: 0
            y: 0
            width: 90
            height: 90
            color: "#e74c3c"
            z: 1
        }
        Rectangle {
            x: 40
            y: 20
            width: 90
            height: 90
            color: "#27ae60"
            z: 3                    // the highest z is drawn on top
        }
        Rectangle {
            x: 80
            y: 40
            width: 90
            height: 90
            color: "#f1c40f"
            z: 2
        }

        Text {
            text: "z: 1, 3, 2"
            x: 200
            y: 40
            color: "#555555"
        }
    }
}
