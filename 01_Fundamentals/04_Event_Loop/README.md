# 04_Event_Loop

Every Qt program is built around an **event loop**. Mouse clicks, key presses, timers and network data all arrive as events in a queue, and the loop takes them out one by one. This lesson is a console program that makes the loop visible, and shows what happens when a handler is slow.


---

## 1. What You Build

Five small examples with a `QCoreApplication` and a few timers. They print in the order that things really happen, so you can read the event loop from the output.

---

## 2. The Project

A **Qt Console Application** with the wizard's `CMakeLists.txt` and the two tutorial changes. The program uses `QCoreApplication`, the application class without a GUI. A program with windows uses `QApplication` instead, and the loop works the same way.

---

## 3. The Ideas

### 3.1 What the loop does

```
          +--------------------+
 events → |    event queue     |
          +----------+---------+
                     | the loop takes one event at a time
                     v
          +--------------------+
          |  QObject::event()  |  → your slot, handler or paintEvent
          +--------------------+
```

`app.exec()` starts the loop. It waits for events, delivers each to its target object and waits again. It returns when `quit()` or `exit(code)` is called. All widgets and all signals of a thread run in that thread's loop. In a GUI program the loop of the main thread is the **GUI thread**.

### 3.2 Nothing runs before the loop

Example 1: `QTimer::singleShot(0, ...)` does not call the function. It posts an event. The function runs when the loop starts, not before. This is why code after `singleShot` continues first.

### 3.3 Timers

A `QTimer` sends the signal `timeout()` every `interval` milliseconds (example 2). `QTimer::singleShot(ms, receiver, functor)` fires once. The timer needs a running event loop. The time is not exact: it is the **earliest** time at which the function may run.

### 3.4 A slow handler blocks everything

The loop runs one handler at a time. In example 3 the handler A sleeps for 300 ms. The timer B was due after 150 ms, but it can only run after A returns, so it runs after about 400 ms. In a window this means: the window does not repaint, buttons do not react, and the system may show "not responding". **Never do slow work in a slot.** Chapter `09_Concurrency` shows how to move it to another thread.

### 3.5 Queued calls

`QMetaObject::invokeMethod(obj, functor, Qt::QueuedConnection)` posts the call as an event instead of making it at once (example 4). The same happens with a signal connection between objects in different threads. `QCoreApplication::processEvents()` runs the waiting events right now. Use it rarely: it can start code in the middle of a handler.

### 3.6 Leaving the loop

| Call | Effect |
|------|--------|
| `QCoreApplication::quit()` | Same as `exit(0)` |
| `QCoreApplication::exit(7)` | The loop ends and `exec()` returns `7` (example 5) |
| closing the last window (`QApplication`) | Calls `quit()` automatically |

### 3.7 More than one loop

`QEventLoop` is a loop you can create yourself. Every example here has its own local loop, so each one finishes before the next one starts. A dialog's `exec()` also starts a nested loop. A nested loop keeps the program responsive while it waits.

### 3.8 `deleteLater()`

`obj->deleteLater()` posts a deletion event. The object is deleted when control returns to the loop. This is the safe way to delete an object from inside its own slot.

---

## 4. Output

```
=== Example 1: events wait for the loop ===
a: before exec()
b: the timer is set, but nothing has run yet
c: runs inside the loop
d: exec() returned because quit() was called

=== Example 2: a repeating timer ===
tick 1
tick 2
tick 3
the timer stopped after 3 ticks

=== Example 3: a slow handler blocks everything ===
A runs at 100 ms and then sleeps for 300 ms
B was due at 150 ms, it ran after 400 ms or more: true

=== Example 4: a queued call is only posted ===
invokeMethod() returned, the call has not run yet
the queued call runs now
processEvents() returned

=== Example 5: exit() and the return value of exec() ===
app.exec() returned 7
```

The numbers in example 3 are checked with `>=`, so the output is the same on every run.

---

## 5. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/Event_Loop
```

Try this:

1. In example 2, change the interval to 200 and look at how long the program runs.
2. In example 3, change the sleep from 300 to 30. Is B still late?
3. In example 1, remove `loop.quit()`. The program never ends. Why?
4. Replace `Qt::QueuedConnection` in example 4 by `Qt::DirectConnection` and compare the order of the two lines.

---

## Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| A timer never fires | The event loop is not running, or the timer has no object to live in | Call `exec()`, and create the timer after `QCoreApplication` |
| `QObject::startTimer: Timers can only be used with threads started with QThread` | The timer is used in a thread without an event loop | Use a `QThread` and call `exec()` in it, or move the timer to the main thread |
| The window freezes during a long calculation | The calculation runs in a slot, in the GUI thread | Move it to a worker thread (chapter 09), or split it into small steps with `QTimer::singleShot(0, ...)` |
| The program never ends | Nobody calls `quit()` | Connect a signal to `quit()`, or close the last window |
| The program ends at once | `exec()` is not called, or the loop has nothing to wait for and `quit()` was called | Check that `return app.exec();` is reached |

---

## Reference

| Topic | Where to look |
|-------|---------------|
| The event system | <https://doc.qt.io/qt-6/eventsandfilters.html> |
| `QCoreApplication` | <https://doc.qt.io/qt-6/qcoreapplication.html> |
| `QEventLoop` | <https://doc.qt.io/qt-6/qeventloop.html> |
| `QTimer` | <https://doc.qt.io/qt-6/qtimer.html> |
| `QElapsedTimer` | <https://doc.qt.io/qt-6/qelapsedtimer.html> |
| Module | Qt Core (`find_package(Qt6 ... COMPONENTS Core)`) |
| Headers | `<QCoreApplication>`, `<QEventLoop>`, `<QTimer>`, `<QElapsedTimer>`, `<QThread>` |
| Minimum Qt version | Qt 6.8 |
