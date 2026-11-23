import QtQuick
import States

Window {
    width: 800
    height: 480
    minimumWidth: 560
    minimumHeight: 360

    visible: true
    title: "States_demo"

    Screen01 {
        id: mainScreen
        anchors.fill: parent
    }
}
