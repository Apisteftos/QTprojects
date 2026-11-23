import QtQuick
import Designer_developer_handoff

Window {
    width: 800
    height: 480
    minimumWidth: 560
    minimumHeight: 360
    visible: true
    title: "Designer_developer_handoff"

    Backend {
        id: backend
    }

    Screen01 {
        id: mainScreen
        anchors.fill: parent
        level: backend.level
        statusText: backend.status
    }

    Connections {
        target: mainScreen.startArea
        function onClicked() {
            backend.start()
        }
    }

    Connections {
        target: mainScreen.stopArea
        function onClicked() {
            backend.stop()
        }
    }
}
