# 04_property_bindings

A **property binding** is the most important idea of QML. You do not write "when the slider moves, change the box". You write "the box is as wide as the slider", and Qt keeps it true. This lesson shows a binding, the way an assignment **breaks** it, how to restore it, and the `Binding` type.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with two small changes for that old version, which are not part of this folder). An automated check moved the slider, clicked the buttons and the check box, and compared the sizes of the boxes (17 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A window with a slider at the top and three colored boxes under it:

1. A **blue** box whose width is the slider value and whose height is half of its width.
2. A **green** box with the same binding, and two buttons: **Assign width = 100** and **Restore binding**.
3. An **orange** box that follows the slider only while the check box **Follow the slider** is ticked.

Move the slider and watch which boxes follow. The `Slider` comes from Qt Quick Controls (lesson 06); here it is only a source of a changing number.

---

## 2. What a Binding Is

```qml
Rectangle {
    width: slider.value      // a binding: an expression after the colon
    height: width / 2        // a binding that uses another property of the same item
}
```

The text after the colon is not a value that is calculated once. It is an **expression that Qt remembers**. While it is evaluated, Qt notes which properties it reads (`slider.value`). Each of them has a change signal. When one of them changes, Qt evaluates the expression again and writes the new result into the property. So the property is always up to date, with no code that you have to call.

Consequences:

- Bindings can depend on other bindings: `height` depends on `width`, which depends on `slider.value`.
- The expression can be more than a name: `text: qsTr("size: %1 x %2").arg(box.width).arg(box.height)` is a binding too.
- Use bindings for anything that is **derived** from other values. You can delete most of the "update" code of other toolkits.
- Qt evaluates a binding again only when something it read has changed, and only the bindings that are affected.
- If two bindings depend on each other in a circle, Qt reports a warning: `Binding loop detected for property "width"`.

---

## 3. Assignment Breaks a Binding

```qml
onClicked: brokenBox.width = 100
```

This is JavaScript. It writes the value `100` into `width` **and removes the binding**. From now on, `width` is a plain number. When the slider moves, the green box stays at 100. The binding is not paused, it is deleted.

This is the most common mistake of beginners: a binding that works at the start and stops later, because a handler somewhere assigns a value to the property. If a property seems to stop following its source, look for an assignment.

What to do when you need to set a value from a handler and keep the binding:

| Goal | Tool |
|------|------|
| Set a value once, and never bind again | A plain assignment (that is fine) |
| Put a binding back | `brokenBox.width = Qt.binding(function () { return slider.value })` |
| A binding that is only active at some times | The `Binding` type (section 4) |
| A value that the user changes and the program also wants to change | A separate property that holds the state, and bind the item to it |

Note the difference of the two forms:

```qml
width: slider.value                     // in a QML object: a binding
brokenBox.width = slider.value          // in JavaScript: ONE assignment of the current value
brokenBox.width = Qt.binding(function () { return slider.value })   // in JavaScript: a binding
```

---

## 4. The `Binding` Type

```qml
Rectangle { id: gatedBox; width: 100 }

Binding {
    target: gatedBox
    property: "width"
    value: slider.value
    when: follow.checked
}
```

A `Binding` object **applies** a value to a property of another object. While `when` is `true`, `gatedBox.width` follows the slider. When `when` becomes `false`, the property goes back to what it was before: here the orange box returns to its own `width: 100`, and later changes of the slider do not move it. (The `restoreMode` property of `Binding` can change this: for example, `Binding.RestoreNone` keeps the last value.)

Use it when:

- the binding must be active only under a condition,
- you cannot write the binding in the object itself (for example the target is in another file, or it is created in C++),
- several sources may drive one property, and only one at a time (several `Binding` objects with different `when`).

The check box is also an example of a binding: `follow.checked` is read inside `when`, so the `Binding` switches by itself.

---

## 5. Rules and Habits

- **Keep expressions short and free of side effects.** A binding is evaluated often. Do not change other properties inside it, and do not call slow functions.
- **A binding sees `id` names of the same file** and all properties of its own object and parents.
- **Do not assign to a property that has a binding**, unless you want to remove it. Use a separate property or the `Binding` type.
- **Qt tells you about a loop** in the console. Read the message: it names the property.
- **Use `readonly property`** for values that must only be calculated: `readonly property real area: width * height` cannot be overwritten by mistake.
- **A binding is not a copy.** `width: slider.value` follows. `width = slider.value` in a handler copies the value of one moment.

---

## 6. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app04_property_bindings
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Move the slider. Which boxes follow?
2. Click **Assign width = 100**, move the slider, then click **Restore binding**.
3. Tick **Follow the slider**, move the slider, then untick the box. What is the width of the orange box now? Move the slider again.
4. Add `restoreMode: Binding.RestoreNone` to the `Binding` and repeat step 3.
5. Add `readonly property real area: width * height` to the blue box and show it in the text next to it.
6. Create a binding loop on purpose: `width: height` and `height: width` in the same rectangle. Read the message in the output.

---

## 7. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| A property stops following its source | An assignment somewhere removed the binding | Search for `property = ...` in handlers; use `Qt.binding` or `Binding` |
| `Binding loop detected for property "..."` | Two or more bindings depend on each other | Remove one of the dependencies |
| A value is not updated | The expression reads something that has no change signal (for example a plain JavaScript variable) | Use a `property` of an object, so QML can watch it |
| `Binding` has no effect | `target` is wrong, or `property` has a typo (it is a string) | Check both; `target` must be an object, not a string |
| The binding runs too often and the interface is slow | A heavy function in the expression | Calculate in a property that changes seldom, or move the work to C++ |
| `ReferenceError: slider is not defined` | The `id` is in another file | `id` is only visible in its own file; pass the value through a property |
| `Unable to assign [undefined] to int` | The expression returned `undefined` (for example a missing property) | Check the names |

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Property binding | <https://doc.qt.io/qt-6/qtqml-syntax-propertybinding.html> |
| `Binding` | <https://doc.qt.io/qt-6/qml-qtqml-binding.html> |
| `Qt.binding()` | <https://doc.qt.io/qt-6/qml-qtqml-qt.html#binding-method> |
| Binding loops | <https://doc.qt.io/qt-6/qtqml-syntax-propertybinding.html#binding-loops> |
| `Slider` | <https://doc.qt.io/qt-6/qml-qtquick-controls-slider.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`); `Slider` and `CheckBox` come from Qt Quick Controls |
| Imports | `import QtQuick`, `import QtQuick.Controls` |
| Minimum Qt version | Qt 6.8 |
