import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 420
    height: 560
    visible: true
    title: qsTr("Navigation")

    // === The frame: a header with a menu button, a back button and the title of the current page ===
    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: stack.depth > 1 ? "‹" : "☰"
                font.pixelSize: 20
                onClicked: stack.depth > 1 ? stack.pop() : drawer.open()   // back, or the menu
            }
            Label {
                // currentItem is the page on top of the stack. "title" is a property of Page.
                text: stack.currentItem ? stack.currentItem.title : ""
                font.bold: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
            Item { implicitWidth: 40 }                                     // balances the button on the left
        }
    }

    // === StackView: pages on top of each other. push() opens a page, pop() goes back ===
    StackView {
        id: stack
        anchors.fill: parent
        initialItem: HomePage {
            onOpenDetail: name => stack.push(detailComponent, { itemName: name })
            onOpenTabs: stack.push(tabsComponent)
        }
    }

    // === Component: a page is created when it is pushed, and destroyed when it is popped ===
    Component {
        id: detailComponent
        DetailPage {
            onDeeper: stack.push(detailComponent, { itemName: itemName + "+", level: level + 1 })
            onBackToStart: stack.pop(null)       // pop(null) removes everything except the first page
        }
    }
    Component {
        id: tabsComponent
        TabsPage { }
    }

    // === Drawer: a panel that slides in from the edge ===
    Drawer {
        id: drawer
        width: root.width * 0.7
        height: root.height

        Column {
            anchors.fill: parent
            Label { text: qsTr("Menu"); font.bold: true; padding: 16 }
            ItemDelegate {
                width: parent.width
                text: qsTr("Home")
                onClicked: { stack.pop(null); drawer.close() }
            }
            ItemDelegate {
                width: parent.width
                text: qsTr("Tabs")
                onClicked: {
                    stack.pop(null)
                    stack.push(tabsComponent)
                    drawer.close()
                }
            }
        }
    }
}
