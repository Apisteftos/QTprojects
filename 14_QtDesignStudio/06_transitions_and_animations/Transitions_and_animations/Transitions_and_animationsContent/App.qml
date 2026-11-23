import QtQuick
import Transitions_and_animations

Window {
    width: 800
    height: 480
    minimumWidth: 560
    minimumHeight: 360

    visible: true
    title: "Transitions_demo"

    Screen01 {
        id: mainScreen
        anchors.fill: parent
    }
}
