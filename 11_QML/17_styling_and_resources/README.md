# 17_styling_and_resources

A Qt Quick app looks the way its **style** says. This lesson shows how to choose a style, how to change its theme and accent, how to build an own look with a **Theme singleton**, and how images, fonts and other files get into the program as **resources**.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder). An automated check loaded the window, looked at the image status, switched the dark mode (the window colour and the Theme changed), and changed the font family (11 checks, all passed). The sandbox had no SVG plugin, so the lesson uses a PNG. Statements about the order of the style selection come from the Qt documentation; only the points marked "tested" were tried. Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A window with the standard controls in the Material style, a switch for dark mode, a choice of the accent colour, a card and a button with an own look, images (one is missing on purpose), and a font selection.

| File | Content |
|------|---------|
| `Main.qml` | The window and all sections |
| `Theme.qml` | Singleton with colours, sizes and fonts of the own look |
| `images/logo.png` | An image in the module resources |
| `qtquickcontrols2.conf` | Settings of the style, as a resource in the root |

---

## 2. Choosing the Style

Qt Quick Controls has several styles: **Basic**, **Fusion**, **Imagine**, **Material**, **Universal**, and native ones for some platforms (**Windows**, **macOS**, **iOS**, and in newer Qt versions **FluentWinUI3**). Which are available depends on your Qt version and platform.

### 2.1 At run time

`import QtQuick.Controls` uses the style that is set when the program starts. According to the Qt documentation, these ways exist, from the strongest to the weakest:

| # | Way | Example |
|---|-----|---------|
| 1 | In C++ before the engine is created | `QQuickStyle::setStyle("Material")` (header `<QQuickStyle>`, module Qt Quick Controls 2, `Qt6::QuickControls2`) |
| 2 | Command line | `./app17_styling_and_resources -style Fusion` |
| 3 | Environment variable | `QT_QUICK_CONTROLS_STYLE=Fusion ./app17_...` |
| 4 | Configuration file | `qtquickcontrols2.conf` (this lesson) |

The style cannot change while the program runs. A user setting for "style" needs a restart.

The configuration file is the nicest for a tutorial, because it needs no C++:

```ini
[Controls]
Style=Material

[Material]
Theme=Light
Accent=Teal
Primary=BlueGrey
```

It must be in the **root** of the resources (`:/qtquickcontrols2.conf`). In CMake:

```cmake
qt_add_resources(app17_styling_and_resources conf
    PREFIX "/"
    FILES qtquickcontrols2.conf
)
```

Tested: when `Main.qml` had no style import at all, the file alone gave the Material style.

### 2.2 At compile time

`import QtQuick.Controls.Material` selects the style directly. The documentation says: this **overrides** the run-time selection, and you cannot change the style then. Tested: with this import, `QT_QUICK_CONTROLS_STYLE=Basic` did not change the look. In the same test, a file with `import QtQuick.Controls.Material` **and** `import QtQuick.Controls.Universal` was drawn in the Universal style, the last import. The documentation warns against two styles in one application. Import **one** style.

This lesson imports Material, because it uses its attached properties (next section). The conf file gives the defaults for the theme and accent, and it is the file that would choose the style if the import were removed.

To try the run-time selection: remove the line `import QtQuick.Controls.Material` and the three `Material.` lines in `Main.qml` (and the `Binding` stays as it is), build, and start the program with `-style Fusion` or with the environment variable. Then the file `qtquickcontrols2.conf` is the weakest vote, and your choice wins.

---

## 3. Theme and Accent

Material (and Universal) have two big switches:

```qml
Material.theme: darkSwitch.checked ? Material.Dark : Material.Light
Material.accent: accents[accentBox.currentIndex]     // Material.Teal, Material.Orange, ...
```

They are **attached properties** (lesson 16). They are set on the window, and every control inside inherits them. A control can set them again to be different from its parent: for example a dark toolbar in a light window. Universal has the same names (`Universal.theme`, `Universal.accent`) with its own colour list.

---

## 4. Your Own Look

Do not repeat `"#1f9d8f"` in thirty places. Put the values in one **singleton**:

```qml
// Theme.qml
pragma Singleton
import QtQuick

QtObject {
    property bool dark: false
    readonly property int radius: 10
    readonly property color card: dark ? "#2b2f36" : "#eef2f7"
}
```

Two more things are needed so that QML finds it:

```cmake
set_source_files_properties(Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)   # before qt_add_qml_module
qt_add_qml_module(... QML_FILES Main.qml Theme.qml ...)
```

Now every file of the module can write `Theme.card`. `Main.qml` sets `Theme.dark` with a `Binding` (lesson 04), so the card follows the switch. The test checked that.

A button with its own look replaces two parts of the control: `contentItem` (what is inside) and `background`:

```qml
Button {
    id: ownButton
    contentItem: Label { text: ownButton.text; color: "white" }
    background: Rectangle { color: ownButton.down ? Qt.darker(Theme.brand, 1.3) : Theme.brand }
}
```

Use the control's own properties (`down`, `hovered`, `checked`, `enabled`) for the states. This works in the Basic, Fusion, Imagine, Material and Universal styles. The native styles of Windows and macOS are made to look like the system and, according to the Qt documentation, they are not meant for customization of this kind; use another style when you need your own look.

---

## 5. Images

```qml
Image {
    source: "images/logo.png"        // relative to this QML file
    sourceSize.width: 96
    sourceSize.height: 96
    fillMode: Image.PreserveAspectFit
}
```

- **Relative path.** The file is part of the module, because `CMakeLists.txt` lists it under `RESOURCES`. The path is relative to the QML file, like on a disk.
- **`status`** tells what happened: `Image.Null`, `Image.Loading`, `Image.Ready`, `Image.Error`. The second image points to a file that does not exist; the label shows "Image error", and the console prints `Cannot open: ...`. Always look at the console when an image does not show.
- **`sourceSize`** limits the size to which the picture is decoded. A 4000 pixel photo in a 100 pixel box needs less memory with `sourceSize`. For a SVG it decides at which size it is drawn.
- **`fillMode`**: `Stretch` (default), `PreserveAspectFit`, `PreserveAspectCrop`, `Tile` ...
- **SVG** works when the Qt SVG plugin is installed (it is in a Qt from the installer). A vector image stays sharp at every size.
- **Icons on controls**: `icon.source`, `icon.width`, `icon.color`. `icon.color: "transparent"` keeps the colours of the file; a colour recolours a one-colour icon.
- **Images from the network**: `source: "https://..."` loads in the background, `status` is `Loading` until it is there. Use `asynchronous: true` for big local files.

---

## 6. Resources

A resource is a file that is **inside the executable**. The program finds it on every computer, with no paths to the install folder.

| Way | Use | Path in the program |
|-----|-----|---------------------|
| `qt_add_qml_module(... RESOURCES file)` | Files that belong to a QML module (images, JSON, sounds) | relative to the QML file (the best way). The full path starts with `qrc:/qt/qml/<URI>/` when the project uses `qt_standard_project_setup(REQUIRES 6.8)` |
| `qt_add_resources(target name PREFIX "/x" FILES ...)` | Any other file, with a prefix of your choice | `qrc:/x/file` in QML, `":/x/file"` in C++ |

Lesson 15 used the second way for a JSON file, and this lesson uses both. Remember:

- A resource is **read only**. A program that must write (settings, user data) uses `QStandardPaths` (chapter `10_IO_and_Data`).
- Every file you add is in the executable, so a big video as a resource makes a big program. Put big files next to the program and load them with a path.
- After you add a file to the list, **run CMake again** (Qt Creator does it when you save the file). A file that is not in the list is not found at run time.

---

## 7. Fonts

```qml
Label {
    font.family: "Monospace"
    font.pixelSize: Theme.fontLarge
    font.italic: true
}
```

- `font.family`, `font.pixelSize` (fixed) or `font.pointSize` (follows the screen settings), `font.bold`, `font.italic`, `font.weight`, `font.capitalization`.
- A generic name like `"Monospace"` or `"Sans Serif"` lets the system choose. A name that is not installed is replaced silently, so the text still shows, but in another font.
- `Qt.fontFamilies()` lists the installed families. The last label shows how many.
- Set the font of **all** controls of a window at once with `ApplicationWindow { font.pixelSize: 14 }`; children inherit it.

### 7.1 Own fonts

To ship a font with the program, put the file (`.ttf`, `.otf`) into the resources and load it:

```qml
FontLoader { id: brandFont; source: "fonts/Brand-Regular.ttf" }
Label { font.family: brandFont.name }
```

In C++ the same is `QFontDatabase::addApplicationFont(":/fonts/Brand-Regular.ttf")`. **Check the licence of the font** before you put it into a public repository or a product: many free fonts allow this, some do not. This lesson has no font file for that reason.

---

## 8. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app17_styling_and_resources
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Switch the dark mode and change the accent. Which controls follow, and which parts of the "own look" follow?
2. Change `Accent=Teal` in `qtquickcontrols2.conf`. Nothing changes? Look at `Material.accent` in `Main.qml`: the QML value is stronger than the file.
3. Remove the Material import and the `Material.` lines, and start with `-style Fusion`.
4. Add a second colour, `warning`, to `Theme.qml` and use it for a label.
5. Replace `logo.png` with your own picture of the same name.
6. Change `Theme.radius` to 24 and watch the card and the button.

---

## 9. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `Material is not a type` / `Material.theme` unknown | The style module is not imported (or not installed) | `import QtQuick.Controls.Material` |
| `-style` or the environment variable changes nothing | A style is imported in the QML code (compile-time selection) | Remove the import, or choose the style in the import |
| Two styles are mixed up | Two style imports in the program | Import only one style |
| The conf file is ignored | It is not in the resource root, or the name is wrong | `qt_add_resources(... PREFIX "/" FILES qtquickcontrols2.conf)`, name exactly `qtquickcontrols2.conf` |
| `Theme` is not defined | `QT_QML_SINGLETON_TYPE` is missing, or it is set after `qt_add_qml_module` | Set the property before, and list the file in `QML_FILES` |
| The image does not show | Wrong path, file not listed under `RESOURCES`, or an unknown format | Read the console; check `status` |
| An SVG is not drawn | The SVG plugin is missing in your Qt | Install it (module Qt SVG) or use a PNG |
| A custom `background` is ignored | A native style (Windows, macOS) is active | Choose Basic, Fusion, Material, Universal or Imagine |
| The font looks different on another computer | The family is not installed there | Ship the font (check the licence) or use a generic name |
| The text of a control does not follow the theme | The colour was set to a fixed value | Use `palette` or the `Theme` singleton and keep the dark colours in the same place |

---

## 10. Reference

| Topic | Where to look |
|-------|---------------|
| Styling Qt Quick Controls (selection of the style) | <https://doc.qt.io/qt-6/qtquickcontrols-styles.html> |
| Customizing Qt Quick Controls | <https://doc.qt.io/qt-6/qtquickcontrols-customize.html> |
| Material style | <https://doc.qt.io/qt-6/qtquickcontrols-material.html> |
| Universal style | <https://doc.qt.io/qt-6/qtquickcontrols-universal.html> |
| Configuration file | <https://doc.qt.io/qt-6/qtquickcontrols-configuration.html> |
| `Image` | <https://doc.qt.io/qt-6/qml-qtquick-image.html> |
| `FontLoader` | <https://doc.qt.io/qt-6/qml-qtquick-fontloader.html> |
| QML singletons | <https://doc.qt.io/qt-6/qml-singleton.html> |
| The Qt resource system | <https://doc.qt.io/qt-6/resources.html> |
| `QQuickStyle` | <https://doc.qt.io/qt-6/qquickstyle.html> |
| Module | Qt Quick Controls (`find_package(Qt6 REQUIRED COMPONENTS Quick)`; `QuickControls2` only for `QQuickStyle`) |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Controls.Material`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
