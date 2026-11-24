# 11_javascript_in_qml

QML uses **JavaScript** for everything that is not a declaration: the expressions in bindings, the code in signal handlers, functions, and whole `.js` files. This lesson builds a sign-up form that validates its input, and uses it to show where JavaScript belongs in a Qt application, and where it does not.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with two small changes for that old version, which are not part of this folder). An automated check typed into the four fields, read the error messages, and watched the **Submit** button and the welcome line (16 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A form with **Name**, **E-mail**, **Age** and **Password**. Under each field, a red line tells what is wrong, but only after the user has typed something. The **Submit** button is enabled when all four fields are valid, and then it shows a welcome line.

The rules are in `validation.js`: a name of at least 2 characters, an address like `a@b.cc`, a whole age from 18 to 120, and a password with 8 characters and a digit.

---

## 2. Where JavaScript Appears in QML

| Place | Example |
|-------|---------|
| An **expression** in a binding | `text: touched ? message : ""` |
| A **signal handler** | `onClicked: root.result = ...` |
| A **function** in an object | `function allValid() { ... }` |
| A **`.js` file** | `import "validation.js" as Validation` |
| The start of an object | `Component.onCompleted: { ... }` |

The language is **JavaScript** (the ECMAScript standard that Qt's own engine, V4, implements). The syntax of QML is **not** JavaScript, only the code parts are: `property`, `signal` and the `Item { }` blocks are QML. Qt has no browser: there is no `window`, no `document`, no `fetch` of a browser. It has `console.log()`, `JSON`, `Math`, `Date`, `RegExp`, and the global `Qt` object with helpers like `Qt.binding`, `Qt.formatDateTime`, `Qt.callLater` and `Qt.openUrlExternally`.

Use `const` and `let` and arrow functions (`x => x * 2`) like in modern JavaScript, and `===` instead of `==`.

---

## 3. Functions

```qml
function allValid() {
    return nameMessage === "" && emailMessage === "" && ageMessage === "" && passwordMessage === ""
}
```

A function in an object is a **method**: other objects can call it (`root.allValid()`). It can read the ids and properties of the file.

**A function call inside a binding is also a binding.** `enabled: root.allValid()` is evaluated again when **any property that the function reads** changes: the four messages. Qt finds the dependencies while the function runs. This is why the button reacts at once. If the function read a plain JavaScript variable, there would be nothing to watch, and the binding would not update (lesson 04).

---

## 4. A JavaScript File

```js
.pragma library

function emailError(text) { ... }
```

```qml
import "validation.js" as Validation
readonly property string emailMessage: Validation.emailError(emailField.text)
```

- The file contains functions. They are used as `Validation.emailError(...)`, with the name after `as` (it must start with a capital letter).
- **`.pragma library`** makes the file a shared library: one copy for the whole program, and **no access** to the items of the QML file that uses it. The functions get everything they need as arguments and give a result back. This is a good thing: they are easy to understand and to test.
- Without `.pragma library`, a JavaScript file is created again for each QML file that imports it, and it can see that file's context.
- The file must be listed in `QML_FILES` in `CMakeLists.txt`, like a QML file.
- The functions here are **pure**: the same input gives the same output, with no side effect. Write validation, formatting and small calculations like this.

The errors are strings, and `""` means "valid". It is a simple convention for forms.

---

## 5. `Component.onCompleted`

```qml
Component.onCompleted: {
    nameField.forceActiveFocus()
    console.log("The form is ready, valid:", allValid())
}
```

This is a handler for an **attached signal** of every QML object. It runs **once**, when the object and all its children have been created and the bindings have their first values. Use it for start-up code: give the focus to a field, start something, read a setting. The test saw that the name field has the focus.

Rules:

- The order between different objects' `onCompleted` is not specified. Do not rely on it.
- Do not do slow work here: it delays the first picture of the window.
- Do not set a property here that has a binding: you would remove the binding (lesson 04). Use `Component.onCompleted` for actions, bindings for values.
- `Component.onDestruction` is the counterpart for clean-up.

`console.log()`, `console.warn()` and `console.error()` write to the output of the program (and to the Application Output in Qt Creator). They are the first tool for finding errors in QML.

---

## 6. When to Move Logic to C++

JavaScript in QML is **fine** for the small things around the interface and **wrong** for the logic of the application. The rule of the chapter: **QML shows and reacts, C++ decides and computes.**

| Keep in QML / JavaScript | Move to C++ |
|---------------------------|-------------|
| One-line expressions and bindings | A function with more than about 20 lines |
| Short handlers: set a property, call a method | Business rules: prices, permissions, state machines |
| Formatting for display (`toFixed`, `qsTr`, `Qt.formatDateTime`) | Parsing files, protocols, JSON of any size |
| Simple input checks, like in this lesson | Validation that must also run without a UI (a server, a test, a command line) |
| Animations and layout logic | Anything that needs to be fast or to run in a thread |
| Quick prototypes | Anything that needs a unit test (QTest) |
| | Access to files, network, database, hardware, system libraries |

Reasons:

- **Testing.** C++ logic can be tested with QTest (chapter `12_BuildTestDeploy`) without a window. A JavaScript function can be tested, but only inside the QML test framework (lesson 18).
- **Speed.** The JavaScript engine is good for the interface, but it is much slower than compiled C++ for loops and data.
- **Types.** C++ checks types when you compile. In JavaScript, a typing mistake is found when the line runs.
- **Reuse.** The same C++ class can serve a QML interface, a Qt Widgets interface and a test.

In this lesson the validation is in JavaScript because it is small, and it is about the **display** of messages in a form. If the same rules must be applied by a server or stored in a database, they belong to C++. How a C++ class is given to QML is the topic of lessons 12 and 13.

---

## 7. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app11_javascript_in_qml
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Type a wrong e-mail address, then correct it. Watch the red line and the button.
2. Type `18.5` and `abc` into the age field.
3. Look at the output of the program: you should see the line from `Component.onCompleted`.
4. Add a new rule to `passwordError`: at least one capital letter. (`/[A-Z]/.test(text)`)
5. Write a function `function initials()` in `Main.qml` that returns the capital letters of the name, and show it under the form.
6. Remove `.pragma library` from `validation.js`. Does the program still work? What changes?

---

## 8. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `ReferenceError: Validation is not defined` | The import is missing, or the name after `as` is different | `import "validation.js" as Validation` |
| `Cannot find module` / the `.js` file is not found | It is not listed in `QML_FILES`, or the path is wrong | Add it to `CMakeLists.txt` |
| A binding with a function does not update | The function reads only plain JavaScript variables | Read properties of QML objects |
| `TypeError: Cannot read property 'x' of null` | An object does not exist (yet) | Check for `null`, or wait until the object is created |
| `Component.onCompleted` shows old values | Another object is created later | Do not depend on the order; use bindings |
| `==` is true for `"0" == 0` | JavaScript converts types with `==` | Use `===` |
| The regular expression does not match | A mistake in the pattern, or `^` and `$` are missing | Test it first in the console of a web browser |
| A `.pragma library` function cannot see an item | That is intended | Pass the value as an argument |

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| JavaScript expressions in QML | <https://doc.qt.io/qt-6/qtqml-javascript-expressions.html> |
| Defining JavaScript resources | <https://doc.qt.io/qt-6/qtqml-javascript-resources.html> |
| Importing JavaScript resources | <https://doc.qt.io/qt-6/qtqml-javascript-imports.html> |
| The `Qt` global object | <https://doc.qt.io/qt-6/qml-qtqml-qt.html> |
| The `Component` attached signals | <https://doc.qt.io/qt-6/qml-qtqml-component.html#completed-signal> |
| Integrating QML and C++ | <https://doc.qt.io/qt-6/qtqml-cppintegration-topic.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts`, `import "validation.js" as Validation` |
| Minimum Qt version | Qt 6.8 |
