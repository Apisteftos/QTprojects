# 02_First_Window

Your first Qt Widgets window. You create the project with the Qt Creator wizard, read every file it writes, and then change the title, the size and the status bar from C++.

**Requires Qt 6.8 or newer.**

**Tested:** the project builds with CMake and GCC 13.3 without warnings and runs with Qt's offscreen platform plugin. A small test program checked the window properties of section 5 (10 checks, all pass). It was built against Qt 6.4.2, the version of the Ubuntu 24.04 packages, because no newer Qt was available on the test machine. The code uses only functions that exist in every Qt 6 version, so Qt 6.8 and newer should behave the same. If your compiler or Qt version shows a problem, check the section "Common Problems".

---

## 1. What You Build

A window with a title, a menu bar area, an empty central area and a status bar. It opens at 640 x 400 pixels and cannot be made smaller than 320 x 200. The status bar says "Ready" for five seconds.

There is no button yet and there is no logic. The goal is to understand what a window is, which objects exist when the program runs, and which files belong together.

---

## 2. Create the Project

In Qt Creator choose **File > New Project > Application (Qt) > Qt Widgets Application** and use these settings:

| Page | Setting | Value |
|------|---------|-------|
| Location | Name | `First_Window` |
| Build System | Build system | CMake |
| Class Information | Class name | `MainWindow` |
| Class Information | Base class | `QMainWindow` |
| Class Information | Generate form | ticked |
| Translation | Language | none |
| Kits | Kit | a kit with Qt 6.8 or newer |

The project name starts with a letter, because it becomes the name of the executable.

Then change two lines in the `CMakeLists.txt` that the wizard wrote. Everything else stays as it is.

| Wizard value | Tutorial value | Why |
|--------------|----------------|-----|
| `cmake_minimum_required(VERSION 3.16)` | `cmake_minimum_required(VERSION 3.21)` | The tutorial minimum |
| `set(CMAKE_CXX_STANDARD 17)` | `set(CMAKE_CXX_STANDARD 23)` | The tutorial uses C++23 |

The form `mainwindow.ui` also stays as the wizard wrote it. The window is changed in C++ in section 5, so you can see each property in code.

---

## 3. The Files the Wizard Writes

| File | Purpose |
|------|---------|
| `CMakeLists.txt` | Describes the build. It finds Qt, lists the source files and links `Qt::Widgets` |
| `main.cpp` | Starts the program: creates the application object and the window, then runs the event loop |
| `mainwindow.h` | Declares the class `MainWindow` |
| `mainwindow.cpp` | Defines it. The constructor builds the window |
| `mainwindow.ui` | The form made in Qt Designer: an XML file. `uic` turns it into the header `ui_mainwindow.h` at build time |

### `main.cpp`

```cpp
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
```

| Line | What it does |
|------|--------------|
| `QApplication a(argc, argv)` | Creates the one application object. It reads the command-line arguments that Qt understands, connects to the window system and owns the event loop. It must exist before any widget is created |
| `MainWindow w` | Creates the window. This only builds the object in memory. Nothing is on the screen yet |
| `w.show()` | Makes the window visible. A new top-level window is hidden until you call this |
| `a.exec()` | Starts the event loop. The function returns when the last window is closed, and its value becomes the exit code of the program |

`w` is an ordinary local variable, so it is destroyed at the end of `main`, after `exec()` returned. That is correct for the top-level window. Widgets inside a window are created with `new` and get a parent (lesson 03).

### `mainwindow.h`

```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
```

- `Q_OBJECT` tells the build to run **moc** on this header. It is needed for signals, slots and properties. `CMAKE_AUTOMOC ON` makes CMake do that automatically.
- `Ui::MainWindow` is **not** the same class as `MainWindow`. It is a generated helper class, in the namespace `Ui`, that creates all the widgets of the form. The header only forward-declares it, and `ui` points to it.
- `parent = nullptr` means the window has no parent. A widget without a parent is a window of its own.

### `mainwindow.cpp`

```cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // The window's own properties
    setWindowTitle("My First Window");
    resize(640, 400);
    setMinimumSize(320, 200);

    // The status bar comes from the form; show a message for 5 seconds
    statusBar()->showMessage("Ready", 5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
```

`ui->setupUi(this)` creates the central widget, the menu bar and the status bar that the form describes, and puts them into the window. It must be the first statement of the constructor body. The lines after it are ours. The destructor deletes the helper object. The widgets themselves belong to the window and are deleted with it.

`ui_mainwindow.h` does not exist in the source folder. It is generated into the build folder, which is why the include may look unresolved in the editor until the first build.

### `CMakeLists.txt`

| Line | Meaning |
|------|---------|
| `set(CMAKE_AUTOUIC ON)` | Runs `uic` on the `.ui` files |
| `set(CMAKE_AUTOMOC ON)` | Runs `moc` on headers with `Q_OBJECT` |
| `set(CMAKE_AUTORCC ON)` | Runs `rcc` on resource files (none here) |
| `find_package(Qt6 ... COMPONENTS Widgets)` | Finds the Qt Widgets module. It also loads Qt Core and Qt Gui |
| `qt_add_executable(First_Window ...)` | Creates the program from the listed files. The `.ui` file is listed so that `uic` sees it |
| `target_link_libraries(... Qt6::Widgets)` | Links the program with Qt Widgets |

The wizard also writes lines for Qt 5, Android, macOS and installation. They do no harm, and this tutorial leaves them in.

---

## 4. What Is a Window?

In Qt Widgets, everything you see is a **widget**: a button, a label, a window. They all derive from `QWidget`. The difference between a window and a button is only the **parent**:

- A widget **with** a parent is drawn inside its parent.
- A widget **without** a parent is a **top-level widget**, which means a window. It gets a frame and a title bar from the window system.

`isWindow()` tells you which one a widget is. Three classes are used as windows:

| Class | Use it for | Contains |
|-------|-----------|----------|
| `QWidget` | The simplest window, or any widget | Nothing. You add everything yourself |
| `QMainWindow` | The main window of an application | A menu bar, tool bars, dock areas, a central widget and a status bar |
| `QDialog` | A short question or a settings window (lesson `02_Widgets/04`) | A result code and `exec()` for a modal dialog |

### The structure of a `QMainWindow`

```
+--------------------------------------------+
| Menu bar            (QMenuBar)             |
+--------------------------------------------+
| Tool bars           (QToolBar)             |
+--------+-----------------------+-----------+
| Dock   | Central widget        | Dock      |
| area   | (QWidget, your content)| area     |
+--------+-----------------------+-----------+
| Status bar          (QStatusBar)           |
+--------------------------------------------+
```

The central widget is the large area in the middle. Your own content goes there. The form of the wizard already contains an empty one. Menus, tool bars and actions are the topic of chapter `05_MainWindow`.

---

## 5. Our Changes: Title, Size, Status Bar

| Call | Effect |
|------|--------|
| `setWindowTitle("My First Window")` | The text in the title bar. The form says `MainWindow`, and the code replaces it when the constructor runs |
| `resize(640, 400)` | The size of the window **content** (without the frame) when it opens. The form said 800 x 600 |
| `setMinimumSize(320, 200)` | The user cannot make the window smaller. Qt keeps the size at this value or more, even if you call `resize(100, 100)` |
| `statusBar()->showMessage("Ready", 5000)` | A temporary message in the status bar. The second argument is the time in milliseconds. `0` or no argument keeps the message until it is replaced |

A few more calls to try (all are members of `QWidget`):

| Call | Effect |
|------|--------|
| `setMaximumSize(w, h)` | The user cannot make the window larger |
| `setFixedSize(w, h)` | Minimum and maximum together: the size cannot change |
| `setWindowIcon(QIcon(...))` | The icon of the window (needs `#include <QIcon>`) |
| `showMaximized()`, `showFullScreen()`, `showMinimized()` | Instead of `show()`. Opens in that state |

### Size, position and the frame

- `size()`, `width()` and `height()` describe the **content**. `frameGeometry()` includes the title bar and the border.
- Sizes are in **device-independent pixels**. On a screen with a scale factor of 200 %, a window of 640 x 400 is drawn with 1280 x 800 physical pixels. You do not convert anything, because Qt does it.
- `move(x, y)` sets the position of the window on the screen. Under **Wayland**, an application is usually not allowed to place its own windows, so `move()` has no visible effect there. Under X11, Windows and macOS it works.

---

## 6. The Shortest Window

The class `MainWindow` and the form are the usual structure for an application, but a window needs much less. This program has no extra files and no `.ui` form:

```cpp
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Shortest Window");
    window.resize(400, 250);
    window.show();

    return app.exec();
}
```

Its `CMakeLists.txt` needs only `find_package(Qt6 REQUIRED COMPONENTS Widgets)`, `qt_add_executable(...)` and `target_link_libraries(... PRIVATE Qt6::Widgets)`. It builds and runs. Use this version when you want to try one idea quickly. Use a class of your own when the window is part of an application, because a class has a place for members, slots and signals.

---

## 7. Build and Run

Open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**. Or on the command line, from this folder:

```bash
cmake -S . -B build
cmake --build build
./build/First_Window
```

Try this:

1. Drag the window edges. It stops at 320 x 200.
2. Wait five seconds. The "Ready" message disappears.
3. Change `resize(640, 400)` to `resize(100, 100)`. The window opens at the minimum size, not at 100 x 100.
4. Comment out `w.show()` in `main.cpp`. The program starts, but no window appears, and it never ends by itself, because the event loop is running. Stop it in Qt Creator or press Ctrl+C in the terminal.
5. Put `w.show()` before `return` and add a second `MainWindow w2; w2.show();`. Two windows open. The program ends only when both are closed.

---

## Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The program starts and runs, but no window appears | `show()` was never called | Call `w.show()` before `a.exec()` |
| `QWidget: Cannot create a QWidget without QApplication` (and the program stops) | A widget was created before `QApplication` | Create `QApplication` first in `main` |
| `fatal error: ui_mainwindow.h: No such file or directory` | The header is created by `uic` during the build. The `.ui` file is not in the list of sources, or `CMAKE_AUTOUIC` is off | List `mainwindow.ui` in `qt_add_executable` and set `CMAKE_AUTOUIC ON` |
| `undefined reference to vtable for MainWindow` | `moc` did not run. `Q_OBJECT` was added to an existing header and CMake was not run again | Run **Build > Run CMake** (or delete the build folder), then build |
| `Could not find a package configuration file provided by "Qt6"` | CMake does not know where Qt is | Choose a Qt kit in Qt Creator, or pass `-DCMAKE_PREFIX_PATH=<your Qt folder>` |
| The window does not go to the position that `move()` gives | Wayland does not let applications place their windows | Expected. Nothing to fix |
| `Could not load the Qt platform plugin "xcb"` (or `wayland`) at start | The program cannot connect to a display, or a system library of the plugin is missing | Start the program in a desktop session. For tests without a display use `QT_QPA_PLATFORM=offscreen` |
| `ui` is flagged as an error in the editor | The editor does not see the generated header yet | Build once. If it stays, run CMake again |

---

## Reference

| Topic | Where to look |
|-------|---------------|
| `QApplication` | <https://doc.qt.io/qt-6/qapplication.html> |
| `QMainWindow` | <https://doc.qt.io/qt-6/qmainwindow.html> |
| `QWidget` (`show`, `resize`, `setMinimumSize`, `setWindowTitle`, `move`) | <https://doc.qt.io/qt-6/qwidget.html> |
| `QStatusBar` | <https://doc.qt.io/qt-6/qstatusbar.html> |
| Window and dialog widgets | <https://doc.qt.io/qt-6/application-windows.html> |
| Using a Designer UI file in your application | <https://doc.qt.io/qt-6/designer-using-a-ui-file.html> |
| Qt and CMake (`AUTOMOC`, `AUTOUIC`) | <https://doc.qt.io/qt-6/cmake-manual.html> |
| Module | Qt Widgets (`find_package(Qt6 ... COMPONENTS Widgets)`) |
| Headers | `<QApplication>`, `<QMainWindow>`, `<QStatusBar>` |
| Minimum Qt version | Qt 6.8 |
