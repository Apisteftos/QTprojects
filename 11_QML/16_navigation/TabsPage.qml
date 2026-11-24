import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// SwipeView: pages side by side, the user swipes. TabBar: the buttons for the same pages.
Page {
    id: page
    title: qsTr("Tabs")

    property int loadCount: 0     // how often the lazy panel was created

    header: TabBar {
        id: tabBar
        currentIndex: swipe.currentIndex          // the two controls follow each other
        TabButton { text: qsTr("Info") }
        TabButton { text: qsTr("Counter") }
        TabButton { text: qsTr("Lazy") }
    }

    SwipeView {
        id: swipe
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        // Page 1: plain text
        Item {
            Label {
                anchors.centerIn: parent
                text: qsTr("Swipe sideways, or use the tabs")
            }
        }

        // Page 2: it keeps its state while you swipe away (the page stays alive)
        Item {
            property int clicks: 0
            Column {
                anchors.centerIn: parent
                spacing: 8
                Label { text: qsTr("Clicks: %1").arg(parent.parent.clicks) }
                Button { text: qsTr("Click"); onClicked: parent.parent.clicks++ }
            }
        }

        // Page 3: a Loader creates its content only while the page is the current one
        Item {
            id: lazyPage
            Loader {
                id: loader
                anchors.centerIn: parent
                // SwipeView.isCurrentItem is an attached property: it exists only on the direct child
                // of the SwipeView (the Item), so the Loader reads it from there.
                active: lazyPage.SwipeView.isCurrentItem
                sourceComponent: Label {
                    text: qsTr("I am created when you open this tab")
                    color: "#2e86de"
                }
                onLoaded: page.loadCount++
            }
        }
    }

    footer: Label {
        padding: 8
        text: qsTr("The lazy panel was created %1 times").arg(page.loadCount)
        color: "#777"
    }
}
