import QtQuick
import Layout_and_anchors

Window {
    width: 800
    height: 480
    minimumWidth: 560
    minimumHeight: 360

    visible: true
    title: "Layout_and_anchors"

    Screen01 {
        id: mainScreen
        anchors.fill: parent
    }
}
