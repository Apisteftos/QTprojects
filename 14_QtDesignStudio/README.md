# 14_QtDesignStudio

Qt Design Studio: a visual tool for designing Qt Quick interfaces. This README is the installation guide, with the problems you can meet on Linux, and a first project.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. Lessons 15 and 17 were also built in Qt Creator, with Qt 6.12.0 and GCC 15.

---

## 1. What Is Qt Design Studio?

Qt Design Studio is a separate application from Qt Creator. You build a Qt Quick interface by placing items on a canvas and setting their properties in panels, and the tool writes the QML for you. You can switch to the text editor at any time and read or edit the generated QML.

| | Qt Creator | Qt Design Studio |
|---|---|---|
| Main use | Writing C++ and QML code, building and debugging applications | Designing the interface visually |
| Project file | `CMakeLists.txt` | `.qmlproject` |
| Typical user | Developer | Designer, or a developer who prototypes a screen |

Qt Design Studio continues to use the `.qmlproject` file format, while Qt Creator uses a `CMakeLists.txt` file as the project file. The CMake generator of Design Studio converts the contents of the `.qmlproject` file to CMake, so a design can become a normal C++ application.

In this tutorial, the code-first chapters (`11_QML`) stay the main way to learn QML. Design Studio is a tool for experiments and for seeing what a property does.

---

## 2. Topics

Each lesson is a Design Studio project in its own folder. Create the project in Design Studio, then follow the README of the lesson. The project name must start with a letter, so the lesson `02_items_and_assets` contains a project named `Items_and_assets`.

| # | Folder | What it teaches |
|---|--------|-----------------|
| 01 | `01_scratch_ui` | First project, the editor views, Live Preview, `App.qml` and `Screen01.ui.qml` |
| 02 | `02_items_and_assets` | Rectangle, Text and Image items, the Assets view, importing images, the Properties panel |
| 03 | `03_layout_and_anchors` | Anchors and Layout in the Properties panel, so the UI follows the window size |
| 04 | `04_components` | Your own reusable component (a button or a card), instances and properties |
| 05 | `05_states` | The States view: several looks of one screen |
| 06 | `06_transitions_and_animations` | Transitions between states, animating properties |
| 07 | `07_timeline` | The Timeline editor, keyframes, an animated intro |
| 08 | `08_connections_and_actions` | The Connections view: a click changes a state or a property without code |
| 09 | `09_bindings_and_properties` | Custom properties and bindings in the visual editor |
| 10 | `10_controls_and_styles` | Qt Quick Controls, themes and styling |
| 11 | `11_multi_screen_flow` | Several screens and navigation between them |
| 12 | `12_data_and_models` | `ListModel` and JSON data in lists and views |
| 13 | `13_figma_import` | Importing a design from Figma with Qt Bridge |
| 14 | `14_2d_effects_and_3d` | Effects, the Effect Composer and a simple 3D scene |
| 15 | `15_designer_developer_handoff` | Open the project in Qt Creator, add C++ and logic, Git rules for `.ui.qml` files |
| 16 | `16_project_dashboard_ui` | A complete UI with states, timeline and a data model |
| 17 | `17_workbench_app` | A complete desktop application: menus, seven tabs, a Qt Quick UI, a Qt bridge and a tested C++23 core library, as a real Design Studio project that also builds in Qt Creator |

---

## 3. Installation

There are three ways. Use the first one if you already have Qt installed with the online installer.

### 3.1 Qt Maintenance Tool

Each Qt installation made with the online installer contains a Maintenance Tool. With Qt in `~/Qt` it is `~/Qt/MaintenanceTool`.

```bash
# find the package name
~/Qt/MaintenanceTool search designstudio

# or use the graphical version, log in and tick Qt Design Studio
~/Qt/MaintenanceTool
```

### 3.2 Qt Online Installer

1. Download the Qt Online Installer from your Qt Account.
2. Make it executable and run it:

```bash
chmod +x qt-online-installer-linux-x64-online.run
./qt-online-installer-linux-x64-online.run
```

3. Log in with your Qt Account.
4. On the "Installation options" page, select **Qt Design Studio**.
5. Follow the installer to the end.

Do not use the installer to add components to a folder that already contains an installation. It refuses that. Use the Maintenance Tool (2.1) for that case.

### 3.3 Flatpak

The Flatpak is a separate, sandboxed installation from Flathub. It does not use the Qt in `~/Qt`.

```bash
flatpak search "Qt Design Studio"
flatpak install flathub io.qt.qtdesignstudio
```

### Licensing

The terms depend on the version and on how you use it. The current installation page lists a free evaluation package for businesses and the Qt Edu license for students and teachers, and older documentation mentions a free community version. Read the license page shown by the installer before you choose.

### Check that it is installed

```bash
flatpak list --app | grep -i qtdesignstudio     # Flatpak
ls ~/Qt                                          # online installer: look for a Design Studio folder
```

### Start it

```bash
flatpak run io.qt.qtdesignstudio
```

Or open "Qt Design Studio" from the application menu.

---

## 4. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `qt.glx: qglx_findConfig: Failed to finding matching FBConfig ...` and `Could not initialize GLX` when the Flatpak starts | The app tried to open an OpenGL window through X11, and the sandbox found no matching graphics driver | Try the three fixes below, in this order |
| The Flatpak cannot open a project folder | The sandbox only sees some folders | Copy the project to `~/Documents/QtDesignStudio`, or give access: `flatpak override --user --filesystem=<folder> io.qt.qtdesignstudio` |
| The online installer says the directory already contains an installation | The installer cannot add to an existing installation | Use `~/Qt/MaintenanceTool` instead |
| The target Qt version list in the new-project dialog ends at 6.8 | The dialog offers the versions the project templates support, not the Qt that Design Studio runs on | Choose 6.8. The project minimum is `find_package(Qt6 6.8 ...)`, and a newer Qt is fine for building |
| The Projects panel shows no `.cpp` or `.hpp` files | The panel shows only the files that the `.qmlproject` lists | Edit C++ in Qt Creator (section 6) |
| Qt Creator: `Unknown CMake command "qt_add_library"` | You opened a `CMakeLists.txt` that is only part of the build | Open the one in the project folder (section 6.3) |
| CMake: `Failed to find required Qt component "QuickTimeline"` | The Qt installation lacks the module | Add it in the Maintenance Tool (section 6.3) |

### The GLX error: three fixes

**1. Update Flatpak and its graphics driver packages.** With an NVIDIA card, Flatpak needs a driver package that matches the host driver.

```bash
flatpak update
flatpak run io.qt.qtdesignstudio
```

**2. Run it through Wayland instead of X11.**

```bash
echo $XDG_SESSION_TYPE        # shows wayland or x11
flatpak run --socket=wayland --env=QT_QPA_PLATFORM=wayland io.qt.qtdesignstudio
```

**3. Software rendering.** It is slow, but it shows whether the program works at all.

```bash
flatpak run --env=LIBGL_ALWAYS_SOFTWARE=1 io.qt.qtdesignstudio
```

---

## 5. A First Project

1. In the welcome screen, click **Create Project**.
2. Fill in the details:

| Setting | Value |
|---------|-------|
| Preset | **Empty** |
| Name | `scratch_ui` (any name) |
| Location | A folder **outside** the tutorial repository, for example `~/Documents/QtDesignStudio` |
| Size | `800 x 480` (the default `1920 x 1080` is large) |
| Style | **Basic** |
| Target Qt Version | `Qt 6.8`, the minimum of this tutorial |
| Enable CMake Generation | Leave it ticked |

3. Click **Create**.

The target Qt version can be changed later in the run settings of the project, but the Qt documentation warns that the versions are not fully compatible.

### Try this

1. Drag a **Rectangle** and a **Text** item from the component library onto the canvas.
2. Change `color`, `width` and `x` in the properties panel.
3. Switch to the **Text Editor** view and read the QML that was written. It has the same `Rectangle { ... }` and `property: value` form as `Main.qml` in `11_QML/02_items_and_properties`.
4. Press **Alt+P** to start the Live Preview and see the real window.

### The default project structure

The wizard creates a structure like this (names depend on your project name and the Design Studio version):

| File or folder | Purpose |
|----------------|---------|
| `ProjectName.qmlproject` | The project file |
| `ProjectName/` | A module with the essential QML files, such as `Constants.qml` and `qmldir` |
| `ProjectNameContent/` | The default content, with `App.qml` (the entry point) and `Screen01.ui.qml` (the first scene) |
| `qtquickcontrols2.conf` | The preferred Qt Quick Controls style |
| `Generated/`, `Bundles/` | Imported 3D components and content library elements, if you use them |

---

## 6. Design Studio Projects With C++ Code

A Design Studio project is a QML project first. When you add C++ (as lesson 17 does), the two tools share one folder, and some rules decide who writes which file. These rules come from building lesson 17 in both tools.

### 6.1 Who writes what

| Part | Who writes it | What you may do |
|------|---------------|-----------------|
| `*.ui.qml` screens | Design Studio | Edit visually. No functions or handlers in them |
| `ProjectNameContent/*.qml` | You, or Design Studio | Put **all** your own files here, as the dashboard example does |
| `ProjectNameContent/CMakeLists.txt` and `ProjectName/CMakeLists.txt` | Design Studio ("Do not change") | Do not edit. Design Studio rewrites them when it saves |
| `qds.cmake`, `autogen/environment.h`, `ProjectName.qrc` | Design Studio | Do not edit, except to remove lines of files that no longer exist |
| Root `CMakeLists.txt`, `App/CMakeLists.txt`, `App/main.cpp` | You | Small edits for C++ (a library, a registered type) |
| `*.cpp`, `*.hpp` | You | Design Studio does not list or show them. Add them to the CMake lists by hand. Qt Creator shows only files that a target lists |

Keep your own files in the content folder and edit as few generated files as possible. Then Design Studio can rewrite the generated ones at any time without breaking your project.

### 6.2 The preview of a C++ class: a mock

Design Studio cannot load a C++ class. Give it a QML file with the same type name and the same members (a mock). Put the mock in a folder **outside** the content folder, with a `qmldir` whose `module` name is the folder path seen from the project folder, for example `Workbench_backend`. Inside the content folder, Design Studio would make a module of it and break the build. The real application registers the C++ class under the same module name with `qmlRegisterType`. The mock is never part of the build.

### 6.3 Open the project in Qt Creator

1. Open the `CMakeLists.txt` **in the project folder**, next to `qds.cmake` and the `App` folder. The `CMakeLists.txt` inside the module folders are parts of the build and cannot be opened alone.
2. Use a kit with Qt 6.8 or newer and a C++23 compiler for C++23 code.
3. The Qt installation must contain **Qt Quick Timeline**, **Qt Shader Tools** and any module your code needs (for example Qt Multimedia). Add missing ones in the Qt Maintenance Tool, in **Additional Libraries** of that Qt version.

The default project has two traps that only show in Qt Creator:

- The empty folder `Dependencies` has no `CMakeLists.txt`, but `qds.cmake` includes it. Create one that contains only a comment.
- The generated `Constants.qml` imports `QtQuick.Studio.Application`, which a normal Qt install does not have (it comes with Design Studio). The app stops at start with `module "QtQuick.Studio.Application" is not installed`. Do not use `Constants` in the files of your own application, or install the Qt Quick Designer Components into your Qt installation as the Qt documentation describes.

### 6.4 Stale build folders

When the project layout changes, delete the build folder and run CMake again. A build folder keeps the old configuration, and Design Studio keeps old entries in its generated files (see the table in section 4).

### 6.5 Noise in the output

Lines about PipeWire, PulseAudio, GTK surface sizes or FFmpeg stream information come from the system and the multimedia libraries, not from your code. Qt Creator shows everything on the error output in red. The `QT_LOGGING_RULES` entry in the `.qmlproject` hides the Qt categories you do not want (for example `qt.multimedia.ffmpeg=false`).

---

## 7. Git: What to Commit

The Qt documentation says to store and edit only project source files and configuration files, and not generated files. A build folder (for example from Qt Creator) is generated too.

- The `.qmlproject.qtds` file next to the `.qmlproject` file is the equivalent of the `.user` file in Qt Creator. It stores user-related settings of the project, so it does **not** belong in Git.
- Build folders, `*.user` files, temporary `*.qtds` files and editor caches do not belong in Git either.

This folder has its own `.gitignore` with these rules. In this tutorial repository, commit the README, screenshots you made yourself and any QML files that a lesson explains. Do not commit a whole generated project.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Installing Qt Design Studio | <https://doc.qt.io/qtdesignstudio/studio-installation.html> |
| Creating projects | <https://doc.qt.io/qtdesignstudio/studio-projects.html> |
| Managing projects (Git) | <https://doc.qt.io/qtdesignstudio/studio-projects-managing.html> |
| Designer-developer workflow, CMake generator | <https://doc.qt.io/qtdesignstudio/studio-designer-developer-workflow.html> |
| Previewing on the desktop | <https://doc.qt.io/qtdesignstudio/creator-live-preview-desktop.html> |
| Alternative ways to install Qt on Linux | <https://doc.qt.io/qt/linux-alternative-installers.html> |
| CMake generator, mocks for C++ types | <https://doc.qt.io/qtdesignstudio/studio-cmake-generator.html> |
