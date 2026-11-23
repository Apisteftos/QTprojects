import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Rectangle {
        id: panel
        width: 680
        height: 380
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Text {
            id: titleText
            x: 32
            y: 24
            text: "Settings"
            font.pixelSize: 28
            color: "#1d4d38"
        }

        Column {
            id: leftColumn
            x: 32
            y: 84
            width: 280
            spacing: 18

            Switch {
                id: notifySwitch
                text: "Notifications"
                checked: true
            }

            CheckBox {
                id: agreeCheck
                text: "I agree to the terms"
            }

            ComboBox {
                id: modeBox
                width: 240
                model: ["Quiet", "Normal", "Loud"]
                currentIndex: 1
            }
        }

        Column {
            id: rightColumn
            x: 352
            y: 84
            width: 296
            spacing: 18

            Text {
                id: volumeText
                text: "Volume: " + Math.round(volumeSlider.value)
                font.pixelSize: 18
                color: "#1d4d38"
            }

            Slider {
                id: volumeSlider
                width: 280
                from: 0
                to: 100
                stepSize: 1
                value: 40
            }

            TextField {
                id: nameField
                width: 280
                placeholderText: "Your name"
            }

            Button {
                id: saveButton
                text: "Save"
                enabled: agreeCheck.checked
            }
        }

        Text {
            id: summaryText
            x: 32
            y: 318
            width: 616
            text: (notifySwitch.checked ? "Notifications on" : "Notifications off")
                  + ", mode " + modeBox.currentText
                  + ", volume " + Math.round(volumeSlider.value)
            font.pixelSize: 16
            color: "#5b6b63"
        }
    }
}
