/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

// The file Design Studio generated, changed into the About screen of Workbench.
// The button and the color animation of the original are kept.
// It does not use Constants: that file needs the Qt Quick Studio components, which
// a normal Qt install does not have, so the app would not start in Qt Creator.

import QtQuick
import QtQuick.Controls

Rectangle {
    id: rectangle
    width: 420
    height: 260

    color: "#ffffff"
    radius: 14
    border.color: "#c9d6cf"
    border.width: 2

    Column {
        id: column
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8

        Text {
            id: titleText
            text: qsTr("Workbench")
            font.pixelSize: 30
            font.bold: true
            color: "#1d4d38"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: versionText
            text: qsTr("Version 1.0")
            font.pixelSize: 16
            color: "#5b6b63"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: lineText
            text: qsTr("Qt Quick on the outside, a C++23 library on the inside")
            font.pixelSize: 14
            color: "#1d4d38"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Button {
        id: button
        text: qsTr("Press me")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        checkable: true
        anchors.horizontalCenter: parent.horizontalCenter

        Connections {
            target: button
            onClicked: animation.start()
        }
    }

    Text {
        id: label
        text: qsTr("Hello Workbench_app")
        color: "#5b6b63"
        anchors.top: button.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter

        SequentialAnimation {
            id: animation

            ColorAnimation {
                id: colorAnimation1
                target: rectangle
                property: "color"
                to: "#cfe8da"
                from: "#ffffff"
            }

            ColorAnimation {
                id: colorAnimation2
                target: rectangle
                property: "color"
                to: "#ffffff"
                from: "#cfe8da"
            }
        }
    }
    states: [
        State {
            name: "clicked"
            when: button.checked

            PropertyChanges {
                target: label
                text: qsTr("Button Checked")
            }
        }
    ]
}
