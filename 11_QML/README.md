# 11_QML

Qt Quick and QML from the first window to C++ integration, animations, navigation and testing.

**Developed with Qt 6.10.2 and also tested with Qt 6.12.0. Requires Qt 6.8 LTS or newer** (the generated `CMakeLists.txt` asks for 6.8 with `qt_standard_project_setup(REQUIRES 6.8)`). Features that need a newer version are marked in their README.

QML is Qt's declarative language for building user interfaces. You describe the interface as a tree of items, and Qt Quick draws and animates it. The logic lives in C++ and is exposed to QML as properties, methods, signals and models. This chapter starts with pure QML, then adds the C++ side.

---

## 1. Topics

| # | Folder | What it teaches |
|---|--------|-----------------|
| 01 | `01_first_qml_window` | Qt Quick Application template, `QQmlApplicationEngine`, `Main.qml`, `qt_add_qml_module` |
| 02 | `02_items_and_properties` | `Rectangle`, `Text`, `Image`, `id`, `parent`, custom properties, property types and enums |
| 03 | `03_anchors_and_layouts` | `anchors`, `Row`, `Column`, `RowLayout`, `GridLayout`, attached properties such as `Layout.fillWidth` |
| 04 | `04_property_bindings` | Automatic updates, how an assignment breaks a binding, the `Binding` type |
| 05 | `05_signals_and_handlers` | `onClicked`, custom signals, `Connections`, `MouseArea`, `TapHandler`, `Keys` and focus, `Timer` |
| 06 | `06_quick_controls` | `Button`, `TextField`, `CheckBox`, `Slider`, `ComboBox` |
| 07 | `07_custom_components` | Your own QML types in separate files, `property alias`, `Component` |
| 08 | `08_states_and_transitions` | `State`, `Transition`, behaviors |
| 09 | `09_animations` | `NumberAnimation`, `ColorAnimation`, animation sequences |
| 10 | `10_models_and_views` | `ListModel`, `Repeater`, `ListView`, delegates |
| 11 | `11_javascript_in_qml` | Functions, `Component.onCompleted`, when to move logic to C++ |
| 12 | `12_cpp_properties_and_methods` | `Q_PROPERTY` with `NOTIFY`, `Q_INVOKABLE`, `QML_ELEMENT`, why a method call in a binding does not update |
| 13 | `13_cpp_signals_to_qml` | C++ signals with arguments, `Connections`, a `QTimer` in C++, `QML_SINGLETON` |
| 14 | `14_cpp_list_models` | `QAbstractListModel` with roles, inserting and removing rows, `QAbstractItemModelTester` |
| 15 | `15_network_and_data` | A C++ backend with `QNetworkAccessManager` and JSON feeding a list; loading and error state; resources |
| 16 | `16_navigation` | `StackView`, `SwipeView`, `TabBar`, `Drawer`, `Loader`, attached properties |
| 17 | `17_styling_and_resources` | Choosing a style (`qtquickcontrols2.conf`, run time and compile time), Material theme and accent, a `Theme` singleton, fonts, images, resource files |
| 18 | `18_testing_qml` | Qt Quick Test (`TestCase`, data-driven tests, `SignalSpy`), `ctest`, `qmllint`, `qmlformat`, the QML Profiler |
| 19 | `19_project_dashboard` | Project: an animated dashboard with a C++ backend, states, transitions and an alarm message |
| 20 | `20_project_mvvm` | Project: MVVM with a plain C++ model, a ViewModel exposed to QML, and C++ tests without a window |
| 21 | `21_project_mvc` | Project: MVC with a Qt model, a controller and a QML view, and how it differs from MVVM |

Every folder contains a `README.md` (explanation and code), `main.cpp`, `Main.qml` and a `CMakeLists.txt`. From lesson 12 on, a folder also has the C++ classes of the lesson, and from lesson 11 on, extra QML files, a data file or a `tests/` folder where the lesson needs them. The last section of every README is a Reference table.

Lessons 18, 20 and 21 have a test program. Run it from the lesson folder with `ctest --test-dir build --output-on-failure`; the tests need no screen.

---

## 2. Naming Rules

Every example in this chapter follows the same three rules.

| Item | Rule | Example (`02_items_and_properties`) |
|------|------|--------------------------------------|
| Folder and project name | `NN_topic_name`, lowercase | `02_items_and_properties` |
| Executable | `app` + project name | `app02_items_and_properties` |
| QML module URI | Topic name **without** the number | `items_and_properties` |

Why the URI has no number: a QML module URI must start with a letter. A name such as `02_items_and_properties` is rejected by CMake with the error `URI must start with letter`.

Why the executable has the `app` prefix: the executable and the QML module are two separate CMake targets. The prefix keeps their names from clashing.

The URI is used in two places, and both must match:

```cmake
qt_add_qml_module(app02_items_and_properties
    URI items_and_properties
    ...
```

```cpp
engine.loadFromModule("items_and_properties", "Main");
```

---

## 3. Qt Creator: C++23, CMake 3.21 and a Valid URI by Default

The Qt Creator template "Qt Quick Application" generates `cmake_minimum_required(VERSION 3.16)`, no `CMAKE_CXX_STANDARD` line, and a URI taken from the project name. For this tutorial we want CMake 3.21, C++23 and a URI that starts with a letter.

Instead of editing every project by hand, make a user-level copy of the template once. A copy in your user folder with the same `wizard.json` replaces the original in the wizard, and the system files stay untouched.

### 3.1 Find the template

```bash
S=/usr/share/qtcreator/templates/wizards/projects
D=~/.config/QtProject/qtcreator/templates/wizards/projects

ls "$S" | grep -i quick
```

You should see `qtquickapplication`. (`qtquickapplication_compat` is the variant for older Qt versions, and you do not need it.)

### 3.2 Copy it and set CMake 3.21 and C++23

```bash
name=qtquickapplication
mkdir -p "$D"
[ -d "$D/${name}23" ] || cp -r "$S/$name" "$D/${name}23"
f="$D/${name}23/CMakeLists.txt"

# minimum version -> 3.21
sed -i -E 's/^cmake_minimum_required\(VERSION [0-9.]+\)/cmake_minimum_required(VERSION 3.21)/' "$f"
# an existing standard line -> 23
sed -i -E 's/^set\(CMAKE_CXX_STANDARD [0-9]+\)/set(CMAKE_CXX_STANDARD 23)/' "$f"
# no standard line yet -> add it before the REQUIRED line
grep -q 'CMAKE_CXX_STANDARD 23' "$f" || \
  sed -i 's/^set(CMAKE_CXX_STANDARD_REQUIRED ON)/set(CMAKE_CXX_STANDARD 23)\nset(CMAKE_CXX_STANDARD_REQUIRED ON)/' "$f"

grep -nE 'cmake_minimum|CXX_STANDARD' "$f"
```

The last command should print:

```
cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

### 3.3 Make the URI start with a letter

The template builds the URI from the project name. This patch drops leading digits and underscores from it, in both the CMake file and `main.cpp`:

```bash
T="$D/${name}23"

sed -i "s#URI %{ProjectName}#URI %{JS: value('ProjectName').replace(/^[0-9_]+/, '')}#" "$T/CMakeLists.txt"
sed -i "s#value('ProjectName')}#value('ProjectName').replace(/^[0-9_]+/, '')}#" "$T/main.cpp"

grep -n 'URI\|loadFromModule' "$T/CMakeLists.txt" "$T/main.cpp"
```

The check should print:

```
CMakeLists.txt:26:    URI %{JS: value('ProjectName').replace(/^[0-9_]+/, '')}
main.cpp:28:    engine.loadFromModule("%{JS: value('ProjectName').replace(/^[0-9_]+/, '')}", "Main");
```

Restart Qt Creator after the change. Now a project named `01_first_qml_window` gets the URI `first_qml_window` automatically, in both places.

---

## 4. Creating a Project

1. **File → New Project → Application (Qt) → Qt Quick Application** (not the "compat" variant).
2. Name the project `NN_topic_name`, for example `01_first_qml_window`, and choose this folder as the location.
3. Select a Qt 6.8 or newer kit.
4. Finish, and leave the generated files unchanged.

The generated `CMakeLists.txt` then looks like this:

```cmake
cmake_minimum_required(VERSION 3.21)

project(01_first_qml_window VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app01_first_qml_window
    main.cpp
)

qt_add_qml_module(app01_first_qml_window
    URI first_qml_window
    QML_FILES
        Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app01_first_qml_window PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app01_first_qml_window
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app01_first_qml_window
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS app01_first_qml_window
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
```

### The generated CMake file, line by line

| Line | Meaning |
|------|---------|
| `cmake_minimum_required(VERSION 3.21)` | Oldest CMake version that may build the project (changed from 3.16 by the template fix) |
| `project(... VERSION 0.1 LANGUAGES CXX)` | Project name, version and language |
| `set(CMAKE_CXX_STANDARD 23)` | Compile as C++23 (added by the template fix) |
| `set(CMAKE_CXX_STANDARD_REQUIRED ON)` | Fail instead of silently falling back to an older standard |
| `find_package(Qt6 REQUIRED COMPONENTS Quick)` | Find Qt 6 with the Quick module (Qt Quick, which also pulls in Qml and Gui) |
| `qt_standard_project_setup(REQUIRES 6.8)` | Qt's recommended defaults for a project, and a check that Qt is at least 6.8 |
| `qt_add_executable(app... main.cpp)` | Creates the executable from `main.cpp` |
| `qt_add_qml_module(... URI ... QML_FILES Main.qml)` | Packages the QML files into a module and registers it under the given URI |
| `set_target_properties(...)` | Settings for macOS and Windows (bundle, no console window), harmless on Linux |
| `target_link_libraries(... Qt6::Quick)` | Links the executable to Qt Quick |
| `include(GNUInstallDirs)` and `install(...)` | Install rules for `cmake --install` |

---

## 5. Build and Run

From inside an example folder:

```bash
cmake -S . -B build
cmake --build build
./build/app<NN_topic_name>
```

For example `./build/app01_first_qml_window`. You can also open the example's `CMakeLists.txt` in Qt Creator, select a kit and press **Run**.

---

## 6. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `URI must start with letter` at `qt_add_qml_module` | The URI starts with a digit | Remove the number from the URI (see section 2), or apply the template fix in section 3.3 |
| The window stays empty or the app exits immediately, with a message that the module was not found | The URI in `CMakeLists.txt` and the one in `loadFromModule` differ | Make both names identical |
| `Qt6 ... Quick` not found | The Qt Quick development package is missing | On Ubuntu install `qt6-declarative-dev` |
| QML imports such as `QtQuick.Controls` are missing at run time | The QML runtime modules are not installed | On Ubuntu install the matching `qml6-module-*` packages, for example `qml6-module-qtquick-controls` |
| `qt_standard_project_setup` complains about the Qt version | Your Qt is older than 6.8 | Use a Qt 6.8 or newer kit, or lower the number in `REQUIRES` |
| The new project still gets C++17 and CMake 3.16 | Qt Creator is still using the original template | Check that the `qtquickapplication23` folder exists in your user template folder, then restart Qt Creator |
| `Could NOT find Qt6TaskTree` and a warning about `Qt6QmlAssetDownloaderPrivate` (Qt 6.12 installed with the online installer) | The Essentials install of Qt 6.12 does not contain the Task Tree module (a technology preview), which a private QML plugin looks for | Harmless, the project still builds. To remove it, add the component with `~/Qt/MaintenanceTool ... install qt.qt6.6120.addons.qttasktree`, then re-run CMake |

---

## 7. Development Machine

All examples in this chapter were built and run on this machine.

| Item | Version |
|---|---|
| Operating system | Ubuntu 26.04.1 LTS |
| Compiler | GCC 15.2.0 |
| C++ standard | C++23 |
| Qt (default) | 6.10.2 (Ubuntu package) |
| Qt (also tested) | 6.12.0 (Qt online installer, kept separate from the system Qt) |
| IDE | Qt Creator 18.0.2 (built on Qt 6.10.2) |
| CMake | 4.2.3 |

The project files only require Qt 6.8 and CMake 3.21, so older versions work as well. Qt 6.12 itself needs CMake 3.25 or newer.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Qt Quick overview | <https://doc.qt.io/qt-6/qtquick-index.html> |
| QML language reference | <https://doc.qt.io/qt-6/qtqml-index.html> |
| `qt_add_qml_module` | <https://doc.qt.io/qt-6/qt-add-qml-module.html> |
| `qt_standard_project_setup` | <https://doc.qt.io/qt-6/qt-standard-project-setup.html> |
| Qt Quick Controls | <https://doc.qt.io/qt-6/qtquickcontrols-index.html> |
| Exposing C++ to QML | <https://doc.qt.io/qt-6/qtqml-cppintegration-overview.html> |