import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ============================================================
// Main.qml — Root application window
//
// Navigation pattern: TabBar + SwipeView
//   - TabBar at bottom (like mobile apps)
//   - SwipeView holds all pages
//   - Pages can also be swiped between
//
// Alert overlay — appears on top of everything when
// dataProvider.hasAlert is true
// ============================================================
ApplicationWindow {
    id: root
    width:   900
    height:  620
    visible: true
    title:   "Qt QML Dashboard"

    // ── Tab navigation ─────────────────────────────────────
    SwipeView {
        id: swipeView
        anchors.fill: parent
        anchors.bottomMargin: tabBar.height
        currentIndex: tabBar.currentIndex

        DashboardPage {}
        ChartsPage    {}
        SettingsPage  {}
        AboutPage     {}
    }

    // ── Bottom tab bar ─────────────────────────────────────
    TabBar {
        id: tabBar
        anchors.bottom: parent.bottom
        anchors.left:   parent.left
        anchors.right:  parent.right
        currentIndex:   swipeView.currentIndex

        TabButton { text: "📊 Dashboard" }
        TabButton { text: "📈 Charts"    }
        TabButton { text: "⚙️ Settings"  }
        TabButton { text: "ℹ️ About"     }
    }

    // ── Alert overlay ──────────────────────────────────────
    // Sits on top of everything — appears with animation
    Rectangle {
        id: alertBanner
        anchors.top:   parent.top
        anchors.left:  parent.left
        anchors.right: parent.right
        height: dataProvider.hasAlert ? 60 : 0
        color: "#c62828"

        // Smooth height animation
        Behavior on height {
            NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
        }

        visible: height > 0

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12

            Text {
                text: "⚠ " + dataProvider.alertMessage
                color: "white"
                font.pixelSize: 14
                font.bold: true
                Layout.fillWidth: true
                elide: Text.ElideRight
            }

            Button {
                text: "Dismiss"
                flat: true
                onClicked: dataProvider.dismissAlert()
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font.pixelSize: 13
                }
            }
        }
    }
}
