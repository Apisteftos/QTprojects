import QtQuick
import QtQuick.Controls

Window {
    width: 480
    height: 520
    visible: true
    title: qsTr("Animations")
    color: "#eef1f5"

    // === 1. An animation "on" a property: it starts together with the item and runs for ever ===
    Text {
        x: 20
        y: 14
        text: qsTr("1. Animation on a property (a sequence that repeats)")
        font.bold: true
    }
    Rectangle {
        id: track
        x: 20
        y: 40
        width: 440
        height: 40
        radius: 6
        color: "white"

        Rectangle {
            id: ball
            y: 6
            width: 28
            height: 28
            radius: 14
            color: "#2e86de"

            // SequentialAnimation runs its children one after the other. "on x" means: animate the property x.
            SequentialAnimation on x {
                id: sweep
                loops: Animation.Infinite
                NumberAnimation { from: 6; to: track.width - 34; duration: 1000; easing.type: Easing.InOutQuad }
                NumberAnimation { from: track.width - 34; to: 6; duration: 1000; easing.type: Easing.InOutQuad }
            }
        }
    }

    // === 2. An animation with an id, controlled by buttons ===
    Text {
        x: 20
        y: 110
        text: qsTr("2. Start, pause, resume and stop")
        font.bold: true
    }
    Item {
        x: 20
        y: 140
        width: 80
        height: 80

        Rectangle {
            id: pulse
            anchors.centerIn: parent
            width: 40
            height: 40
            radius: 20
            color: "#27ae60"
        }

        // ParallelAnimation runs its children at the same time. The sequence goes bigger and then smaller again.
        SequentialAnimation {
            id: pulseAnim
            loops: Animation.Infinite
            ParallelAnimation {
                NumberAnimation { target: pulse; property: "scale"; from: 1.0; to: 1.8; duration: 600; easing.type: Easing.OutQuad }
                NumberAnimation { target: pulse; property: "opacity"; from: 1.0; to: 0.3; duration: 600 }
            }
            ParallelAnimation {
                NumberAnimation { target: pulse; property: "scale"; to: 1.0; duration: 600; easing.type: Easing.InQuad }
                NumberAnimation { target: pulse; property: "opacity"; to: 1.0; duration: 600 }
            }
        }
    }
    Row {
        x: 120
        y: 160
        spacing: 8
        Button { text: qsTr("Start"); onClicked: pulseAnim.start() }
        Button { text: qsTr("Pause"); onClicked: pulseAnim.pause() }
        Button { text: qsTr("Resume"); onClicked: pulseAnim.resume() }
        Button { text: qsTr("Stop"); onClicked: pulseAnim.stop() }
    }
    Text {
        x: 120
        y: 210
        text: pulseAnim.running ? (pulseAnim.paused ? qsTr("paused") : qsTr("running")) : qsTr("stopped")
    }

    // === 3. ColorAnimation: the same as NumberAnimation, for colors ===
    Text {
        x: 20
        y: 250
        text: qsTr("3. ColorAnimation")
        font.bold: true
    }
    Rectangle {
        id: lamp
        x: 20
        y: 276
        width: 440
        height: 36
        radius: 6
        color: "#27ae60"

        SequentialAnimation on color {
            loops: Animation.Infinite
            ColorAnimation { to: "#e67e22"; duration: 800 }
            ColorAnimation { to: "#c0392b"; duration: 800 }
            ColorAnimation { to: "#27ae60"; duration: 800 }
        }
    }

    // === 4. An animation that starts when something happens, with different easing ===
    Text {
        x: 20
        y: 340
        text: qsTr("4. Easing curves")
        font.bold: true
    }
    Rectangle {
        id: lane
        x: 20
        y: 366
        width: 440
        height: 40
        radius: 6
        color: "white"
        Rectangle {
            id: box
            x: 6
            y: 6
            width: 28
            height: 28
            radius: 4
            color: "#8e44ad"
        }
    }
    NumberAnimation {
        id: move
        target: box
        property: "x"
        from: 6
        to: lane.width - 34
        duration: 1200
    }
    Row {
        x: 20
        y: 420
        spacing: 8
        Button { text: qsTr("Linear"); onClicked: { move.easing.type = Easing.Linear; move.restart() } }
        Button { text: qsTr("OutBounce"); onClicked: { move.easing.type = Easing.OutBounce; move.restart() } }
        Button { text: qsTr("OutElastic"); onClicked: { move.easing.type = Easing.OutElastic; move.restart() } }
        Button { text: qsTr("InOutCubic"); onClicked: { move.easing.type = Easing.InOutCubic; move.restart() } }
    }
}
