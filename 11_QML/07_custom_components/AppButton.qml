import QtQuick

// A reusable button. The file name AppButton.qml makes the type name AppButton (it must start with a capital letter).
Rectangle {
    id: root

    // The public interface of the type: what a user of AppButton can set and use
    property alias text: label.text            // "text" of AppButton is the "text" of the inner Label
    property color baseColor: "#2e86de"
    signal clicked()

    implicitWidth: label.implicitWidth + 32    // the size that the button wants, from its content
    implicitHeight: 36
    radius: 6
    color: !enabled ? "#bdc3c7" : area.pressed ? Qt.darker(baseColor, 1.2) : baseColor
    opacity: enabled ? 1.0 : 0.7

    Text {
        id: label                              // private: other files cannot see this id
        anchors.centerIn: parent
        color: "white"
    }

    MouseArea {
        id: area
        anchors.fill: parent
        enabled: root.enabled
        onClicked: root.clicked()
    }
}
