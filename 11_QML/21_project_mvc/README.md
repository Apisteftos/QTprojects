# 21_project_mvc

The pattern **MVC** (Model, View, Controller) for a QML program: a small score board for a game. The **model** holds the data and the rules, the **view** (QML) shows the model and tells every input to the **controller**, and the controller changes the model. The same pattern as `20_project_mvvm` in spirit, but with a different split of the work: read section 6 to see the difference.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; one is an explicit `import project_mvc` in `Main.qml`, which Qt 6.8 does not need). Two automated checks: a C++ test program for the model and the controller **without a window** (14 results, all passed; `ctest` also passed), and a run of the real window with typing, the Enter key and clicks (21 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A list of players with points. You can add a player (a name field and the Enter key), give or take a point, remove a player, and start a new round. The player(s) with the highest points are marked with a star. A red line shows why a request was refused.

| File | Layer | Content |
|------|-------|---------|
| `scoremodel.h`, `scoremodel.cpp` | **Model** | A `QAbstractListModel` with the players and the rules |
| `scorecontroller.h`, `scorecontroller.cpp` | **Controller** | The commands of the user: add, give point, take point, remove, new round |
| `Main.qml` | **View** | The list and the controls. Changes no data |
| `main.cpp` | | Creates model and controller and gives both to QML |
| `tests/tst_score.cpp` | | C++ tests of model and controller |

---

## 2. The Three Roles

```
              input (click, text)
   User ───────────────────────────▶ View (QML) ──── calls ────▶ Controller ──── changes ────▶ Model
     ▲                                  ▲                                                          │
     └──────────── looks at ────────────┴────────────── signals: "the data changed" ───────────────┘
```

| Role | Does | Does not |
|------|------|----------|
| **Model** | Holds the data. Applies the rules (no empty names, no double names, points never below 0). Sends signals when the data change | Know the controller or the view |
| **View** | **Reads** the model (the list, `count`, `total`) and **reports** the input of the user to the controller | Change the data |
| **Controller** | Gets the input, decides what it means, calls the model. Tells the view when a request was refused (a signal) | Hold data, or touch any item of the view |

Two arrows leave the view: one to the controller (**input**), and one from the model (**output**, by bindings and signals). That is the center of MVC: **input and output take different roads.**

---

## 3. The Model

```cpp
class ScoreModel : public QAbstractListModel {
    enum Role { NameRole = Qt::UserRole + 1, PointsRole, LeadingRole };
    enum class Result { Ok, EmptyName, DuplicateName, NoSuchRow, NoPointsLeft };

    Result addPlayer(const QString &name);
    Result addPoints(int row, int delta);
    ...
signals:
    void countChanged();
    void totalChanged();
};
```

It is a Qt model (lesson 14), so a `ListView` shows it directly. Three things to learn from it:

- **Rules are in the model.** A name is trimmed, empty names and double names (also with other letters: "ana" and "Ana") are refused, and points do not go below 0. Whatever controller or test calls the model, the rules hold.
- **The model reports what happened, not what to say.** `addPlayer` returns a `Result`, not a sentence. The words belong to the controller (translation with `tr()`).
- **`LeadingRole` is calculated.** "Leading" is not stored; it depends on all players. Therefore `addPoints` sends `dataChanged` for **all** rows. If it only announced the changed row, a view would keep the old star on a player who is no longer in front. The test `leadingWithTies` checks exactly this case: Ben scores, and the star of **Ana** goes away. Without the signal for all rows, the model would be right and the screen wrong.

The same care is in `removeAt`: after a removal the leader can change, so the roles are announced again.

The signals only go out for a real change (`addPoints(0, 0)` sends nothing, a refused name sends nothing). The test counts them.

---

## 4. The Controller

```cpp
class ScoreController : public QObject {
    Q_INVOKABLE bool addPlayer(const QString &name);
    Q_INVOKABLE void givePoint(int row);
    ...
signals:
    void rejected(const QString &reason);
};
```

It is short on purpose. Every function is a **user action**: "the user wants to add a player". It calls the model and turns a refusal into a message:

```cpp
bool ScoreController::report(ScoreModel::Result result)
{
    switch (result) {
    case ScoreModel::Result::Ok:  return true;
    case ScoreModel::Result::DuplicateName:
        emit rejected(tr("This name is already in the game"));
        break;
    ...
    }
    return false;
}
```

`addPlayer` returns `true` when it worked, so the view can clear the name field only then (section 5). The controller knows nothing about the field.

---

## 5. The View

```qml
ApplicationWindow {
    required property ScoreModel scores              // to READ
    required property ScoreController controller     // to REPORT input

    ListView { model: root.scores; ... }
    Button { text: "+"; onClicked: root.controller.givePoint(row.index) }
}
```

- **Output:** `model: root.scores` and `root.scores.total` are bindings. When the model changes, the screen follows by itself.
- **Input:** every click calls a function of the controller. The view never writes `points = points + 1`, and never calls the model's `addPoints`.
- **The view decides how to show a message.** `Connections { target: root.controller; function onRejected(reason) { ... } }` fills a red label that a `Timer` clears after 3 seconds. The controller only says that a request was refused.
- **Clear the field only when the controller accepted the name:**

```qml
onClicked: {
    if (root.controller.addPlayer(nameField.text))
        nameField.clear()
}
```

  With a double name, the text stays, and the user can correct it (the test checks this).

In `main.cpp` both objects are created **before** the engine and given with `setInitialProperties`, as in lesson 20 (see its section 5 for the reasons: ownership, destruction order, and `required property` with a type).

---

## 6. MVC or MVVM?

Both separate rules, input and output. The difference is **who prepares the data for the screen**.

| | MVC (this lesson) | MVVM (lesson 20) |
|---|-------------------|------------------|
| What the view reads | The **model** itself | The **ViewModel**, which holds the state of the screen |
| Where the input goes | To a **controller** (commands) | To the ViewModel (properties and commands) |
| Who knows what is "valid for the screen" (an error text, "Save" enabled) | The controller reports, the view decides | The ViewModel: `error`, `canSave` |
| Fits when | The data are a list or a table and the view shows them as they are | A form with fields that depend on each other and a state of its own |
| Cost | Fewer classes | One more layer, but a very simple view |

Rules to choose:

- A **list that the user edits** (this lesson): MVC is natural, since the Qt model is already what the view needs.
- A **form** with fields, calculated values and buttons that switch on and off (lesson 20): a ViewModel is simpler.
- In a bigger program both appear: a ViewModel can contain a model like `ScoreModel` for its list.

Qt itself calls its own idea "Model/View": the view and the controller are one object there (the delegate and the view class handle input). This lesson separates the controller as its own class, which is what the classic MVC means and what is useful when the input has real rules. Do not worry about the exact names: ask the question "which class changes the data, and which class only shows them?" and keep the answer clear.

---

## 7. Testing Without a Window

```cpp
void TstScore::controllerReportsProblems()
{
    ScoreModel model;
    ScoreController controller(model);
    QSignalSpy rejected(&controller, &ScoreController::rejected);

    QVERIFY(!controller.addPlayer("  "));
    QCOMPARE(rejected.last().at(0).toString(), QString("Type a name first"));
    ...
    QCOMPARE(model.count(), 1);   // the model is unchanged by all the refused requests
}
```

The model is tested alone (the rules, the signals, with `QAbstractItemModelTester`), and the controller is tested with a model, like a user would use it, with a `QSignalSpy` for the refusals. No window and no QML are needed.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/app21_project_mvc
```

`CMakeLists.txt` sets `QT_QPA_PLATFORM=offscreen` for the tests, so they run on a computer without a screen too. You can also open `CMakeLists.txt` in Qt Creator and press **Run** (choose the target `app21_project_mvc` for the program and `tst_project_mvc` for the tests).

---

## 8. Try This

1. Add two players with the same points, give a point to one of them and look at the stars.
2. Type "ana" when "Ana" is in the list. Read the message. The text stays in the field.
3. Take a point from a player with 0 points.
4. Change the rule in `ScoreModel::addPlayer`: a name needs at least 3 characters (new `Result::NameTooShort`). Write the test first. Which files must change? (Model, controller for the message, tests. The view does not.)
5. Add a "Winner" label to the view: the name of the leader. Where does the calculation go? (Hint: the view only shows; a new `Q_PROPERTY` `leader` of the model is the clean answer.)
6. Add an undo: the controller remembers the last action. Is the memory of the controller still "no data"? Discuss: this is a real design question.
7. Replace the QML view by a second one that only prints the players with `std::println` (no Qt Quick). The model and the controller do not change.

---

## 9. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The star stays on the wrong player | `dataChanged` was sent only for the changed row | Announce the roles of all rows (section 3) |
| `Required property controller was not initialized` | The properties were not set before loading | `setInitialProperties` with the **same names** as the required properties |
| `ScoreModel is not a type` | The class is not in `SOURCES`, or `QML_ELEMENT` is missing | Check `CMakeLists.txt` and the header |
| Crash when the window closes | A model or controller was declared after the engine | Declare both before the engine |
| The view changes the data itself | Writing to a role in a delegate (`points = ...`) | Call the controller; the model changes the data |
| The model sends no signal, the view does not change | `beginInsertRows`/`endInsertRows` or `dataChanged` is missing | Send them for every change |
| The controller has a lot of rules | Rules grew into it | Move them to the model, so that every user of the model gets them |
| The controller knows a QML item (`findChild`) | The layers are mixed | Use a signal, like `rejected` |
| Test link error `undefined reference` | A `.cpp` is not in the test target | List every file that the test uses |

---

## 10. Reference

| Topic | Where to look |
|-------|---------------|
| Model/View programming in Qt | <https://doc.qt.io/qt-6/model-view-programming.html> |
| `QAbstractListModel`, `QAbstractItemModelTester` | <https://doc.qt.io/qt-6/qabstractlistmodel.html>, <https://doc.qt.io/qt-6/qabstractitemmodeltester.html> |
| Exposing C++ types to QML (`QML_ELEMENT`, `QML_UNCREATABLE`) | <https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html> |
| Initial properties of QML objects | <https://doc.qt.io/qt-6/qqmlapplicationengine.html#setInitialProperties> |
| `Connections` | <https://doc.qt.io/qt-6/qml-qtqml-connections.html> |
| Qt Test | <https://doc.qt.io/qt-6/qtest-overview.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`), Qt Test for the tests |
| Headers | `<QAbstractListModel>`, `<QObject>`, `<QtQml/qqmlregistration.h>`, `<QGuiApplication>`, `<QQmlApplicationEngine>`, `<QtTest>` |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
