import QtQuick

Window {
    id: root
    width: 520
    height: 520
    visible: true
    title: qsTr("Signals and Handlers")
    color: "#eef1f5"

    // === A button type with its own signal ===
    // The MouseArea has a signal "clicked". The Btn forwards it as its own signal, so the code
    // that uses a Btn does not need to know that there is a MouseArea inside.
    component Btn: Rectangle {
        id: btn
        property string label: ""
        signal clicked()               // a custom signal, declared with "signal name(arguments)"
        width: 110
        height: 40
        radius: 6
        color: area.pressed ? "#1b6ec2" : "#2e86de"
        Text {
            anchors.centerIn: parent
            text: btn.label
            color: "white"
        }
        MouseArea {
            id: area
            anchors.fill: parent
            onClicked: btn.clicked()   // a signal can be emitted by calling it like a function
        }
    }

    // === The data: a counter with a limit and a signal that says "the limit is reached" ===
    QtObject {
        id: counter
        property int value: 0
        readonly property int limit: 5
        signal limitReached(int limit)

        function increment() {
            if (value >= limit) {
                limitReached(limit)    // emit the signal
                return
            }
            value += 1
        }
        function reset() {
            value = 0
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Text {
            id: valueText
            text: qsTr("Count: %1").arg(counter.value)
            font.pixelSize: 36
        }

        // === 1. A signal handler: "on" + signal name with a capital letter ===
        Row {
            spacing: 10
            Btn { label: "+1"; onClicked: counter.increment() }
            Btn { label: "-1"; onClicked: counter.value = Math.max(0, counter.value - 1) }
            Btn { label: "Reset"; onClicked: counter.reset() }
        }

        // === 2. Connections: react to a signal of an object that is not the parent ===
        Text {
            id: status
            text: qsTr("Status: ok")
            color: "#555"
        }
        Connections {
            target: counter
            // In Qt 6, the handler is a function with the name "on" + signal and the same arguments
            function onLimitReached(limit) {
                status.text = qsTr("Status: the limit %1 is reached").arg(limit)
            }
            function onValueChanged() {          // every property has a change signal: <name>Changed
                if (counter.value < counter.limit)
                    status.text = qsTr("Status: ok")
            }
        }

        // === 3. TapHandler: a modern way to react to taps and touches ===
        Rectangle {
            id: tapBox
            width: 240
            height: 60
            radius: 6
            color: "#27ae60"
            property int taps: 0
            property int doubleTaps: 0
            Text {
                anchors.centerIn: parent
                color: "white"
                text: qsTr("Tap me: %1 taps, %2 double").arg(tapBox.taps).arg(tapBox.doubleTaps)
            }
            TapHandler {
                onTapped: tapBox.taps += 1
                onDoubleTapped: tapBox.doubleTaps += 1
            }
        }

        // === 4. Keys: keyboard events go to the item that has the focus ===
        Rectangle {
            id: keyBox
            width: 240
            height: 60
            radius: 6
            color: activeFocus ? "#e67e22" : "#bdc3c7"     // "activeFocus" is a property: this is a binding
            focus: true                                      // ask for the focus at the start
            Text {
                anchors.centerIn: parent
                text: keyBox.activeFocus ? qsTr("Keys: Up, Down, Space") : qsTr("Click here for the focus")
            }
            Keys.onUpPressed: counter.increment()
            Keys.onDownPressed: counter.value = Math.max(0, counter.value - 1)
            Keys.onSpacePressed: counter.reset()
            MouseArea {
                anchors.fill: parent
                onClicked: keyBox.forceActiveFocus()
            }
        }

        // === 5. Timer: a signal "triggered" at a regular interval ===
        Row {
            spacing: 10
            Btn {
                label: timer.running ? qsTr("Stop timer") : qsTr("Start timer")
                onClicked: timer.running = !timer.running
            }
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("+1 every second")
            }
        }
        Timer {
            id: timer
            interval: 1000
            repeat: true
            onTriggered: counter.increment()
        }
    }
}
