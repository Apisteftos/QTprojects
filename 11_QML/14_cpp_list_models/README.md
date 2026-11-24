# 14_cpp_list_models

For a short list in QML, a `ListModel` is enough (lesson 10). For real data (items that C++ owns, loads from a file or a database, and changes by its own rules) you write the model in **C++**, as a `QAbstractListModel`. QML views use it exactly like a `ListModel`. This lesson builds a to-do list.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; one is an explicit `import cpp_list_models` in `Main.qml`, which Qt 6.8 does not need). An automated check tested `TodoModel` in C++ (data, roles, signals, add, remove, edit, clear), ran Qt's own `QAbstractItemModelTester` on it, and clicked through the window (37 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

- A heading "2 of 3 tasks left" that follows the model.
- A text field and an **Add** button (also the Enter key). **Add** is off while the field is empty.
- A `ListView` with one row for each task: a check box, the title (crossed out when done) and a **×** button that removes the row.
- A button **Clear finished tasks**.

The model has three tasks at the start. The data lives in C++ (`QList<Item>`), and QML holds no copy of it.

---

## 2. A List Model in Four Functions

`QAbstractListModel` is the base class of C++ models for lists. You must implement:

| Function | Job |
|----------|-----|
| `rowCount(parent)` | The number of rows. For a list: `parent.isValid() ? 0 : count()` (only the invisible root has rows) |
| `data(index, role)` | The value of one row for one **role** |
| `roleNames()` | The names of the roles. These become the names in the QML delegate |
| *(editable)* `setData(index, value, role)` and `flags(index)` | Change the data from a view |

### 2.1 Roles

```cpp
enum Roles { TitleRole = Qt::UserRole + 1, DoneRole };

QHash<int, QByteArray> TodoModel::roleNames() const
{
    return { {TitleRole, "title"}, {DoneRole, "done"} };
}
```

A role is one piece of data of a row. Your own roles start at `Qt::UserRole + 1`, and `roleNames()` gives them names for QML. The delegate then declares

```qml
required property string title
required property bool done
required property int index
```

`index` is always there. The names must **not** be properties of the delegate item (lesson 13, section 7).

### 2.2 `data()`

`data()` is called by the view whenever it needs a value. It must be **fast** and **without side effects**, and it must check its arguments (`index.isValid()`, the row range): a view can ask for a row that has just been removed.

---

## 3. Telling the Views About Changes

A view cannot see what you do to your list. You must tell it, with the **exact** signals. Qt provides helper functions:

| Change | Code |
|--------|------|
| A row is added | `beginInsertRows(QModelIndex(), row, row); ...change...; endInsertRows();` |
| A row is removed | `beginRemoveRows(QModelIndex(), row, row); ...change...; endRemoveRows();` |
| The value of a row changed | `emit dataChanged(index, index, {DoneRole});` |
| Everything is different | `beginResetModel(); ...; endResetModel();` (the view starts again, and loses its scroll position) |
| Rows are moved | `beginMoveRows(...)` and `endMoveRows()` |

The `begin...` call must come **before** the change and the `end...` call after it. The view uses this to animate the change and to keep its current item right. A change without the calls leads to wrong rows, crashes or old data in the view. The last argument of `dataChanged` lists the roles that changed, so the view updates only what is needed.

Test it with Qt's own checker. **`QAbstractItemModelTester`** (Qt Test) checks a model while you use it, and reports every mistake. In the test of this lesson it found a real fault in the first version: `flags()` returned `ItemIsEditable` also for the **invisible root item**. The fix is in `flags()`: for an invalid index, return the flags of the base class unchanged. Use it for every model you write.

---

## 4. Properties of the Model

```cpp
Q_PROPERTY(int count READ count NOTIFY countChanged)
Q_PROPERTY(int remaining READ remaining NOTIFY remainingChanged)
```

The model also has normal properties (lesson 12). `count` and `remaining` are calculated from the list, and they emit their notify signals when a row is added, removed or ticked. The heading in QML uses them in a binding: `"%1 of %2 tasks left".arg(todos.remaining).arg(todos.count)`. (`rowCount()` is not a property, so a binding on it would not update: lesson 12, section 6.)

---

## 5. Changing the Data From QML

Two ways to send a change back to C++ are in this lesson:

```qml
onClicked: todos.remove(row.index)                 // a Q_INVOKABLE method with the row number
onToggled: todos.setDone(row.index, checked)       // another one, which calls setData()
```

Why not `onToggled: row.done = checked`? Writing the role in the delegate should go through `setData()`. In the test with Qt 6.4.2, that write did **not** reach the model when the delegate used `required property`. Your Qt 6.8 kit may behave differently, but a method that says what it does is clear in every version, and it is easy to test in C++. Use `setData()` for editing from C++ views (Qt Widgets views, `QTableView`), and methods for QML.

The method takes the **row number** (`index`). A row number is valid only while the list does not change: do not keep it for later.

Notice that the QML code does not decide anything about the list. It asks the model to do things, and the model changes itself and tells the views.

---

## 6. `ListModel` or C++?

| | `ListModel` (QML) | `QAbstractListModel` (C++) |
|---|---|---|
| Data | Written in QML or added by JavaScript | Any C++ data: a list, a database, a file, a network |
| Size | Small lists | Any size, also with lazy loading (`canFetchMore`, `fetchMore`) |
| Logic | In JavaScript | In C++: testable, fast |
| Tests | Only through QML | `QSignalSpy`, `QAbstractItemModelTester` |
| Reuse | QML only | QML and Qt Widgets views |
| Effort | Very small | About 80 lines for a list |

Rule: start with a `ListModel` for a prototype. Move to C++ when the data has rules or comes from outside. A **table** model (`QAbstractTableModel`, several columns) is shown with `TableView`, and a **tree** with `TreeView`.

For sorting and filtering, do not write the code into the model: put a `QSortFilterProxyModel` between the model and the view (chapter `07_ModelView`, lesson 03). It also works for QML.

---

## 7. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app14_cpp_list_models
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Add a few tasks, tick some, and press **Clear finished tasks**.
2. Add a role `PriorityRole` (an `int`) with the name `priority`, show it in the delegate, and fill it with a number for each item.
3. In `TodoModel::remove`, remove the lines `beginRemoveRows` and `endRemoveRows`. What does the view do? (Only try this to learn: it is a bug.)
4. Make `add()` insert at the top (row 0) instead of the end.
5. Change `TodoModel` so that it loads its items from a JSON file at the start (chapter `10_IO_and_Data`, lesson 01) and saves them when the list changes.
6. Put a `QSortFilterProxyModel` (C++) between the model and the `ListView` that hides finished tasks.

---

## 8. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The list is empty | `roleNames()` is missing, or the delegate has no size | Implement it; give the delegate a height |
| `ReferenceError: title is not defined` | The role name in `roleNames()` is different | Use the same names in the delegate |
| A row shows old data | `dataChanged` is not emitted | Emit it with the changed roles |
| The view shows wrong rows after a change | The `begin...` and `end...` calls are missing or in the wrong order | Always use them around the change |
| Crash when the last row is removed | The code uses a row number that is gone | Check the range in every function |
| `QAbstractItemModelTester` reports `flags` | `flags()` returns editable for the root | Return the base flags for an invalid index |
| `rowCount` is called with a valid parent and the view shows children | `rowCount` does not return 0 for a valid parent | `parent.isValid() ? 0 : count()` |
| The count in QML does not update | `count` is a method, or it has no notify signal | A `Q_PROPERTY` with `NOTIFY` |
| `Unable to assign ...` when you write the role | The role is read-only, `setData` returns false | Implement `setData` and `flags` |

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Using C++ models with Qt Quick views | <https://doc.qt.io/qt-6/qtquick-modelviewsdata-cppmodels.html> |
| `QAbstractListModel` | <https://doc.qt.io/qt-6/qabstractlistmodel.html> |
| Subclassing `QAbstractItemModel` | <https://doc.qt.io/qt-6/qabstractitemmodel.html#subclassing> |
| `QAbstractItemModelTester` | <https://doc.qt.io/qt-6/qabstractitemmodeltester.html> |
| Model/View programming | <https://doc.qt.io/qt-6/model-view-programming.html> |
| `ListView` | <https://doc.qt.io/qt-6/qml-qtquick-listview.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`); `QAbstractListModel` is in Qt Core |
| Headers | `<QAbstractListModel>`, `<QList>`, `<QtQml/qqmlregistration.h>` |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
