# 14_QtDesignStudio

Qt Design Studio: a visual tool for designing Qt Quick interfaces. This README is the installation guide, with the problems you can meet on Linux, and a first project.

**Tested with Qt Design Studio 4.8.1** (Flatpak) on Ubuntu 26.04.1 LTS.

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

## 2. Installation

There are three ways. Use the first one if you already have Qt installed with the online installer.

### 2.1 Qt Maintenance Tool

Each Qt installation made with the online installer contains a Maintenance Tool. With Qt in `~/Qt` it is `~/Qt/MaintenanceTool`.

```bash
# find the package name
~/Qt/MaintenanceTool search designstudio

# or use the graphical version, log in and tick Qt Design Studio
~/Qt/MaintenanceTool
```

### 2.2 Qt Online Installer

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

### 2.3 Flatpak

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

## 3. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `qt.glx: qglx_findConfig: Failed to finding matching FBConfig ...` and `Could not initialize GLX` when the Flatpak starts | The app tried to open an OpenGL window through X11, and the sandbox found no matching graphics driver | Try the three fixes below, in this order |
| The Flatpak cannot open a project folder | The sandbox only sees some folders | Copy the project to `~/Documents/QtDesignStudio`, or give access: `flatpak override --user --filesystem=<folder> io.qt.qtdesignstudio` |
| The online installer says the directory already contains an installation | The installer cannot add to an existing installation | Use `~/Qt/MaintenanceTool` instead |

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

## 4. A First Project

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

## 5. Git: What to Commit

The Qt documentation says to store and edit only project source files and configuration files, and not generated files.

- The `.qmlproject.qtds` file next to the `.qmlproject` file is the equivalent of the `.user` file in Qt Creator. It stores user-related settings of the project, so it does **not** belong in Git.
- Build folders, `*.user` files and editor caches do not belong in Git either.

This folder has its own `.gitignore` with these rules. In this tutorial repository, commit the README, screenshots you made yourself and any QML files that a lesson explains. Do not commit a whole generated project.

---

## 6. Reference

| Topic | Where to look |
|-------|---------------|
| Installing Qt Design Studio | <https://doc.qt.io/qtdesignstudio/studio-installation.html> |
| Creating projects | <https://doc.qt.io/qtdesignstudio/studio-projects.html> |
| Managing projects (Git) | <https://doc.qt.io/qtdesignstudio/studio-projects-managing.html> |
| Designer-developer workflow, CMake generator | <https://doc.qt.io/qtdesignstudio/studio-designer-developer-workflow.html> |
| Previewing on the desktop | <https://doc.qt.io/qtdesignstudio/creator-live-preview-desktop.html> |
| Alternative ways to install Qt on Linux | <https://doc.qt.io/qt/linux-alternative-installers.html> |
