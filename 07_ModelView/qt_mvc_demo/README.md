# Qt MVP Demo — Task Manager

A Qt6 C++ project demonstrating the **MVP (Model-View-Presenter)** pattern,
and how it relates to **MVC (Model-View-Controller)**. Built as a Task Manager
application where all three layers are cleanly separated.

---

## 1. MVC vs MVP — What Is the Difference?

Both patterns solve the same problem: **separating UI from business logic**.
They differ in who controls the flow.

### MVC — Model View Controller

The original pattern, invented for Smalltalk in the 1970s.

```mermaid
flowchart LR
    User -->|interacts with| V[View]
    V -->|user events| C[Controller]
    C -->|updates| M[Model]
    M -->|notifies directly| V

    style V fill:#2E75B6,color:#fff
    style C fill:#1F4E79,color:#fff
    style M fill:#404040,color:#fff
```

- The **View** can observe the **Model** directly
- The **Controller** handles input and updates the Model
- The **Model** notifies the View directly when it changes
- View and Model are **aware of each other**

**Problem:** In practice the View ends up tightly coupled to the Model.
Hard to test the View without the Model being present.

---

### MVP — Model View Presenter

A refinement of MVC popular in Qt, Android (classic), and WinForms.

```mermaid
flowchart LR
    User -->|interacts with| V[View]
    V -->|signals| P[Presenter]
    P -->|calls methods| M[Model]
    M -->|signals| P
    P -->|calls slots| V

    style V fill:#2E75B6,color:#fff
    style P fill:#1F4E79,color:#fff
    style M fill:#404040,color:#fff
```

- The **Presenter** sits in the middle — View and Model never talk directly
- The **View** only emits signals and exposes update slots
- The **Model** only owns data and emits signals when it changes
- View and Model are **completely unaware of each other**

**Benefit:** You can test the Presenter + Model without any widgets.
You can swap the View entirely without touching the Model.

---

### Side-by-side comparison

| Aspect | MVC | MVP |
|---|---|---|
| Who handles input? | Controller | Presenter |
| View ↔ Model relationship | Direct (observer) | None — Presenter only |
| Testability | Medium | High — Presenter testable without UI |
| Coupling | View knows Model | View knows nothing |
| Qt fit | Loose fit | Natural fit with signals/slots |
| Used in | Web frameworks, games | Qt Widgets, Android classic |

---

### MVVM — for completeness

```mermaid
flowchart LR
    V[View] <-->|data binding| VM[ViewModel]
    VM -->|calls| M[Model]
    M -->|notifies| VM

    style V fill:#2E75B6,color:#fff
    style VM fill:#1F4E79,color:#fff
    style M fill:#404040,color:#fff
```

MVVM adds **automatic two-way data binding** between View and ViewModel.
In Qt this is the standard pattern when using **QML** — the QML engine
handles the binding automatically via `Q_PROPERTY`. With pure Qt Widgets
(no QML), MVVM is harder to implement cleanly, so MVP is preferred.

---

## 2. This Project — MVP Architecture

```mermaid
flowchart TD
    subgraph View Layer
        TV[TaskView\nQWidget]
    end

    subgraph Presenter Layer
        TP[TaskPresenter\nQObject]
    end

    subgraph Model Layer
        TM[TaskModel\nQObject]
    end

    subgraph Shell
        MW[MainWindow\nQMainWindow]
    end

    MW -->|creates & owns| TM
    MW -->|creates & owns| TV
    MW -->|creates & owns| TP

    TV -->|addRequested| TP
    TV -->|removeRequested| TP
    TV -->|toggleRequested| TP
    TV -->|clearAllRequested| TP

    TP -->|addTask| TM
    TP -->|removeTask| TM
    TP -->|toggleDone| TM
    TP -->|clearAll| TM

    TM -->|taskAdded| TP
    TM -->|taskRemoved| TP
    TM -->|taskUpdated| TP
    TM -->|modelReset| TP

    TP -->|addTaskItem| TV
    TP -->|removeTaskItem| TV
    TP -->|updateTaskItem| TV
    TP -->|updateStats| TV

    style TV fill:#2E75B6,color:#fff
    style TP fill:#1F4E79,color:#fff
    style TM fill:#404040,color:#fff
    style MW fill:#888,color:#fff
```

**The golden rule:** The arrows between layers only point **through the Presenter**.
`TaskView` and `TaskModel` have zero knowledge of each other.

---

## 3. Layer Responsibilities

### Model — `taskmodel.h / taskmodel.cpp`

```
Owns:    QList<Task>  (the data)
Exposes: addTask(), removeTask(), toggleDone(), clearAll()
Emits:   taskAdded, taskRemoved, taskUpdated, modelReset
Knows:   Nothing about any widget or presenter
```

The Model is a pure data manager. It could be used in a command-line
app, a unit test, or a completely different UI without any changes.

---

### View — `taskview.h / taskview.cpp`

```
Owns:    All widgets (QListWidget, QLineEdit, QPushButton, QLabel)
Exposes: setTasks(), addTaskItem(), removeTaskItem(), updateTaskItem(),
         clearInput(), updateStats()
Emits:   addRequested, removeRequested, toggleRequested, clearAllRequested
Knows:   Nothing about TaskModel or TaskPresenter
```

The View translates raw widget events (button clicks, double-clicks)
into meaningful **intention signals**. It does not decide what happens next —
it just announces what the user wanted.

---

### Presenter — `taskpresenter.h / taskpresenter.cpp`

```
Owns:    Pointers to Model and View (does not own lifetime)
Wires:   View signals → Model methods
         Model signals → View update methods
Knows:   Both Model and View interfaces
Contains: Application logic — "when X happens, do Y then update Z"
```

The Presenter is the brain. All decisions live here:
- When `addRequested` fires → call `m_model->addTask()` then `m_view->clearInput()`
- When `taskAdded` fires → call `m_view->addTaskItem()` then `refreshStats()`

---

### MainWindow — `mainwindow.h / mainwindow.cpp`

```
Role:    Application shell only
Does:    Creates Model, View, Presenter — nothing else
Logic:   Zero
```

```cpp
m_model     = new TaskModel(this);
m_view      = new TaskView(this);
m_presenter = new TaskPresenter(m_model, m_view, this);
setCentralWidget(m_view);
```

This is the entire body of `MainWindow`. Clean.

---

## 4. Class Diagram

```mermaid
classDiagram
    class QObject
    class QMainWindow
    class QWidget

    class Task {
        +int id
        +QString title
        +bool done
    }

    class TaskModel {
        -QList~Task~ m_tasks
        -int m_nextId
        +tasks() QList~Task~
        +count() int
        +doneCount() int
        +addTask(QString)
        +removeTask(int)
        +toggleDone(int)
        +clearAll()
        ~taskAdded(Task)
        ~taskRemoved(int)
        ~taskUpdated(Task)
        ~modelReset()
    }

    class TaskView {
        -QListWidget* m_list
        -QLineEdit* m_input
        +setTasks(QList~Task~)
        +addTaskItem(Task)
        +removeTaskItem(int)
        +updateTaskItem(Task)
        +clearInput()
        +updateStats(int, int)
        ~addRequested(QString)
        ~removeRequested(int)
        ~toggleRequested(int)
        ~clearAllRequested()
    }

    class TaskPresenter {
        -TaskModel* m_model
        -TaskView* m_view
        +TaskPresenter(model, view)
        -refreshStats()
    }

    class MainWindow {
        -TaskModel* m_model
        -TaskView* m_view
        -TaskPresenter* m_presenter
    }

    QObject <|-- TaskModel
    QObject <|-- TaskPresenter
    QWidget <|-- TaskView
    QMainWindow <|-- MainWindow

    TaskModel "1" *-- "many" Task
    TaskPresenter --> TaskModel
    TaskPresenter --> TaskView
    MainWindow *-- TaskModel
    MainWindow *-- TaskView
    MainWindow *-- TaskPresenter
```

---

## 5. Signal Flow — What Happens When User Adds a Task

```mermaid
sequenceDiagram
    actor User
    participant V as TaskView
    participant P as TaskPresenter
    participant M as TaskModel

    User->>V: types title, clicks Add
    V->>V: onAddClicked()
    V-->>P: emit addRequested(title)
    P->>M: m_model->addTask(title)
    M->>M: appends Task to list
    M-->>P: emit taskAdded(task)
    P->>V: m_view->addTaskItem(task)
    P->>V: m_view->clearInput()
    P->>V: m_view->updateStats(...)
    V->>User: list updated, input cleared
```

---

## 6. CMakeLists.txt — Key Points

```cmake
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp   mainwindow.h
    taskmodel.cpp    taskmodel.h
    taskview.cpp     taskview.h
    taskpresenter.cpp taskpresenter.h
)
```

All three layers must be listed. `CMAKE_AUTOMOC ON` handles moc
generation for every file containing `Q_OBJECT`.

---

## 7. Build & Run

**In Qt Creator:**
1. Right-click project → **Run CMake**
2. `Ctrl+B` — build
3. `Ctrl+R` — run

**From terminal:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
./qt_mvc_demo
```

---

## 8. Why MVP Over Plain Qt?

| Situation | Without MVP | With MVP |
|---|---|---|
| Add feature | Edit mainwindow.cpp (grows forever) | Add method to correct layer only |
| Unit test logic | Must instantiate widgets | Test Presenter + Model, no widgets |
| Swap UI toolkit | Rewrite everything | Rewrite View only |
| Find a bug | Search all files | Bug is in exactly one layer |
| New developer | Reads 1000-line mainwindow.cpp | Reads 3 focused files |

For a project where the backend logic is complex (for example a networked application),
MVP ensures the communication logic never gets mixed with widget code.
