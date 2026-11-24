# 16_navigation

An app with more than one screen needs a way to move between them. Qt Quick Controls has five tools for this: **StackView** (pages on top of each other), **SwipeView** with **TabBar** (pages side by side), **Drawer** (a side menu) and **Loader** (create content only when it is needed). This lesson uses all five in one small app.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder). An automated check clicked through the window: push and pop, the back button, the tabs, the lazy panel and the drawer (31 checks, all passed). Animations were only run offscreen, so please build it once with your own 6.8+ kit and look at them.

---

## 1. What You Build

| File | Content |
|------|---------|
| `Main.qml` | The frame: window, header with a menu/back button, `StackView`, `Drawer` |
| `HomePage.qml` | First page, three buttons |
| `DetailPage.qml` | A page with data (`itemName`, `level`) that can open itself again |
| `TabsPage.qml` | `TabBar` + `SwipeView` with three pages, one of them lazy |

---

## 2. StackView: Pages on Top of Each Other

```qml
StackView {
    id: stack
    anchors.fill: parent
    initialItem: HomePage { }
}
```

| Call | Effect |
|------|--------|
| `stack.push(component, { itemName: "Alpha" })` | Creates the page and shows it. The second argument sets its properties |
| `stack.pop()` | Removes the top page (back) |
| `stack.pop(null)` | Removes all pages except the first |
| `stack.replace(component)` | Swaps the top page |
| `stack.depth` | Number of pages. `1` means: nothing to go back to |
| `stack.currentItem` | The page on top |

The back button in `Main.qml` uses `depth` to choose its job: with `depth > 1` it calls `pop()`, otherwise it opens the drawer.

### 2.1 Pages as Components

A page is created **when it is pushed** and destroyed when it is popped, so it needs a `Component` (or the file name as a `url`):

```qml
Component { id: detailComponent; DetailPage { } }
...
stack.push(detailComponent, { itemName: name })
```

`required property string itemName` in `DetailPage` forces the caller to give the value. The page has no idea who opens it.

### 2.2 Pages Send Signals, the Frame Navigates

`HomePage` has `signal openDetail(string name)`, and it does not know the `StackView`. `Main.qml` connects the signal. This way a page can be tested alone and used in another app. A page that calls `stack.push` directly is tied to one frame.

---

## 3. SwipeView and TabBar

```qml
header: TabBar {
    currentIndex: swipe.currentIndex
    TabButton { text: "Info" }
    ...
}
SwipeView {
    id: swipe
    currentIndex: tabBar.currentIndex
}
```

Two controls, one number. Each binds to the other, so a swipe moves the tab and a click on a tab moves the page. (This is the one place where two bindings pointing at each other are correct: both stay in sync, and no loop runs, because a binding that is set to the same value does nothing.)

Every direct child of a `SwipeView` is a page. The pages **stay alive** while you swipe away: the Counter page keeps its number. The test clicked twice, went to another tab and came back, and the number was still 2.

---

## 4. Loader: Create It When It Is Needed

A `Loader` makes its content from `sourceComponent` (or `source`, a file) while `active` is `true`. When `active` becomes `false`, the content is destroyed.

```qml
Item {
    id: lazyPage
    Loader {
        active: lazyPage.SwipeView.isCurrentItem
        sourceComponent: Label { text: "I am created when you open this tab" }
    }
}
```

The footer counts how often the content was created: it is `0` at the start, `1` after the first visit, `2` after the second. Use a `Loader` for pages that are heavy (a big list, a map, a video), so that the app starts fast and does not keep them in memory.

### 4.1 Attached Properties

`SwipeView.isCurrentItem` is an **attached property**: the `SwipeView` adds it to its direct children. It exists only there. The first version of this lesson wrote `active: SwipeView.isCurrentItem` inside the `Loader`, which is a child of the page, not of the `SwipeView`. The property was never `true`, and the panel was never created. The fix is to read it from the page: `lazyPage.SwipeView.isCurrentItem`.

---

## 5. Drawer

```qml
Drawer {
    id: drawer
    width: root.width * 0.7
    height: root.height
    ...
}
```

`drawer.open()` and `drawer.close()` show and hide it. The user can also pull it from the edge. It needs a `height` (and a `width`), because it is not inside a layout. The drawer here jumps to a page with `stack.pop(null)` and `stack.push(...)`, so that the stack does not grow with every menu click.

---

## 6. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app16_navigation
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Open "Details: Alpha", press "Go deeper" three times, and watch the title and the level. Then press "Back to the start".
2. Change `pop(null)` to `pop()` in `onBackToStart`. What is different?
3. Add a `popEnter` / `pushEnter` transition to the `StackView` (see the Reference).
4. In `TabsPage.qml`, change the Lazy page so that the content is `source: "OtherPage.qml"`, a new file.
5. Set `interactive: false` on the `SwipeView`. The tabs still work, but swiping does not.
6. Add a fourth page and a fourth `TabButton`.

---

## 7. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The Loader never shows its content | `SwipeView.isCurrentItem` was read in a child, not in the direct child | Use `pageId.SwipeView.isCurrentItem` |
| `stack.push` shows a blank page | The component has an error (see the console), or it has no size | Check the output; pages in a `StackView` fill it by default |
| "required property" error when pushing | The initial values were not given | `push(component, { name: value })` |
| The back button does nothing on the first page | `depth` is 1 | Open the menu instead, as in this lesson |
| The pages in the stack keep old data | Pages stay alive below the top one | Reset them in `StackView.onActivated`, or use `replace` |
| Two pages answer a click | An invisible page below still gets events | Pages in a `StackView` are hidden while they are below; hide yours too if you use your own container |
| The tabs and the swipe do not match | Only one of the two `currentIndex` bindings was written | Bind both, as in section 3 |
| The drawer is empty or has no size | `width` and `height` are missing | Set both |
| The state of a page is lost | The page was popped, or a `Loader` became inactive | Keep the state in C++ or in a parent that stays alive |

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| `StackView` | <https://doc.qt.io/qt-6/qml-qtquick-controls-stackview.html> |
| `SwipeView`, `TabBar`, `TabButton` | <https://doc.qt.io/qt-6/qml-qtquick-controls-swipeview.html>, <https://doc.qt.io/qt-6/qml-qtquick-controls-tabbar.html> |
| `Drawer` | <https://doc.qt.io/qt-6/qml-qtquick-controls-drawer.html> |
| `Loader` | <https://doc.qt.io/qt-6/qml-qtquick-loader.html> |
| `Page`, `ApplicationWindow`, `ToolBar` | <https://doc.qt.io/qt-6/qml-qtquick-controls-page.html> |
| Attached properties | <https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html#attached-properties-and-attached-signal-handlers> |
| Module | Qt Quick Controls (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
