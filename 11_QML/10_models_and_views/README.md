# 10_models_and_views

A list in a user interface is three things: the **data** (the model), a way to **draw one row** (the delegate), and a **view** that puts the rows on the screen and scrolls them. Qt Quick keeps these three apart, as Qt Widgets do (chapter `07_ModelView`). This lesson uses a `ListModel`, a `ListView` and a `Repeater`.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with two small changes for that old version, which are not part of this folder). An automated check selected rows, changed, moved, added and removed items, emptied the list and added 30 items (20 checks, all passed). With 30 items, only 13 row delegates existed at the same time. Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A shopping list:

- A row of blue **chips** at the top, one per item (a `Repeater`).
- A scrolling **list** of the items with the name on the left and the quantity on the right (a `ListView`), and a yellow highlight on the selected row.
- Buttons: **Add**, **More** (one more of the selected item), **Remove** and **Move up**.
- A text that says which item is selected.

Both views show the **same model**. When you change the model, both change.

---

## 2. The Model: `ListModel`

```qml
ListModel {
    id: fruits
    ListElement { name: "Apple"; quantity: 4 }
    ListElement { name: "Banana"; quantity: 12 }
}
```

A `ListModel` is a list of rows. Each row has **roles**: named values (`name` and `quantity`). The roles of the first row decide the names for all rows.

| Method / property | Meaning |
|-------------------|---------|
| `append({ name: "Fig", quantity: 3 })` | Add a row at the end |
| `insert(index, {...})` | Insert a row |
| `remove(index, count)` | Remove rows |
| `move(from, to, count)` | Move rows |
| `setProperty(index, "quantity", 5)` | Change one role of one row |
| `set(index, {...})` | Replace a row |
| `get(index)` | Read a row as an object |
| `count` | The number of rows |
| `clear()` | Remove all |

Every change sends a signal, and every view that uses the model **updates itself**: only the changed rows are created, removed or moved, and the view can animate it.

Other things can be a model:

| Model | When |
|-------|------|
| A number (`model: 5`) | Five rows, with only `index` |
| A JavaScript array (`model: ["A", "B"]`) | Simple lists, read-only, role `modelData` |
| A `ListModel` | Data that you create and change in QML (this lesson) |
| A model from C++ (`QAbstractListModel`) | Real data: files, a database, a network. Lesson 14 |

`ListModel` is good for prototypes and small lists. For data that comes from C++ code, use a C++ model.

---

## 3. The Delegate

```qml
delegate: Item {
    required property int index
    required property string name
    required property int quantity
    width: ListView.view.width
    height: 40
    Text { text: row.name }
}
```

The delegate is a **template for one row**. The view creates it again for every row, and gives it the data:

- `required property` declares the roles that the delegate wants. This is the recommended way in Qt 6: it is clear, and the tools can check it. (The older way, to use `name` as a bare name inside a delegate without declaring it, still works in simple cases but is not recommended: `qmllint` warns about it, and it fails when the delegate is a separate file.)
- `index` is the number of the row.
- `ListView.view` is an **attached property**: the `ListView` that shows this delegate. The delegate uses it for the width, so it needs no id of the view.
- In the `Repeater` delegate of the lesson, the child `Text` writes `parent.name`. A bare `name` would also be found, but `parent.` (or an `id`) shows where the value comes from.

A delegate is a good place for a **custom component** (lesson 07): `delegate: ContactCard { }` keeps the list file short.

---

## 4. The Views

### 4.1 `ListView`

A `ListView` shows rows one under the other (or side by side with `orientation: ListView.Horizontal`). It scrolls, and it **creates delegates only for the rows that are visible** (and a few more around them). The lesson adds 30 items, and the test found 13 delegates alive: the others do not exist until you scroll to them. So a list with 100 000 rows is as fast as one with 10.

Important properties:

| Property | Meaning |
|----------|---------|
| `model`, `delegate` | The two required parts |
| `currentIndex`, `currentItem` | The selected row and its delegate |
| `highlight` | An item that moves to the current row |
| `header`, `footer` | An item before the first and after the last row |
| `spacing`, `clip` | The space between rows; cut the content at the border. **Set `clip: true`** or the rows are seen outside |
| `section.property` | Group the rows with headings |
| `ScrollBar.vertical: ScrollBar { }` | A scroll bar (Qt Quick Controls) |

Important: **a delegate is created and deleted while you scroll.** It must not keep state that cannot be lost. State belongs into the **model**.

### 4.2 `Repeater`

A `Repeater` creates **all** items at once, and it has no scrolling. Use it for a small number of items that are placed by a positioner (`Row`, `Column`, `Flow`, `Grid`) or a layout: the chips, the buttons of a tab bar, the cells of a calendar. Use `ListView` for anything long or scrolling.

Other views: `GridView` (a grid of cells), `PathView` (items on a path), `TableView` (rows and columns, lesson 14 and Qt documentation).

---

## 5. A Trap: `get()` Does Not Notify

The text "Selected: ..." first used `fruits.get(list.currentIndex).name`. It showed the wrong name after **Remove**: the removed item was still named, because the binding depends on `list.currentIndex`, and that did not change (the index was still 0, but another row had moved there). `get()` returns the data of **one moment**, and QML cannot see when it changes later.

Two good solutions:

- **`list.currentItem.name`**: the delegate is always updated by the view (the version in the lesson).
- A **role in a delegate** or a model that notifies (`QAbstractListModel` from C++ sends `dataChanged`).

Use `get()` in handlers, where you want the value of the moment (like the **More** button does), and not in bindings.

---

## 6. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app10_models_and_views
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Click a row, then **More**, **Move up** and **Remove**. Watch the chips at the top at the same time.
2. Click **Add** many times, until the list scrolls. Scroll with the mouse wheel.
3. Add a `footer` to the `ListView`: a `Text` that says "End of list".
4. Add `section.property: "name"` and `section.criteria: ViewSection.FirstCharacter` with a `section.delegate`. (Qt documentation: `ListView.section`.)
5. Add a third property `bought: false` to the first `ListElement`s and show it as a check mark in the delegate.
6. Replace `highlight` by a color that depends on `ListView.isCurrentItem` inside the delegate.

---

## 7. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The list is empty | `model` or `delegate` is missing, or the delegate has no size | Give the delegate `width` and `height` |
| `ReferenceError: name is not defined` | The delegate does not declare the role | `required property string name` |
| All rows are on top of each other | The delegate has no height | `height: 40` |
| The rows are seen outside of the `ListView` | `clip` is `false` | `clip: true` |
| An error about a script in a `ListElement` | `ListElement` accepts only simple values (numbers, strings, bools), not expressions or bindings | Use `append` in code for computed values |
| A value in the view is old | `get()` was used in a binding | `currentItem`, or a role of the delegate |
| `currentIndex` is `-1` after the last row is removed | There is no current row | Test `currentIndex >= 0` before using it |
| Scrolling is slow | The delegate is heavy, or it loads pictures without limit | Make the delegate simple; set `sourceSize` of images; use `cacheBuffer` |

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Models and views in Qt Quick | <https://doc.qt.io/qt-6/qtquick-modelviewsdata-modelview.html> |
| `ListModel` | <https://doc.qt.io/qt-6/qml-qtqml-models-listmodel.html> |
| `ListView` | <https://doc.qt.io/qt-6/qml-qtquick-listview.html> |
| `Repeater` | <https://doc.qt.io/qt-6/qml-qtquick-repeater.html> |
| Required properties | <https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html#required-properties> |
| `ScrollBar` | <https://doc.qt.io/qt-6/qml-qtquick-controls-scrollbar.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`); `ListModel` is in `QtQml.Models`, which `import QtQuick` includes |
| Imports | `import QtQuick`, `import QtQuick.Controls` |
| Minimum Qt version | Qt 6.8 |
