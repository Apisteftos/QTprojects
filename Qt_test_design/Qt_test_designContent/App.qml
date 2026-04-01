import QtQuick
import Qt_test_design

Window {
    width: mainScreen.width
    height: mainScreen.height

    visible: true
    title: "Qt_test_design"

    Screen01 {
        id: mainScreen

        anchors.centerIn: parent
    }

}

