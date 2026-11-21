/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick
import QtQuick.Controls
import Scratch_ui

Rectangle {
    id: rectangle
    width: Constants.width
    height: Constants.height

    color: Constants.backgroundColor

    Text {
        id: label
        text: qsTr("Hello Scratch_ui")
        font.family: Constants.font.family
        anchors.topMargin: 45
        anchors.horizontalCenter: parent.horizontalCenter

        SequentialAnimation {
            id: animation

            ColorAnimation {
                id: colorAnimation1
                target: rectangle
                property: "color"
                to: "#2294c6"
                from: Constants.backgroundColor
            }

            ColorAnimation {
                id: colorAnimation2
                target: rectangle
                property: "color"
                to: Constants.backgroundColor
                from: "#2294c6"
            }
        }
    }

    Rectangle {
        id: rectangle1
        x: 873
        y: 406
        width: 157
        height: 155
        color: "#ffffff"
        radius: 10
        border.color: "#ff0000"

        TextInput {
            id: textInput
            x: 1
            y: 0
            width: 157
            height: 23
            text: qsTr("The rise up of electronics")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
        }

        TextEdit {
            id: textEdit
            x: 39
            y: 40
            width: 79
            height: 70
            text: qsTr("- goals\n- memory\n- AI definition\n- Philosophy")
            font.pixelSize: 12
        }
    }

    Image {
        id: landscape
        x: 708
        y: 600
        width: 505
        height: 384
        visible: true
        source: "images/landscape.png"
        fillMode: Image.PreserveAspectFit

        Rectangle {
            id: rectangle2
            x: 153
            y: 98
            width: 200
            height: 200
            color: "#ffffff"
        }

        Rectangle {
            id: rectangle3
            x: 192
            y: 148
            width: 122
            height: 100
            color: "#ffffff"
        }
    }
    states: [
        State {
            name: "clicked"

            PropertyChanges {
                target: label
                text: qsTr("Button Checked")
            }
        }
    ]
}
