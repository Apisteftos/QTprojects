import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

// Tab 2: play a video file.
Item {
    id: root

    property var backend
    property var theme
    property var appState
    property string fileName: ""

    signal openRequested

    function load(url) {
        const path = backend.localPath(url)
        fileName = backend.fileName(path)
        player.source = url
        player.play()
        backend.addRecent(path)
        appState.statusText = fileName
    }

    // True when the computer has an audio output. Without one the video plays silently,
    // and no AudioOutput is created (it would print "No audio device detected").
    readonly property bool hasAudio: mediaDevices.audioOutputs.length > 0

    MediaDevices {
        id: mediaDevices
    }

    Component {
        id: audioComponent

        AudioOutput {
            volume: volumeSlider.value
        }
    }

    Component.onCompleted: {
        if (hasAudio)
            player.audioOutput = audioComponent.createObject(root)
    }

    MediaPlayer {
        id: player
        videoOutput: videoOutput
        playbackRate: Number(speedBox.currentText)
        loops: loopSwitch.checked ? MediaPlayer.Infinite : 1
        onErrorOccurred: (error, errorString) => appState.statusText = errorString
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 14

        RowLayout {
            spacing: 14

            Button {
                text: qsTr("Open…")
                highlighted: true
                onClicked: root.openRequested()
            }

            Label {
                text: root.fileName === "" ? qsTr("No video open") : root.fileName
                color: theme.text
                font.bold: root.fileName !== ""
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#10201a"
            radius: 12

            VideoOutput {
                id: videoOutput
                anchors.fill: parent
                anchors.margins: 6
                fillMode: VideoOutput.PreserveAspectFit
            }

            Label {
                anchors.centerIn: parent
                text: qsTr("Open a video with File > Open Video…")
                color: "#9bb0a5"
                visible: player.source == ""
            }
        }

        RowLayout {
            spacing: 12

            Label {
                text: backend.formatClock(Math.floor(player.position / 1000))
                color: theme.muted
            }

            Slider {
                id: positionSlider
                Layout.fillWidth: true
                from: 0
                to: Math.max(1, player.duration)
                value: player.position
                enabled: player.seekable
                onMoved: player.position = value
            }

            Label {
                text: backend.formatClock(Math.floor(player.duration / 1000))
                color: theme.muted
            }
        }

        RowLayout {
            spacing: 20

            Button {
                id: playButton
                text: checked ? qsTr("Pause") : qsTr("Play")
                checkable: true
                enabled: player.source != ""
                highlighted: checked
                onToggled: checked ? player.play() : player.pause()
            }

            // A click changes `checked`, which would remove the binding to the
            // player. This Binding keeps the button in step with the real state.
            Binding {
                target: playButton
                property: "checked"
                value: player.playbackState === MediaPlayer.PlayingState
            }

            Label {
                text: root.hasAudio ? qsTr("Volume") : qsTr("No audio device")
                color: theme.muted
            }

            Slider {
                id: volumeSlider
                from: 0
                to: 1
                value: 0.7
                implicitWidth: 140
                enabled: root.hasAudio
            }

            Label {
                text: qsTr("Speed")
                color: theme.muted
            }

            ComboBox {
                id: speedBox
                model: ["0.5", "1.0", "1.5", "2.0"]
                currentIndex: 1
                displayText: currentText + "×"
            }

            Switch {
                id: loopSwitch
                text: qsTr("Loop")
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }
}
