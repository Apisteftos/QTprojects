# 08_states_and_transitions

An interface is a set of **situations**: a panel is open or closed, a button is pressed or not, a level is low or high. In QML you describe each situation as a **state**, and you describe how to move from one to the other as a **transition**. This lesson builds an expanding panel and a lamp that changes with a slider.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with two small changes for that old version, which are not part of this folder). An automated check clicked the panel and read its height while it was moving and after it stopped, and moved the slider through the three lamp states (22 checks, all passed). Please build it once with your own 6.8+ kit.

---

## 1. What You Build

1. A **Details** panel. A click on the header opens it: the panel grows from 48 to 200 pixels, the arrow turns by 90 degrees, and the text fades in. A second click closes it again, with the same movement backwards.
2. A slider and a **lamp**. The lamp is green below 40, orange from 40 and red from 70. The color changes smoothly, and the text says which state is active.

---

## 2. States

```qml
states: State {
    name: "expanded"
    PropertyChanges { target: panel; height: 200 }
    PropertyChanges { target: arrow; rotation: 90 }
    PropertyChanges { target: body; opacity: 1 }
}
```

- Every item has a `state` property. The **default state** has the name `""` (empty) and no changes: it is the item as you wrote it.
- A `State` is a **list of changes** to properties. While the state is active, `PropertyChanges` overrides the properties of its `target`. When the state is left, the properties go back to their normal values: the panel has `height: 48` in its code, and that value comes back by itself. You do not write a "close" version.
- You can change the state by assigning it: `panel.state = "expanded"`, or `panel.state = ""` to go back.
- `PropertyChanges` can change any property, also `text` and `color`, and also `visible`. Other changes are `AnchorChanges` (anchors), `ParentChange` (the parent of an item) and `StateChangeScript` (run code).
- A state belongs to one item, and it may change **properties of other items** (here the arrow and the body). The panel is the item that owns the states.

### 2.1 Choosing a state with `when`

```qml
State {
    name: "high"
    when: level.value >= 70
    PropertyChanges { target: lamp; color: "#c0392b" }
}
```

A state with a `when` condition is active **whenever the condition is true**, with no code that sets `state`. This is a binding for the state. If more than one condition is true, **the first state in the list** wins: at the level 80 both `high` and `medium` are true, and the lamp is in `high` because it is written first. So write the strictest condition first. When no condition is true, the item is in its default state.

---

## 3. Transitions

Without a transition, a state change is **instant**. A `Transition` says how to move between two states:

```qml
transitions: Transition {
    from: ""
    to: "expanded"
    reversible: true
    NumberAnimation { target: panel; property: "height"; duration: 300; easing.type: Easing.OutCubic }
    NumberAnimation { target: arrow; property: "rotation"; duration: 300 }
    NumberAnimation { target: body; property: "opacity"; duration: 300 }
}
```

| Property | Meaning |
|----------|---------|
| `from`, `to` | Which change of state. `"*"` means any state. Without them, the transition is for all changes |
| `reversible: true` | The same transition is used backwards for `to` -> `from`. Without it, the way back is instant, unless you write a second `Transition` |
| the children | The animations. They run **at the same time** (in parallel) by default. Put them into a `SequentialAnimation` to run them one after the other |

The animations in a transition do not need a `from` and `to` value. They use the value that the property has now and the value that the new state gives. Lesson 09 explains the animation types, and shows `SequentialAnimation` and `ParallelAnimation`.

The test measured the height of the panel while it moved: after 120 ms it was between 48 and 200 (the easing `OutCubic` is fast at the start), and after 500 ms it was exactly 200 (and 48 after closing).

Hint: if a transition does nothing, check the names in `from` and `to`, and whether the property is really changed by the state.

---

## 4. `Behavior`

```qml
Behavior on color {
    ColorAnimation { duration: 250 }
}
```

A `Behavior` says: **whenever this property changes, for any reason, animate the change.** It does not need a state. In the lamp, the color changes because of a state, and the `Behavior` makes it fade. It would also fade if the color was set from a handler or a binding. The test saw an intermediate color 100 ms after the slider went back to the default.

| | `Transition` | `Behavior` |
|---|--------------|------------|
| Starts when | A **state** changes | A **property** changes |
| Can animate | Many properties of many items together | One property |
| Good for | A set of changes that belong together | "This value should never jump" |

Use a `Behavior` for properties that are changed from many places (a position, a color, an opacity). Use states and transitions for a screen that has a few clear situations.

---

## 5. Rules and Habits

- **Name states by meaning**: `"expanded"`, `"error"`, `"loading"`, not `"state1"`.
- **Put all differences into the state** and do not set the properties from handlers at the same time. A property that has a binding and a state change at the same time is hard to follow.
- **`clip: true`** on the panel keeps the content inside while the height is small. Without it, the text would be seen under the panel.
- **Do not animate the layout of many items every frame** without need. Opacity, position and rotation are cheap. A change of the size of a text with many lines is not.
- A **state machine** with many states and signals is also available (`QtQml.StateMachine`). For ordinary screens, `State` is enough.

---

## 6. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app08_states_and_transitions
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Click the header several times quickly, also while the panel moves.
2. Change `reversible: true` to `false`. What happens on the way back?
3. Change `Easing.OutCubic` to `Easing.OutBounce` and to `Easing.InOutQuad`.
4. Move the `high` state after the `medium` state in the list. Move the slider to 80.
5. Add a state `"critical"` for `level.value >= 95` with a black lamp.
6. Remove the `Behavior` of the lamp and move the slider across 40.

---

## 7. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The state changes, but nothing is animated | There is no `Transition` that matches, or the property has no change | Check `from`, `to` and the target property |
| The change back is instant | `reversible` is missing, or the second transition is not written | `reversible: true` |
| The state does not become active | The `when` expression is false, or another state before it is true | Print it with `console.log`; check the order |
| The property does not return after the state | The property was assigned in JavaScript while the state was active | Do not assign; use states for all changes |
| The animation jumps at the start | The value is a binding that changes during the animation | Use a plain value for the property that is animated |
| A `Behavior` never runs at the start | The first value is set before the `Behavior` exists | That is normal: it is for changes later |
| `Cannot assign to non-existent property` in `PropertyChanges` | The target has no such property | Check the spelling |
| Content is visible outside the panel | `clip` is not set | `clip: true` |

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Qt Quick states | <https://doc.qt.io/qt-6/qtquick-statesanimations-states.html> |
| `State`, `PropertyChanges` | <https://doc.qt.io/qt-6/qml-qtquick-state.html>, <https://doc.qt.io/qt-6/qml-qtquick-propertychanges.html> |
| `Transition` | <https://doc.qt.io/qt-6/qml-qtquick-transition.html> |
| `Behavior` | <https://doc.qt.io/qt-6/qml-qtquick-behavior.html> |
| Animation and transitions | <https://doc.qt.io/qt-6/qtquick-statesanimations-animations.html> |
| Easing curves | <https://doc.qt.io/qt-6/qml-qtquick-propertyanimation.html#easing.type-prop> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Imports | `import QtQuick`, `import QtQuick.Controls` (for the `Slider`) |
| Minimum Qt version | Qt 6.8 |
