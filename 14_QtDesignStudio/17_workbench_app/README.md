# 17_workbench_app

A complete desktop application: **Workbench**. One window with a menu bar (File, View, Tools, Help, with submenus) and seven tabs. It has a user interface in Qt Quick and its own logic in a plain C++23 library.

The earlier lessons built screens. This lesson builds a program: the screens, the logic behind them, and the tests of that logic.

This lesson uses the project that **Qt Design Studio 4.8.2** generates. You create the project `Workbench_app` in Design Studio, then copy the files of this lesson over it (section 3).

**Tested:**

- **Core library:** built with CMake and GCC 13.3 (`-Wall -Wextra -Wpedantic`, no warnings). 89 checks pass (section 6).
- **QML:** 48 checks pass with Qt 6.12.0 (PySide6, offscreen). The QML files were used in the same folder structure as the Design Studio project and ran against the real core library through a test-only bridge (section 6).
- **The whole project, by hand:** it was built in Qt Creator with Qt 6.12.0 and GCC 15 (the real C++ backend and the core library), and the app started and ran. It also runs with **Run** in Design Studio 4.8.x (with the mock backend). Getting there took several fixes, which are in section 4.7 and in the Common Problems list.

**Not tested:**

- **Real video playback and sound.** The automated tests do not play video. By hand, a video file was opened on a machine without a usable audio device. The sound path (with an audio device) was not checked.
- **Qt 6.8.** The code uses only Qt 6.8 API, but the project was built with Qt 6.12.0 only. Qt 6.8 is the minimum of the tutorial, not a tested version.
- **A clean copy of the steps in section 3** on a second machine.

---

## 1. What You Build

| Tab | What it does | Controls |
|-----|--------------|----------|
| **Text** | Opens a text file, shows it, searches in it. You can also type in it (Read only switch) | `Button`, `TextField` (search), `Slider` (font size), `CheckBox` (word wrap), `Switch` (monospace), `Label` |
| **Video** | Plays a video file | toggle `Button` (Play/Pause), `Slider` (position and volume), `ComboBox` (speed), `Switch` (loop) |
| **Image** | Shows an image with zoom, rotation and grayscale | `Slider` (zoom), `CheckBox` (fit), toggle `Button` (grayscale), `Switch` (background) |
| **Units** | Converts length, weight and temperature | `ComboBox` ×3, `TextField`, `Button` (swap) |
| **Color** | Mixes a color with three sliders or a hex code | `Slider` ×3, `TextField` (hex), `Switch` (hex or RGB), `Button` (copy) |
| **Notes** | Writes a note and saves it as a text file with a Save dialog | `TextField` (title), `TextArea`, `CheckBox` (autosave), `Button` (Save, Save as) |
| **Timer** | A countdown with a progress bar | `Slider` (minutes), `ComboBox` (preset), toggle `Button` (Start/Stop) |

| Menu | Entries |
|------|---------|
| **File** | Open Text, Open Video, Open Image, Recent (a submenu with the last 5 files), Quit |
| **View** | Theme (Light, Dark), Status Bar, Go to Tab (7 entries), Full Screen |
| **Tools** | Unit Converter, Options (Word Wrap, Autosave), Preferences |
| **Help** | Keyboard Shortcuts, About Workbench, About Qt |

Not included, compared with the first sketch: line numbers in the text tab, and a checkerboard background in the image tab (there is a dark background switch instead).

---

## 2. The Design: Three Layers

```
  QML (what the user sees)          App.qml, TextTab.qml, ... , Theme.qml, AppState.qml
        |  calls functions, reads properties
        v
  Qt bridge (converts types)        backend/workbench_backend.h / .cpp   (a QObject)
        |  std::string, std::u16string_view, std::filesystem::path
        v
  Core library (the decisions)      core/*.hpp, core/*.cpp       plain C++23, no Qt
```

The rule: **a decision belongs in the core library, never in QML and never in the bridge.** QML shows values and reports clicks. The bridge only converts `QString` to `std::string` and back, and emits change signals.

Why this is useful:

- The core library builds and is tested **without Qt**: `cmake`, a compiler, done.
- The same core can later be used by a command-line tool or by a different user interface.
- The QML stays small, because every question ("is this hex code valid?", "what is 12.5 m in feet?") has one answer in one place.

---

## 3. Put the Files Into the Design Studio Project

**The rule of this lesson:** everything that is ours lives in the content folder, `Workbench_appContent`. The rest of the project stays as Design Studio generated it (the module folder `Workbench_app` with `Constants.qml` and the event list files, `Generated`, `Dependencies`, `cmake`, `qds.cmake`, the root `CMakeLists.txt`). A QML-only project works the same way, for example the dashboard in lesson 15.

### 3.1 Steps

1. In Design Studio create a project named `Workbench_app` (Qt Quick Application, Qt 6.8 or newer, **Enable CMake Generation** ticked) inside this lesson folder. You get `17_workbench_app/Workbench_app/`.
2. Close the project in Design Studio.
3. Copy the folder `Workbench_app` of this lesson **over** your project. Files with the same name are replaced.
4. Create the file `Workbench_app/Dependencies/CMakeLists.txt` with one comment line in it. Design Studio leaves the folder empty, but the generated `qds.cmake` includes it (see Common Problems).
5. For the real build: open the top `CMakeLists.txt` in Qt Creator and choose a kit with Qt Quick Timeline, Qt Shader Tools and Qt Multimedia installed (section 5).
6. For the design view: open `Workbench_app.qmlproject` in Design Studio.

### 3.2 Generated files that changed

Only three small changes outside the content folder, and one new folder next to it. A C++ class cannot be added without them.

| File | Change |
|------|--------|
| `App/CMakeLists.txt` | Builds the core library, adds the backend files from the content folder to the executable and links `workbench_core` |
| `App/main.cpp` | The organization and application name (for the settings) and one line that registers `WorkbenchBackend` as `Workbench_backend` |
| `Workbench_app.qmlproject` | The environment has `QML_XHR_ALLOW_FILE_READ` and `QML_XHR_ALLOW_FILE_WRITE` (section 4.7) |

New: the folder `Workbench_backend/` (the mock, section 4.7) and the file `Dependencies/CMakeLists.txt` (step 4 above).

Unchanged: the module `Workbench_app` (`Constants`, `EventListModel`, `EventListSimulator`, `qmldir`, `CMakeLists.txt`), the `Workbench_appContent/CMakeLists.txt` apart from the file list, the root `CMakeLists.txt`, `qds.cmake`, `App/autogen/environment.h`, `Dependencies`, `Generated`.

Inside the content folder, `App.qml` is replaced by the application window (it was a `Window` that showed `Screen01`), `Screen01.ui.qml` is changed into the About screen (its button and color animation stay), and `CMakeLists.txt` lists the new QML files.

### 3.3 The files in `Workbench_appContent`

| File or folder | Layer | Purpose |
|----------------|-------|---------|
| `App.qml` | QML | The window, the menu bar, the tabs, the dialogs, the status bar |
| `*Tab.qml` | QML | One file per tab |
| `Theme.qml` | QML | The colors. `dark` switches them |
| `AppState.qml` | QML | The settings and the status text that menus and tabs share |
| `Screen01.ui.qml` | QML | The generated screen, now the About screen shown in the About dialog |
| `backend/workbench_backend.h/.cpp` | bridge | `WorkbenchBackend`, one `QObject` for all tabs |
| `core/unit_converter.*` | core | `convertUnit`, `unitsOf`, `formatNumber`. A table of factors per category, temperature through Celsius |
| `core/color_tools.*` | core | `toHex`, `parseHex` (`#RGB`, `#RRGGBB`, with or without `#`), `toRgbText` |
| `core/countdown.*` | core | `Countdown` (no timer inside: the caller calls `tick()`) and `formatClock` |
| `core/text_tools.*` | core | `findNext`, `countMatches`, `countLines` on UTF-16 text, and `noteFileName` |
| `core/file_io.*` | core | `readTextFile`, `writeTextFile` with `std::expected` and a 5 MB limit |
| `core/file_kind.*` | core | `fileKind`: which tab opens a file, by its extension |
| `core/recent_files.*` | core | `RecentFiles`: newest first, no duplicates, at most 5 |
| `core/core_tests.cpp` | core | The tests (section 6) |
| `../Workbench_backend/` | Design Studio | The mock of `WorkbenchBackend`, outside the content folder (section 4.7) |


---

## 4. Ideas Worth Knowing

### 4.1 Text positions are UTF-16

A `QString` and the QML `TextArea` count characters in UTF-16 units. The text functions of the core therefore take `std::u16string_view`. A position from `findNext` goes straight into `TextArea.select()`. If the core used UTF-8 bytes, every non-ASCII letter before a match would shift the selection.

Files are read as bytes (UTF-8) and the bridge makes the `QString`.

### 4.2 Errors are values

`convertUnit`, `readTextFile` and `writeTextFile` return `std::expected`. The caller must look at the result, and there are no exceptions across the Qt boundary. The bridge stores the error text in `lastError` and QML shows it in the status bar.

### 4.3 The timer logic does not know time

`Countdown::tick(seconds)` is called once per second. In the application a `QTimer` calls it. In the tests a loop calls it with any number of seconds, so the test does not wait.

### 4.4 Shared state without singletons

Menus and tabs need the same settings (word wrap, autosave, status bar, status text) and the same colors. `App.qml` creates one `AppState` and one `Theme` object and gives them to every tab as properties:

```qml
Theme { id: appTheme }
AppState { id: appStore }

TimerTab {
    backend: appBackend
    theme: appTheme
    appState: appStore
}
```

Inside a tab, `theme.card` and `appState.statusText` are used. This takes more typing than a singleton, but it needs no `qmldir` and no change in the generated `CMakeLists.txt`, so Design Studio can write that file again without breaking anything. The ids have different names than the properties (`appTheme` and `theme`): `theme: theme` would point to the property itself.

### 4.5 A toggle button that follows the real state

A checkable `Button` changes `checked` when you click it, and that **removes** a binding such as `checked: player.playing`. A `Binding` item keeps it in step:

```qml
Binding {
    target: playButton
    property: "checked"
    value: player.playbackState === MediaPlayer.PlayingState
}
```

The timer tab uses the same idea, because the countdown also stops by itself at 00:00.

### 4.6 A ComboBox resets when its model changes

In the unit converter the unit lists change with the category. A `ComboBox` goes back to its first entry when its model changes. The first version set the indices too early and the second box stayed on the first unit (found by the test). The fix is to set them after the models were replaced:

```qml
onCurrentTextChanged: Qt.callLater(root.resetUnits)
```

### 4.7 A mock for Design Studio

Design Studio cannot load a C++ class. Without help, `WorkbenchBackend` would be an unknown type and the live preview of `App.qml` would show errors. The folder `Workbench_backend/` (next to `Workbench_app`, outside the content folder) holds a QML file with the **same members**, returning simple sample values. Its `qmldir` says `module Workbench_backend`, which is the folder name seen from the project folder, the name Qt expects (so Qt Creator shows no warning). `App.qml` writes `import Workbench_backend`, and Design Studio finds the mock there.

**Why not inside `Workbench_appContent`?** Design Studio treats every folder with a `qmldir` inside the content folder as a module of its own. It then writes a `CMakeLists.txt` for it and adds it to `qds.cmake`, and the real build fails with `cannot find -lWorkbench_appContent_Backendplugin`. A folder that the `.qmlproject` does not list is not touched by the generator.

The real application never loads the mock: `main.cpp` registers the C++ class under the same module name, and the mock is in no CMake file. (The file is called `MockWorkbenchBackend.qml` so that it can never be mixed up with the C++ type, even if Design Studio lists it in a generated file.)

The mock is **not** the tested logic, but it is simple working QML: the timer counts down, units and colors convert, search works, files are read and written with `XMLHttpRequest`. Qt only allows that for local files if the environment variables `QML_XHR_ALLOW_FILE_READ=1` and `QML_XHR_ALLOW_FILE_WRITE=1` are set. The `.qmlproject` sets them. So the app is usable when you press **Run** in Design Studio.

**Design Studio remembers.** If the mock was ever inside the content folder, Design Studio wrote it into two more generated files: a `Q_IMPORT_QML_PLUGIN(...)` line in `App/autogen/environment.h` and `<file>` lines in `Workbench_app.qrc`. Remove those lines by hand, delete the build folder and run CMake again. Otherwise the linker asks for a plugin that does not exist.

Know which one you run: **Run in Design Studio uses the mock. A build with CMake (Qt Creator or the command line) uses the real C++ class and the core library.** When you add a function to the C++ class, add a line to the mock too, or the preview shows an error.

### 4.8 Two modules

Design Studio makes two QML modules: `Workbench_app` (the generated `Constants` and the event list files) and `Workbench_appContent` (our screens). Our files do not need to import either one for each other, because they are in the same folder and the same module. None of our files imports `Workbench_app`: the generated `Constants.qml` needs the Qt Quick Studio components (`QtQuick.Studio.Application`), which a normal Qt install does not have, and the app would stop at start with `module "QtQuick.Studio.Application" is not installed`. Our files simply do not use `Constants`. Never write `import Workbench_appContent`.

---

## 5. Build and Run

### 5.1 What you need

Qt 6.8 or newer, CMake 3.21 and a C++23 compiler (GCC 13 or newer). The Qt installation must contain these modules, because the generated root `CMakeLists.txt` asks for them:

| Module | Why |
|--------|-----|
| Qt Quick Timeline | In the generated `find_package` line of every Design Studio project |
| Qt Shader Tools | The same line |
| Qt Multimedia | The Video tab |

If one is missing, CMake stops with `Failed to find required Qt component "..."`. Add it with the Qt Maintenance Tool: expand the Qt version, open **Additional Libraries** and tick the module. Each Qt version has its own modules, so check the version that your kit uses.

### 5.2 Run in Design Studio

Open `Workbench_app.qmlproject` and press Run. This uses the mock backend (section 4.7).

### 5.3 Build in Qt Creator

1. **File > Open File or Project**, and choose the `CMakeLists.txt` **in the project folder** (next to `qds.cmake` and the `App` folder). Not the one inside `Workbench_app/` or `Workbench_appContent/`: those are parts of the build and fail alone with `Unknown CMake command "qt_add_library"`.
2. Choose a kit (Qt 6.8 or newer, GCC 13 or newer) and configure the project.
3. **Build** and **Run**. This uses the real C++ backend and the core library.

Qt Creator only lists files that a CMake target names. The headers of the core library and the backend appear because their `CMakeLists.txt` files list them. Design Studio does not write C++ file lists, so you add new C++ files to the lists by hand.

### 5.4 Command line

From the project folder:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=<your Qt folder>
cmake --build build
```

The executable is in `build/`. The name comes from the generated CMake files.

### 5.5 The core tests

They need no Qt. Either switch them on in the project:

```
cmake -S . -B build -DWORKBENCH_BUILD_TESTS=ON
cmake --build build --target core_tests
./build/App/core/core_tests
```

or copy the folder `Workbench_appContent/core` next to a small `CMakeLists.txt` that sets `WORKBENCH_BUILD_TESTS` to `ON` and calls `add_subdirectory(core)`.

---

## 6. How This Was Tested

**The core library** (`core/core_tests.cpp`): 89 checks, no test framework. A failed check prints a line and the program returns 1. They cover:

| Module | Examples |
|--------|----------|
| Units | 12.5 m = 41.0105 ft, 100 °C = 212 °F, unknown unit and unknown category are errors, number format |
| Colors | `#2E7D5A`, clamping, the short form `#fa0`, invalid codes, round trip |
| Countdown | Counting down, stop, start after the end, finished is reported once, total 0, negative total |
| Text | First, next and last match, case rules, empty query, matches that do not overlap, line counting, note file names from titles |
| File kinds | Upper case extensions, Windows paths, a dot in a folder name |
| Recent files | Newest first, the oldest entry drops, move to the front |
| Files | Write creates folders, read gives the same bytes, missing file, folder, too large |

Two of the first test runs failed because my **expectations** were wrong (a text position and the order of the recent list). That was checked by hand before the tests were changed.

**The QML** was loaded with Qt 6.12.0 (PySide6, offscreen). The bridge was replaced by a stand-in with the same functions, and every function called the **real core library** through a small C bridge. A script then did what a user does:

| Tab | What was checked |
|-----|------------------|
| Text | Open a file, content and line count, search, next match, wrap around at the end, match count, missing file reported |
| Image | A PNG opens in the image tab, loads, zoom 100 % and 200 % change the width |
| Video | The file type opens the video tab; unknown types are reported |
| Units | The default pair Meter to Foot, a new value, a category change to Temperature, 100 °C = 212 °F, bad input shows `-` |
| Color | A hex code moves the sliders, an invalid code is refused, RGB text |
| Notes | Suggested file name from the title, save to a chosen path, the path is remembered, dirty flag, autosave writes after 3 seconds, no autosave when switched off |
| Timer | The countdown runs, Stop keeps the time, progress, Reset, a finished countdown resets the button |
| Recent | The list is newest first, Clear list |

The first run found two real bugs, both fixed: the unit boxes after a category change (section 4.6), and a `ScrollView` inside a `Frame` that gave a binding loop warning. The final run printed no QML warnings.

**The C++ bridge** could not be compiled where the tests ran (no Qt C++ files there). Its first real build, in Qt Creator, found one error: `QString::utf16()` returns `ushort` pointers, and `std::u16string_view` needs `char16_t` pointers (see Common Problems). After that fix it built and ran.

---

## 7. Limitations

- Text search only folds the letters A to Z for "ignore case".
- Files larger than 5 MB are refused.
- The Save dialog of the Notes tab starts in your Documents folder with a file name made from the title. Autosave writes to the file you chose and only starts after the first save.
- The dark theme changes the colors and the palette of the Basic style. It is simple and not a full style.
- The Image tab does not scroll: a large image at a high zoom is clipped. Use Fit to window.

---

## 8. Design Studio and the Code

Design Studio is good at the looks: `Screen01.ui.qml` is a screen you can edit visually. The tabs contain logic, so `.ui.qml` files cannot hold them (a `.ui.qml` file may not contain functions or handlers). They are normal `.qml` files that you edit in Qt Creator or in the Design Studio code view. Do not commit `*.qtds` and `*.user*` files (the `.gitignore` of the area ignores them).

---

## Common Problems

- **`QML module not found (Workbench_appContent)`:** an `import` line names the content module. Delete it: files of the content folder see each other without an import.
- **`module "Workbench_backend" is not installed`:** in the real app, `qmlRegisterType` in `App/main.cpp` is missing or has another module name. In Design Studio, the folder `Workbench_backend` with its `qmldir` is missing.
- **Linker: `cannot find -lWorkbench_appContent_Backendplugin`:** an old `Workbench_appContent/Backend` folder is still there and Design Studio made a module of it. Delete that folder, restore `qds.cmake` (it lists only `Workbench_app`, `Workbench_appContent`, `App` and `Dependencies`) and run CMake again.
- **Start error `module "QtQuick.Studio.Application" is not installed`:** some file uses `Constants` from the module `Workbench_app`. Remove that use (see section 4.8).
- **The headers (`*.hpp`) are not in the Qt Creator project tree:** Qt Creator only shows files that a CMake target lists. `core/CMakeLists.txt` lists the headers next to the `.cpp` files, and the backend header is listed in `App/CMakeLists.txt`. Design Studio does not write any C++ file lists. Add new C++ files to those lists by hand.
- **CMake: `Failed to find required Qt component "QuickTimeline"` (or `ShaderTools`, `Multimedia`):** the Qt install lacks that module. Add it in the Qt Maintenance Tool under the Qt version, in **Additional Libraries**, and use that Qt version's kit in Qt Creator.
- **CMake: `The source directory .../Dependencies does not contain a CMakeLists.txt`:** the generated `qds.cmake` includes the folder `Dependencies`, but Design Studio left it empty. Create a `Dependencies/CMakeLists.txt` that contains only a comment line.
- **Compiler: `invalid conversion from 'const ushort*' to 'const char16_t*'`:** `QString::utf16()` returns `ushort` pointers. The helper `view()` in `backend/workbench_backend.cpp` casts them with `reinterpret_cast<const char16_t *>`.
- **Linker: `undefined reference to qt_static_plugin_..._BackendPlugin`:** an old line `Q_IMPORT_QML_PLUGIN(Workbench_appContent_BackendPlugin)` is still in `App/autogen/environment.h`, and old `<file>` lines are in `Workbench_app.qrc` (section 4.7). Delete them, delete the build folder, run CMake.
- **CMake: `Unknown CMake command "qt_add_library"`:** you opened a `CMakeLists.txt` that is only a part of the project. Open the one in the project folder (section 5.3).
- **`theme` or `appState` is null in a tab:** `App.qml` must pass `theme: appTheme` and `appState: appStore` to every tab (section 4.4).
- **Warning `Missmatching uri` for a `qmldir`:** the name after `module` must be the folder path seen from the project folder. For the mock it is `Workbench_backend`. Do not add a `qmldir` to `Workbench_appContent` itself.
- **`pipewire`, `PulseAudioService`, `Gtk-WARNING infinite surface size` in the output:** noise from the audio system and the GTK file dialog on your machine. They do not come from this project and cannot be switched off from QML. Qt Creator shows every line on the error output in red, even when the program only writes a message.
- **`No audio device detected`:** the Video tab checks for an audio output (`MediaDevices`). Without one it creates no `AudioOutput`, plays silently and shows "No audio device" instead of the volume label. The `.qmlproject` also hides the FFmpeg messages (`qt.multimedia.ffmpeg=false` in `QT_LOGGING_RULES`).
- **`XMLHttpRequest: Using GET on a local file is disabled`:** the preview mock reads files this way. The `.qmlproject` sets `QML_XHR_ALLOW_FILE_READ` and `QML_XHR_ALLOW_FILE_WRITE`. If you run the mock some other way (for example the `qml` tool), set them yourself.
- **The preview shows an error for a backend function:** the mock lacks that function (section 4.7).
- **No sound or video:** the machine needs an audio device and the multimedia backend of Qt. Check **Help > About Qt** and the Qt Multimedia messages in the console.
- **`std::expected` or `<format>` not found:** the compiler is too old. GCC 13 or newer is needed.
- **The text tab selects the wrong part of a text with accents or emoji:** the position must be in UTF-16 units. Do not replace `std::u16string_view` with UTF-8.

## Reference

- **Qt modules:** Qt Quick, Qt Quick Controls, Qt Quick Layouts, Qt Quick Dialogs, Qt Multimedia, `QtQuick.Effects`
- **Qt version:** Qt 6.8 or newer
- **C++:** C++23 (`std::expected`, `std::format`, `std::filesystem`, `std::u16string_view`)
- **QML types:** `ApplicationWindow`, `MenuBar`, `Menu`, `Action`, `Instantiator`, `TabBar`, `StackLayout`, `FileDialog`, `MediaPlayer`, `VideoOutput`, `MultiEffect`, `Binding`, `Timer`
- **C++ Qt classes:** `QObject`, `Q_PROPERTY`, `Q_INVOKABLE`, `QTimer`, `QSettings`, `QStandardPaths`
- **CMake:** `qt_add_qml_module`, `qt_add_executable`, `add_library`, `target_sources`, `find_package`
- **Design Studio:** `.qmlproject` environment, `.ui.qml` files, `qmlRegisterType`, the CMake generator
