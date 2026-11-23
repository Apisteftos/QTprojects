# 12_data_and_models

Show a list of data. Until now every item on the screen was placed by hand. A **model** holds the data, a **view** shows it, and a **delegate** is the template for one entry. Here a `ListView` shows a list of plants, and the details of the selected plant appear on the right.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings. A test script clicked a row and the Add and Remove buttons (section 4).

The same idea written by hand is in `11_QML/10_models_and_views`. A C++ model comes in `11_QML/14_cpp_list_models`.

---

## 1. What You Build

A list of five plants on the left and a detail card on the right.

| Part | What it does |
|------|--------------|
| List (left) | Shows name, count and a colored dot of every plant. The selected row is highlighted |
| Title | `Plants (5)`: the number of entries, bound to the model |
| Detail card (right) | Shows the name, the count and the color of the **selected** plant |
| **Add** button | Appends a new entry to the model and selects it |
| **Remove** button | Removes the selected entry |

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The model, the view with its delegate, the detail card and the connections |
| `App.qml` | The window |

---

## 2. Terms

| Term | Meaning |
|------|---------|
| **Model** | The data and its structure. `ListModel` holds a list of `ListElement` entries |
| **Role** | A named value of an entry. Here `name`, `count` and `tint` |
| **View** | The component that shows the data: `ListView` (a list), `GridView` (a grid) or `PathView` (items on a path) |
| **Delegate** | The template for one entry. The view makes one instance of it for every entry |
| **`index`** | The position of the entry in the model. The delegate gets it automatically |
| **`currentIndex`** | The entry of the view that is selected |

To show data you bind the `model` property of the view to a model, and the `delegate` property to a component.

When you add a Grid View, List View or Path View in Design Studio, the `ListModel` and the delegate are added automatically, with default content, and you can edit them.

---

## 3. Steps

### 3.1 Create the project

Create `Data_and_models` with the settings of the [area README](../README.md). Keep the `import` line that Design Studio generated in `App.qml`.

### 3.2 Add the list view

1. Drag a **List View** from **Components > Default Components > Views** into a white card in the scene.
2. Design Studio adds a `ListModel` and a delegate with default content. Give the view the id `plantList`. Do not use the id `list`: Design Studio rejects it as ambiguous (error M209).
3. Anchor the view inside the card, below the title text (lesson 03).

### 3.3 Edit the list model

1. Right-click the view in the Navigator and choose **Edit List Model**.
2. Double-click the column headings and the cells to change them. In a list, each column is a role (a property) and each row is an entry.
3. Use the toolbar buttons to add, remove or move rows and columns.

Create three columns, `name`, `count` and `tint`, and enter five rows:

| name | count | tint |
|------|-------|------|
| Fern | 12 | `#2e7d5a` |
| Cactus | 3 | `#7aa35a` |
| Orchid | 7 | `#b05fa0` |
| Moss | 20 | `#4f8a6b` |
| Bonsai | 1 | `#8a6b4f` |

Give the model the id `plantModel`. You can see the result as `ListElement` entries in the Text Editor.

### 3.4 Design the delegate

Replace the default delegate content with your own, in the Code view, as in section 4:

- a rounded Rectangle as the row, with a colored dot (`model.tint`), a name (`model.name`) and a count (`model.count`);
- a Mouse Area over the whole row that sets `plantList.currentIndex = index`;
- a row color that depends on `ListView.isCurrentItem`.

Item Delegate and Swipe Delegate are also available in **Components > Qt Quick Controls**.

### 3.5 The detail card

Add a Rectangle on the right with a circle, two Text items and two buttons. Set these bindings with **Set Binding** (lesson 09):

| Item | Property | Expression |
|------|----------|------------|
| `bigSwatch` | `color` | `plantList.currentIndex >= 0 ? plantModel.get(plantList.currentIndex).tint : "#dfe8e3"` |
| `detailName` | `text` | `plantList.currentIndex >= 0 ? plantModel.get(plantList.currentIndex).name : "Nothing selected"` |
| `detailCount` | `text` | `plantList.currentIndex >= 0 ? plantModel.get(plantList.currentIndex).count + " in stock" : ""` |
| `listTitle` | `text` | `"Plants (" + plantModel.count + ")"` |

`plantModel.get(i)` returns the entry at position `i`. The test `plantList.currentIndex >= 0` avoids an error when nothing is selected.

### 3.6 Change the data at run time

Add two connections (lesson 08) with **Manual Edit**:

| Target | Signal | Code |
|--------|--------|------|
| `addArea` | `clicked` | `plantModel.append({"name": "New plant", "count": 1, "tint": "#c9a227"})` and `plantList.currentIndex = plantModel.count - 1` |
| `removeArea` | `clicked` | `if (plantList.currentIndex >= 0) plantModel.remove(plantList.currentIndex)` |

The view and everything bound to the model update by themselves.

### 3.7 Preview

Press **Alt+P**. Click rows, add and remove entries.

---

## 4. The Code

```qml
import QtQuick

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    ListModel {
        id: plantModel

        ListElement {
            name: "Fern"
            count: 12
            tint: "#2e7d5a"
        }

        ListElement {
            name: "Cactus"
            count: 3
            tint: "#7aa35a"
        }

        // ... Orchid, Moss and Bonsai in the same way
    }

    Rectangle {
        id: listCard
        width: 360
        color: "#ffffff"
        radius: 16
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 24

        Text {
            id: listTitle
            text: "Plants (" + plantModel.count + ")"
            font.pixelSize: 24
            color: "#1d4d38"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 20
        }

        ListView {
            id: plantList
            clip: true
            model: plantModel
            currentIndex: 0
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: listTitle.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 16

            delegate: Rectangle {
                id: plantRow
                width: plantList.width
                height: 52
                radius: 8
                color: ListView.isCurrentItem ? "#dfe8e3" : "#ffffff"

                Rectangle {
                    id: swatch
                    width: 24
                    height: 24
                    radius: 12
                    color: model.tint
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: nameText
                    text: model.name
                    font.pixelSize: 20
                    color: "#1d4d38"
                    anchors.left: swatch.right
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: countText
                    text: model.count
                    font.pixelSize: 18
                    color: "#5b6b63"
                    anchors.right: parent.right
                    anchors.rightMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                    id: rowArea
                    anchors.fill: parent
                    onClicked: plantList.currentIndex = index
                }
            }
        }
    }

    // the detail card (bigSwatch, detailName, detailCount) and the two
    // buttons (addArea, removeArea) are in Screen01.ui.qml

    Connections {
        target: addArea
        function onClicked() {
            plantModel.append({"name": "New plant", "count": 1, "tint": "#c9a227"})
            plantList.currentIndex = plantModel.count - 1
        }
    }

    Connections {
        target: removeArea
        function onClicked() {
            if (plantList.currentIndex >= 0)
                plantModel.remove(plantList.currentIndex)
        }
    }
}
```

The listing is shortened. The complete working file is `Screen01.ui.qml`.

| Part | Meaning |
|------|---------|
| `ListModel` with `ListElement` | The data. Each `ListElement` is one entry with the same roles |
| `model: plantModel` | The view takes its entries from the model |
| `delegate: Rectangle { ... }` | The template. The view creates one for each entry |
| `model.name`, `model.count`, `model.tint` | The roles of the entry that the delegate shows |
| `index` | The position of this entry in the model. Used to select the row |
| `ListView.isCurrentItem` | `true` for the selected row. Used for the highlight |
| `plantModel.count` | The number of entries. It updates when you add or remove |
| `plantModel.append({ ... })`, `plantModel.remove(i)` | Change the model. The view follows |
| `clip: true` | Cuts off rows that scroll out of the view |

What the test script saw with Qt 6.12.0:

| Action | Title | Detail card |
|--------|-------|-------------|
| Start | `Plants (5)` | `Fern`, `12 in stock` |
| Click the row `Orchid` | `Plants (5)` | `Orchid`, `7 in stock` |
| Click **Add** | `Plants (6)` | `New plant`, `1 in stock` |
| Click **Remove** | `Plants (5)` | `Bonsai`, `1 in stock` |

After **Remove** the list has five entries again, and the selection moved to the last entry.

---

## 5. Data from a File: The Model Editor

Design Studio has a **Model Editor** view for tables of data. In it you can create, manage, import and export data models, and you can use them to populate views.

| Task | How |
|------|-----|
| Create a model | In the Model Editor choose the create button, enter a name and choose **Create**. This makes a table with a single cell. Then add columns, rows and data |
| Import | Import a data model from a **JSON** or **CSV** file with the import button of the Model Editor |
| Export | Export a model to a JSON or CSV file |
| Save | You must save the table manually after changes, with the save button |

Use the Model Editor when the data is large or comes from a file. The small `ListModel` in this lesson is good for a handful of entries. The Qt documentation shows the Model Editor for a `Repeater3D`. Check in 4.8.2 how the result can be bound to a `ListView`.

Real application data usually comes from C++ (lesson `11_QML/14_cpp_list_models`) or from a network service (`11_QML/15_network_and_data`). The delegate you design here stays the same.

---

## 6. Try This

1. Add a fourth role `price` with the type real, and show it in the delegate.
2. Change `ListView` to a `GridView` (lesson 03 shows how to anchor it). Set `cellWidth` and `cellHeight`, and adapt the delegate.
3. Add a **Reset count** button that sets `plantModel.setProperty(plantList.currentIndex, "count", 0)`.
4. Highlight rows with an even `index` in a slightly different color, with a binding on `index % 2`.
5. Import a small JSON file in the Model Editor and compare it with the `ListModel`.

---

## 7. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| `This id might be ambiguous and is not supported in Qt Design Studio (M209)` | An id such as `list` clashes with a QML keyword. Rename it, for example to `plantList` |
| The list is empty | The view has no `model`, or the view has no height. Anchor it, or set a `height` |
| `ReferenceError: name is not defined` | The delegate uses a role that the model does not have. Check the column names in the list model editor |
| A selected row is not highlighted | The delegate does not use `ListView.isCurrentItem`, or `currentIndex` is `-1` |
| `TypeError: Cannot read property 'name' of undefined` | `plantModel.get(...)` was called with an index outside the list. Test `plantList.currentIndex >= 0` first |
| Items are drawn outside the view when you scroll | The view needs `clip: true` |
| A change in the Model Editor is not in the preview | The table was not saved. Save it with the save button |
| The color of the dot is black | The `tint` role holds text that is not a color. Use a name or a `#rrggbb` value |

---

## 8. Git

Commit this README, `Screen01.ui.qml` and `App.qml`. See the area README for the files that stay out of Git.

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Lists and other data models | <https://doc.qt.io/qtdesignstudio/quick-data-models.html> |
| The Model Editor | <https://doc.qt.io/qtdesignstudio/studio-model-editor.html> |
| Loading placeholder data | <https://doc.qt.io/qtdesignstudio/qtquick-placeholder-data.html> |
| Models and views in Qt Quick | <https://doc.qt.io/qt-6/qtquick-modelviewsdata-modelview.html> |
| `ListModel` | <https://doc.qt.io/qt-6/qml-qtqml-models-listmodel.html> |
| `ListView` | <https://doc.qt.io/qt-6/qml-qtquick-listview.html> |
