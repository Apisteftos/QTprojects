import QtQuick
import Multi_screen_flow

Window {
    width: 800
    height: 480
    minimumWidth: 560
    minimumHeight: 360

    visible: true
    title: "Multi_screen_flow"

    Screen01 {
        id: mainScreen
        anchors.fill: parent
    }
}
