# 03_QObject_and_ParentChild

`QObject` is the base class of almost everything in Qt. Its most useful feature for a beginner is the **parent-child tree**: a parent deletes its children, so you rarely call `delete` yourself. This lesson is a console program that makes the tree visible.

**Requires Qt 6.8 or newer.**

**Tested:** built with CMake and GCC 13.3 without warnings, against Qt 6.4.2 (the newest Qt on the test machine). The output below is the real output of that run. The code uses only functions that exist in every Qt 6 version.

---

## 1. What You Build

A class `Node` that prints a line when it is created and when it is destroyed. Five small examples build trees of `Node` objects and show who deletes whom.

---

## 2. The Project

This is a **Qt Console Application** from the Qt Creator wizard. `CMakeLists.txt` is the wizard's file with the two tutorial changes: `cmake_minimum_required(VERSION 3.21)` and `CMAKE_CXX_STANDARD 23`. There is no window, so there is no `QApplication`. `QObject` itself needs no application object.

`Node` has no `Q_OBJECT` macro, because a parent-child tree works for every class that derives from `QObject`. `Q_OBJECT` is only needed for signals, slots and properties (chapter 04).

---

## 3. The Ideas

### 3.1 The parent owns the children

```cpp
auto *root = new Node("root");
new Node("child A", root);        // the second argument is the parent
auto *childB = new Node("child B", root);
new Node("grandchild", childB);

delete root;                       // deletes child A, child B and the grandchild too
```

The constructor of `QObject` takes the parent as its last parameter. When a `QObject` is deleted, its destructor first runs the destructor body of your class, and then deletes all its children, in the order they were added, each with its own children. That is the order in the output of example 1.

You create the children with `new` and **never delete them yourself**. Their parent does it. This is why Qt code is full of `new` without a matching `delete`, and why it still has no leak.

### 3.2 Looking at the tree

| Function | Result |
|----------|--------|
| `parent()` | The parent, or `nullptr` for the top of a tree |
| `children()` | The direct children, as a list |
| `findChild<T *>("name")` | A child (also deeper in the tree) with that `objectName`, or `nullptr`. `T` must have `Q_OBJECT` (newer Qt versions check this when you compile): `QObject *` always works, as in the example |
| `findChildren<T *>()` | All children of that type |
| `setObjectName("name")` | The name used by `findChild` and by debugging output |

### 3.3 Deleting a child is safe

A child that is deleted removes itself from its parent's list (example 3). So you may delete a child early. The parent does not try to delete it a second time.

### 3.4 Changing the parent

`setParent(newParent)` moves a child to another tree (example 4). The new parent becomes the owner.

### 3.5 Objects on the stack

A `QObject` on the stack is destroyed at the end of its scope, like any local variable. It may be the **parent** of heap objects (example 2). Two rules keep this safe:

- A stack object may be a parent. Its children must be created with `new`.
- Never make a heap-allocated parent own a stack object, and never declare a stack child **before** its stack parent. The parent would call `delete` on a stack address, and the program crashes.

### 3.6 `QPointer`

A raw pointer to a `QObject` does not know when the object is deleted. A `QPointer` does: it becomes `nullptr` when the object is destroyed (example 5). Use it when you keep a pointer to an object that somebody else may delete.

### 3.7 Widgets use the same tree

A `QWidget` is a `QObject`, so a button with a parent is deleted with that parent. `new QPushButton("OK", window)` is correct without any `delete`. This is why the lessons of chapter `02_Widgets` have no `delete` for their widgets.

### 3.8 Qt objects cannot be copied

`QObject` has no copy constructor, because two objects cannot share one place in a tree and one name. Pass `QObject` by pointer or by reference.

---

## 4. Output

```
=== Example 1: deleting the root deletes the whole tree ===
  created   root
  created   child A
  created   child B
  created   grandchild
  delete root
  destroyed root
  destroyed child A
  destroyed child B
  destroyed grandchild

=== Example 2: parent(), children(), findChild() ===
  created   root
  created   child A
  created   child B
  root has 2 children
  parent of child A: root
  findChild("child B") found: child B
  child B belongs to root
  destroyed root
  destroyed child A
  destroyed child B

=== Example 3: a deleted child leaves the parent's list ===
  created   root
  created   child A
  created   child B
  destroyed child A
  root now has 1 child
  destroyed root
  destroyed child B

=== Example 4: setParent() moves a child ===
  created   first
  created   second
  created   child
  first: 1, second: 0
  first: 0, second: 1
  destroyed second
  destroyed child
  destroyed first

=== Example 5: QPointer becomes null ===
  created   root
  created   child
  guard is valid
  destroyed root
  destroyed child
  guard is null
```

---

## 5. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/QObject_and_ParentChild
```

Try this:

1. In example 1, add `new Node("child C", root);` after the grandchild. Where does it appear in the destruction order?
2. In example 2, print `root.children().size()` after `delete b;` and compare with example 3.
3. Delete `child` in example 5 first (`delete child;`) and run `delete root;` afterwards. Does the program crash?
4. Change the order of the two stack objects `first` and `second` in example 4. What changes in the output?

---

## Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| A crash at the end of a scope (`free(): invalid pointer` or similar) | A stack object was made the child of a parent that is destroyed first | Create children with `new`, or declare the parent before its children |
| A crash after `delete` | The same object is deleted twice: once by you, once by its parent, because it was never removed from the parent | Do not `delete` an object that has a parent, or use `deleteLater()` (lesson 04) |
| `findChild` returns `nullptr` | The name is spelled differently, the object has no `objectName`, or the type does not match | Check `setObjectName` and the type in `<T *>` |
| `static assertion failed: No Q_OBJECT in the class passed to QObject::findChild` | `T` is your own class without `Q_OBJECT` | Use `findChild<QObject *>`, or add `Q_OBJECT` to the class (and `CMAKE_AUTOMOC ON`) |
| `Node` objects are not deleted at all | They have no parent and were never deleted | Give them a parent or delete them yourself |
| `use of deleted function QObject::QObject(const QObject&)` | A `QObject` was copied | Use a pointer or a reference |

---

## Reference

| Topic | Where to look |
|-------|---------------|
| Object trees and ownership | <https://doc.qt.io/qt-6/objecttrees.html> |
| `QObject` | <https://doc.qt.io/qt-6/qobject.html> |
| `QPointer` | <https://doc.qt.io/qt-6/qpointer.html> |
| Module | Qt Core (`find_package(Qt6 ... COMPONENTS Core)`) |
| Headers | `<QObject>`, `<QPointer>`, `<QDebug>` |
| Minimum Qt version | Qt 6.8 |
