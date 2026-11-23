# 15_designer_developer_handoff

Hand a Design Studio project to a developer. The designer builds the screens with mock data. The developer replaces the mock with real application logic and does not need to change the screens.

**Tested with Qt Design Studio 4.8.2:** not run in Design Studio itself. The QML was run with Qt 6.12.0 without warnings in two setups: with the mock `Backend.qml`, and with a stand-in backend written in Python that has the same members. A test script clicked **Start** and **Stop** in both setups (section 5).

**Not compiled:** the C++ sketch in section 6. The test machine had no Qt C++ development files, so treat that code as a sketch.

---

## 1. What You Build

A status panel with a level bar, a status text and two buttons. The screen only shows values and reports clicks. It does not decide anything.

| File | Who owns it | Purpose |
|------|-------------|---------|
| `Screen01.ui.qml` | Designer | The visual part. Has an **interface**: the properties `level` and `statusText`, and the exported buttons `startArea` and `stopArea` |
| `Backend.qml` | Designer, for now | A mock of the application logic: `level`, `running`, `status`, `start()`, `stop()`. A timer changes the level so the designer can see something move |
| `App.qml` | Both | Connects the screen to the backend |

---

## 2. The Idea: a Contract

The designer and the developer agree on a short list of names and types. This is the **contract**.

| Name | Type | Direction |
|------|------|-----------|
| `level` | `int` (0 to 100) | backend → screen |
| `status` | `string` | backend → screen |
| `start()` | function | screen → backend |
| `stop()` | function | screen → backend |

As long as the real backend has the same names and types, the screen works with it unchanged.

Keep the screen free of logic. Every value comes in through a property. Every user action leaves through an exported item.

---

## 3. Steps for the Designer

### 3.1 Create the project

Create `Designer_developer_handoff` with the settings of the [area README](../README.md). Keep the `import` line that Design Studio generated in `App.qml`.

### 3.2 Build the screen

Build the panel in `Screen01.ui.qml`: a track rectangle with a fill rectangle inside, two text items and two buttons (a rectangle with a text and a `MouseArea`).

Bind the fill width to the value:

```qml
Rectangle {
    id: fill
    width: parent.width * root.level / 100
    height: parent.height
}
```

### 3.3 Define the interface

1. Add the custom properties `level` (int) and `statusText` (string) to the root item. Give them default values, so the 2D view shows something. See lesson 09.
2. Export each `MouseArea` as an alias property of the root: select it in the **Navigator** and press the **Export** button of that row. The result is a line like `property alias startArea: startArea`.

### 3.4 Add the mock backend

Add a QML file `Backend.qml` with a `QtObject` that has the contract members. See the file in this folder. The timer lets you see the bar move in the live preview.

### 3.5 Connect everything in `App.qml`

```qml
Backend { id: backend }

Screen01 {
    id: mainScreen
    anchors.fill: parent
    level: backend.level
    statusText: backend.status
}

Connections {
    target: mainScreen.startArea
    function onClicked() { backend.start() }
}
```

Use the `function onClicked() { }` form (see lesson 08).

Run the project. Press **Start**: the bar moves and the status shows `Running`. Press **Stop**: the bar stops.

---

## 4. The Project Folder

| Folder or file | What it is |
|----------------|------------|
| `Designer_developer_handoffContent/` | The QML files and assets. This folder is **not** a module. Never import it |
| `Generated/` | Files Design Studio generates, for example for 3D assets and content library items. Do not edit them by hand |
| `*.qmlproject` | The project file that Design Studio uses |
| `CMakeLists.txt` | The project file that Qt Creator uses. Design Studio creates it when you enable the CMake export (section 6.1) |

The module name is the project name. The `import` line in `App.qml` must match it.

Put the project under Git and store only the source files. Do not commit generated files or the build folder.

---

## 5. How This Was Tested

Two runs with Qt 6.12.0, offscreen, with a script that clicked the two buttons:

| Run | Result |
|-----|--------|
| With `Backend.qml` (the mock) | Before: level 40, `Idle`. After **Start** and 1.3 s: level changed, `Running`. After **Stop**: level frozen, `Idle` |
| Without `Backend.qml`, with a stand-in backend with the same members | Same result |

The second run shows the contract works: the screen and `App.qml` did not change.

---

## 6. Steps for the Developer

### 6.1 Open the project in Qt Creator

1. In Design Studio choose **File > Export Project > Enable CMake Generator**. This creates a `CMakeLists.txt` in the project folder, and the connection tracks later changes you make in Design Studio.
2. In Qt Creator choose **File > Open File or Project** and select the `CMakeLists.txt`.
3. Choose a Qt version that is not older than the one Design Studio used, then build and run.

The generated `CMakeLists.txt` files in the **root** and the **App** folder are yours to edit. The ones in other folders are overwritten when the generator runs again.

### 6.2 Replace the mock (sketch, not compiled)

Write a C++ class with the contract members and register it as a QML type. Then delete `Backend.qml`.

```cpp
// backend.h
#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int level READ level NOTIFY levelChanged FINAL)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged FINAL)

public:
    explicit Backend(QObject *parent = nullptr);

    int level() const { return m_level; }
    QString status() const { return m_running ? "Running" : "Idle"; }

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void levelChanged();
    void statusChanged();

private:
    int m_level = 40;
    bool m_running = false;
};
```

The class goes into a QML module with `qt_add_qml_module`, and `App.qml` imports that module. The screen stays the same: `level: backend.level` and `backend.start()` work with the C++ class exactly as with the mock.

Python works the same way. Design Studio can enable a Python folder with a `main.py` in the project.

---

## Common Problems

- **`QML module not found (...Content)`:** the `import` line contains the folder name. Use the project name only.
- **The screen shows nothing in the 2D view:** the values come from the backend at run time. Give the properties of the root item default values.
- **Design Studio shows an error for the code in `Screen01.ui.qml`:** keep logic out of `.ui.qml` files. Put it in `App.qml` or the backend.
- **The real backend works but the bar does not move:** the C++ property needs a `NOTIFY` signal and the class must emit it when the value changes.

## Reference

- **Qt version:** Qt 6.8 or newer
- **Design Studio:** the Navigator **Export** button, the CMake export (**File > Export Project**), the Connections view
- **Qt classes:** `QtObject`, `Timer`, `Connections` (QML); `QObject`, `Q_PROPERTY`, `Q_INVOKABLE`, `QML_ELEMENT` (C++, header `<QtQml/qqmlregistration.h>`)
- **Documentation:** the Qt Design Studio manual, chapters "Designer-developer workflow" and "Exporting a Qt Design Studio project for C++ development"
