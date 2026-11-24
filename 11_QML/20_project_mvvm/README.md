# 20_project_mvvm

The last project of the chapter: a small room booking form, built with the pattern **MVVM** (Model, View, ViewModel). The pattern answers one question that every growing program asks: *where does the logic live?* Here the rules are in plain C++, the interface only shows things, and between them stands a **ViewModel**, a C++ class that QML binds to.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** built with GCC 13.3 against Qt 6.4.2 (with small changes for that old version, which are not part of this folder; one is an explicit `import project_mvvm` in `Main.qml`, which Qt 6.8 does not need). Two automated checks: a C++ test program for the Model, the list and the ViewModel, **without a window** (27 results, all passed), and a run of the real window with the ViewModel handed in from `main.cpp`, with typing, clicks and key presses (34 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

A form (guest name, number of nights, breakfast) with a live total, an error message, a **Save** button that is on only when the form is valid, and a list of the saved bookings with a remove button.

| File | Layer | Content |
|------|-------|---------|
| `booking.h`, `booking.cpp` | **Model** | `Booking`, `BookingService`: the rules and the price. Plain C++, no Qt |
| `bookinglist.h`, `bookinglist.cpp` | Adapter | The saved bookings as a `QAbstractListModel` (lesson 14) |
| `bookingviewmodel.h`, `bookingviewmodel.cpp` | **ViewModel** | Everything the interface shows, and every action it can do |
| `Main.qml` | **View** | The look. No rules |
| `main.cpp` | | Creates the ViewModel and gives it to QML |
| `tests/tst_booking.cpp` | | C++ tests: no window, no QML |

---

## 2. The Three Layers

```
   View (QML)  ───── binds to, calls ─────▶  ViewModel (QObject)  ───── uses ─────▶  Model (plain C++)
      ▲                                           │
      └──────────── notify signals ───────────────┘
```

| Layer | Knows | Does not know | Contains |
|-------|-------|---------------|----------|
| **Model** | nothing about Qt | the ViewModel, the View | Data and the rules of the business: what is valid, what a stay costs |
| **ViewModel** | the Model | any QML file or item | The state of the screen (what is typed, what is shown) and its commands (save, remove). It turns the rules into texts |
| **View** | the ViewModel | the Model | Items, layout, colours, animations |

The rule: **the arrows go in one direction.** The ViewModel never calls `findChild` or touches an item, and the View never calculates a price. Check it with this question: *could I replace the QML file by a completely different one (or by a test) without changing the ViewModel?* Here the answer is yes, and the tests of this lesson are that second "view".

### 2.1 Where Is the Difference to the Earlier Lessons?

In lessons 12 to 15, C++ classes were used by QML, but the rules were often inside the class that also served the interface. Here the **business rules** (`BookingService::check`, `price`) are separated from the **screen state** (`BookingViewModel`). With a bigger program, this is the difference between a class you can test in a second and a class you can only test by clicking.

### 2.2 The Names

- **Model/View of Qt** (chapter `07_ModelView`, and the `QAbstractListModel` in `BookingList`) is about showing **a list** in a view. It is a technique for lists.
- **MVVM** is a pattern for the **whole screen**. The word "model" means the data and rules of the business there.

These are two different ideas with a similar name. `BookingList` is a Qt model, and it is a small adapter that makes the data of the (Qt-free) Model available to a `ListView`.

---

## 3. The Model: Plain C++

```cpp
enum class Problem { None, GuestTooShort, GuestTooLong, NightsOutOfRange };

class BookingService {
public:
    static Problem check(const Booking &booking);
    static int price(const Booking &booking);      // in cents
    ...
};
```

- No `QObject`, no signal, no `QString`. It could be used in a command line program, a server, or a test.
- Money is in **cents** as `int`. `0.1 + 0.2` is not `0.3` with `double`, and prices must be exact. Text for the screen is made at the edge (`priceText`).
- `check` returns **what** is wrong (`Problem`), not the sentence. The sentence is the job of the ViewModel, so that the model needs no translation (`tr()`).
- The name length is counted in **characters**, not bytes. In UTF-8 an `ë` takes two bytes. The test has rows with umlauts, because it is the kind of mistake that is found by a user in another country.

---

## 4. The ViewModel

```cpp
class BookingViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("The ViewModel is created in main.cpp and given to QML")

    Q_PROPERTY(QString guest READ guest WRITE setGuest NOTIFY guestChanged)     // the form
    Q_PROPERTY(QString totalText READ totalText NOTIFY derivedChanged)          // calculated
    Q_PROPERTY(bool canSave READ canSave NOTIFY derivedChanged)
    Q_PROPERTY(BookingList *bookings READ bookings CONSTANT)                    // the result
    ...
    Q_INVOKABLE void save();
```

Three groups of properties:

1. **The form** (`guest`, `nights`, `breakfast`): read and write. The View writes them when the user edits.
2. **Derived values** (`totalText`, `error`, `canSave`): read only. `recalculate()` makes them from the form with the rules of the Model, and sends **one** signal, `derivedChanged`. The View can never show a total that does not belong to the form, because nobody can set it by hand.
3. **The result** (`bookings`, `count`, `message`).

More points:

- **The ViewModel does not trust the View.** The button is disabled when `canSave` is false, but `save()` checks again. A test (and a later version of the interface) can call `save()` at any time. `saveIsRefusedWhenInvalid` tests exactly this.
- **No scolding too early**: `error` is empty while the name field is empty. That is a decision about the **screen**, so it is in the ViewModel, and it is tested (`startState`).
- **Same value, no signal**: each setter returns at once if the value did not change (the test `signalsAreSentOnce`). This also stops a loop between two bindings.
- `QML_UNCREATABLE` tells QML that the type exists, but that `BookingViewModel { }` in a QML file is not allowed. The object comes from outside.
- Declaration order: `m_service` is declared **before** `m_list`, because the list holds a reference to the service. Members are created in the order of their declaration, and destroyed in the opposite order.

---

## 5. Giving the ViewModel to QML

```cpp
BookingViewModel viewModel;                    // before the engine
QQmlApplicationEngine engine;
engine.setInitialProperties({{"viewModel", QVariant::fromValue(&viewModel)}});
engine.loadFromModule("project_mvvm", "Main");
```

```qml
ApplicationWindow {
    required property BookingViewModel viewModel     // filled by setInitialProperties
```

- `main()` owns the object. The engine only uses it. A pointer given this way is **not** deleted by QML.
- The ViewModel is declared **before** the engine, so the engine is destroyed **first**. No QML item can then use a dead object when the program ends.
- `required property` with the **type** gives an error when the file is loaded without the value (it is not silently `undefined`), and the editor and `qmllint` can check every `root.viewModel.x` against the class (lesson 18).

Other ways to give C++ objects to QML, and when to use them:

| Way | Use |
|-----|-----|
| `QML_ELEMENT` and `Dashboard { }` in QML (lesson 19) | The object belongs to this piece of the interface and can be made by QML |
| `QML_SINGLETON` (lesson 13) | There is exactly one for the whole program |
| `setInitialProperties` (this lesson) | `main()` creates and owns it: needed for objects with arguments, a database, or a test double |
| `setContextProperty` | The old way. A name that exists "somewhere": tools cannot check it. Avoid it in new programs |

---

## 6. The View

```qml
TextField {
    text: root.viewModel.guest                       // ViewModel -> View
    onTextEdited: root.viewModel.guest = text        // View -> ViewModel, only when the USER types
}
```

This is a **two-way connection written as two one-way connections**. Why `onTextEdited` and not `onTextChanged`: `textChanged` also comes when the **ViewModel** sets the text (after `save()` the form is cleared), and the value would be written back at once. `textEdited` comes only from the user. (The same is true for `onValueModified` of the `SpinBox` and `onToggled` of the `Switch`.) The test changes the name in C++ and sees it in the field, and types into the field and sees it in the ViewModel.

Read `Main.qml` once more and look for a rule: there is none. The `SpinBox` allows 0 to 20 on purpose, and the limit 1 to 14 is **only** in the Model. When the user chooses 0 or 15, the ViewModel shows "Choose 1 to 14 nights" and keeps the button off. If the business changes the limit to 21, only `booking.h` changes.

The delegate gets the roles as `required property` (lesson 14) and the text for the price from `priceText`, a `Q_INVOKABLE` of the ViewModel. It is called in a binding, which is fine here because the price in the list never changes after it is saved. (Lesson 12 explains that a `Q_INVOKABLE` in a binding does not update when the data change.)

---

## 7. Testing Without a Window

```cpp
void TstBooking::saveAddsARowAndClearsTheForm()
{
    BookingViewModel vm;
    vm.setGuest("  Al  ");
    vm.setNights(3);
    vm.setBreakfast(true);
    vm.save();

    QCOMPARE(vm.count(), 1);
    QCOMPARE(vm.message(), QString("Saved: Al, 3 nights, 303.00"));
    QCOMPARE(vm.guest(), QString());             // the form is empty again
}
```

The test does what a user does (type, click save) and looks at what the user would see (`message`, `count`, the form). It needs no window, no QML and no display, so it runs in milliseconds on a build server. The tests in the file:

| Test | What it protects |
|------|------------------|
| `price`, `check` (data-driven) | The rules and the boundaries: 30 and 31 characters, 14 and 15 nights, umlauts |
| `listIsAValidModel` | `QAbstractItemModelTester` (lesson 14) checks the model while the test uses it |
| `startState`, `typingChanges...`, `errorTexts` | What the screen shows |
| `saveAddsARow...`, `saveIsRefused...`, `removeAndClear` | The commands |
| `signalsAreSentOnce` | Signals only for real changes |

Build and run:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

(`CMakeLists.txt` sets `QT_QPA_PLATFORM=offscreen` for this test, so it also runs on a computer without a screen, such as a build server. A first try without this setting aborted in the sandbox with "could not connect to display".)

The program itself:

```bash
./build/app20_project_mvvm
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**. For the QML side of the tests see lesson 18.

---

## 8. Try This

1. Type one letter, then a second. Watch the error message and the button.
2. Choose 0 nights with the spin box. Which layer produces the text?
3. Change the breakfast price in `booking.h` to 1500. Which tests fail? (The tests with prices: this is what they are for.)
4. Add a rule: a booking of more than 7 nights gets a discount of 10 percent. Write the test **first** (`price_data`), then the code in `BookingService::price`. Does `Main.qml` change? It should not.
5. Add a new field "Room" (`Single` or `Double`) with a different price. Which files change, in which order?
6. Write a second View: a `main.cpp` that creates the ViewModel and prints a booking to the terminal with `std::println`, with no Qt Quick. This shows that the ViewModel does not depend on QML.
7. Save the bookings in a JSON file when the program ends (chapter `10_IO_and_Data`). Which layer gets the code?

---

## 9. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| `Required property viewModel was not initialized` | The QML file was loaded without `setInitialProperties` | Set the properties before `load`/`loadFromModule`, with the **same name** as the property |
| `BookingViewModel is not a type` | The class is not in `SOURCES`, or `QML_ELEMENT` is missing | Check `CMakeLists.txt` and the header |
| `Cannot create BookingViewModel` | The type is `QML_UNCREATABLE` | Right: create it in C++ |
| Crash when the window closes | The ViewModel was declared **after** the engine and died first | Declare it before the engine |
| The form field is cleared while the user types | A binding is broken or `onTextChanged` writes back | Use `onTextEdited` |
| The text field does not show a change from C++ | Its `text` binding was replaced by an assignment in QML | Do not assign `text = ...` in JavaScript; write to the ViewModel |
| A rule is in the QML file | It was easier at the time | Move it to the Model and test it |
| The ViewModel includes `<QQuickItem>` | The ViewModel talks to the View | Remove it: use a property and a signal instead |
| Test link error `undefined reference` | A `.cpp` file is missing in the test target | List all the files that the test needs |
| `QAbstractItemModelTester` reports an error | `beginInsertRows`/`endInsertRows` is missing, or the row numbers are wrong | Change the data only between `begin...` and `end...` |

---

## 10. Reference

| Topic | Where to look |
|-------|---------------|
| Exposing C++ types to QML (`QML_ELEMENT`, `QML_UNCREATABLE`) | <https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html> |
| Initial properties of QML objects | <https://doc.qt.io/qt-6/qqmlapplicationengine.html#setInitialProperties> |
| `required` properties | <https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html#required-properties> |
| Model/View programming | <https://doc.qt.io/qt-6/model-view-programming.html> |
| `QAbstractListModel`, `QAbstractItemModelTester` | <https://doc.qt.io/qt-6/qabstractlistmodel.html>, <https://doc.qt.io/qt-6/qabstractitemmodeltester.html> |
| Qt Test | <https://doc.qt.io/qt-6/qtest-overview.html> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`), Qt Test for the tests |
| Headers | `<QObject>`, `<QAbstractListModel>`, `<QtQml/qqmlregistration.h>`, `<QGuiApplication>`, `<QQmlApplicationEngine>`, `<QtTest>` |
| Imports | `import QtQuick`, `import QtQuick.Controls`, `import QtQuick.Layouts` |
| Minimum Qt version | Qt 6.8 |
