# 09_animations

An animation changes a property **over time**: a number goes from 6 to 400 in one second, a color fades from green to red. Qt Quick has a small set of animation types, and you combine them. This lesson builds four examples: a sweeping ball, a pulsing indicator with controls, a color cycle and a box with different easing curves.

**Requires Qt 6.8 or newer.** Tested with Qt 6.10.2 and 6.12.0 (see the chapter README).

**Test status:** also built with GCC 13.3 against Qt 6.4.2 (with two small changes for that old version, which are not part of this folder). An automated check measured the positions, the scale and the color while the animations ran with the real clock, and tested start, pause, resume and stop (16 checks, all passed). The tests compare coarse values (moved / not moved), not exact frames. Please build it once with your own 6.8+ kit.

---

## 1. What You Build

1. A **ball** that moves from the left to the right and back, for ever.
2. A **pulse**: a green circle that grows and fades and returns. The buttons **Start**, **Pause**, **Resume** and **Stop** control it, and a text shows the state.
3. A **color bar** that changes from green to orange to red and back to green, for ever.
4. A **box** that runs along a lane when you click a button. The four buttons use different easing curves.

---

## 2. The Animation Types

| Type | What it does |
|------|--------------|
| `NumberAnimation` | Changes a number property (`x`, `width`, `opacity`, `scale`, `rotation`...) |
| `ColorAnimation` | Changes a color property |
| `RotationAnimation` | Rotates, and can choose the shorter way (clockwise or not) |
| `PropertyAnimation` | The base of the others. It works with any type, and can animate several properties at once |
| `SequentialAnimation` | Runs its children **one after the other** |
| `ParallelAnimation` | Runs its children **at the same time** |
| `PauseAnimation` | Waits for a time inside a sequence |
| `PropertyAction` | Sets a property at once, inside a sequence |
| `ScriptAction` | Runs a piece of JavaScript inside a sequence |
| `SpringAnimation`, `SmoothedAnimation` | Follow a changing value with a spring or smoothly |

The most important properties of all animations:

| Property | Meaning |
|----------|---------|
| `from`, `to` | Start and end value. `from` can be left out: the animation starts with the current value |
| `duration` | In milliseconds (the default is 250) |
| `easing.type` | The speed profile (section 4) |
| `loops` | How many times. `Animation.Infinite` for ever |
| `running`, `paused` | The state. You can bind them |
| `target`, `property` | What to animate (`properties: "x,y"` for several) |

---

## 3. Two Ways to Write an Animation

### 3.1 `Animation on property`

```qml
Rectangle {
    id: ball
    SequentialAnimation on x {
        loops: Animation.Infinite
        NumberAnimation { from: 6; to: 400; duration: 1000 }
        NumberAnimation { from: 400; to: 6; duration: 1000 }
    }
}
```

The animation is a **child of the item**, and `on x` names the property. It needs no `target` and starts together with the item (`running` is `true` from the start). This is the shortest form for an animation that always runs, like the ball and the color bar.

### 3.2 An animation with an `id`

```qml
NumberAnimation { id: move; target: box; property: "x"; from: 6; to: 400; duration: 1200 }
...
onClicked: move.restart()
```

A free animation does nothing until it is started: `start()`, `stop()`, `pause()`, `resume()`, `restart()`, or `running = true`. Use this form for animations that belong to an event. The pulse and the box use it.

A **sequence** (`SequentialAnimation`) is an animation too, and it has the same controls: the **Pause** button of the pulse calls `pause()` on the whole sequence, and the test saw that the scale stays exactly the same until **Resume**.

Do not **assign** to a property that an animation is changing at that moment, and do not run two animations on the same property of the same item at the same time. The last one that starts wins, and the result looks wrong.

---

## 4. Easing

An animation does not have to go at a constant speed. The `easing.type` is the speed profile:

| Easing | Look |
|--------|------|
| `Easing.Linear` | Constant speed (a machine) |
| `Easing.InQuad`, `OutQuad`, `InOutQuad` | Slow start, slow end, or both: the most natural for movements |
| `Easing.InOutCubic` | Like Quad, but more |
| `Easing.OutBounce` | Hits the end and bounces |
| `Easing.OutElastic` | Overshoots and swings back, like a spring |

"In" means the effect is at the start, "Out" at the end, "InOut" at both. As a rule, things that **appear** use `Out` (they arrive and slow down), things that **leave** use `In`, and things that **move** between two places use `InOut`. Try the four buttons in the lesson. Do not use `Bounce` and `Elastic` for ordinary controls: they are fun and tiring.

---

## 5. `Sequential` and `Parallel`

The pulse is a `SequentialAnimation` of two `ParallelAnimation`s:

```
Sequential (loops for ever)
  Parallel:  scale 1.0 -> 1.8   and   opacity 1.0 -> 0.3     (0.6 s)
  Parallel:  scale 1.8 -> 1.0   and   opacity 0.3 -> 1.0     (0.6 s)
```

You can nest them as deep as you need. The second `Parallel` has no `from`: it starts with the value that the first one ended with.

---

## 6. Performance and Taste

- **Animate properties that are cheap**: `x`, `y`, `scale`, `rotation`, `opacity` and `color`. The scene graph moves, scales and fades items on the GPU. Animating `width` or `height` of an item that has many children, or a size inside a layout, makes Qt do the layout again every frame.
- **Stop animations that nobody sees.** Set `running: visible`, so that a hidden page does not use the CPU.
- **Short is good**: 150 to 300 ms for the feedback of a control, up to 500 ms for a panel. An animation that makes the user wait is a fault.
- **Use animations to explain a change** (this item came from there), not as decoration.
- Qt Quick animations use the frame clock of the window. If the frame rate is low, the **time** is still correct, and the movement only gets less smooth.

---

## 7. Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/app09_animations
```

You can also open `CMakeLists.txt` in Qt Creator, choose a kit and press **Run**.

Try this:

1. Press **Start**, **Pause**, **Resume** and **Stop**, and read the text.
2. Change `duration` of the ball to `300`, and then to `3000`.
3. Add a `PauseAnimation { duration: 500 }` between the two `NumberAnimation`s of the ball.
4. Change the easing of the ball to `Easing.OutBounce`.
5. Add a `RotationAnimation on rotation` with `from: 0; to: 360; loops: Animation.Infinite` to the box in the lane.
6. Set `running: false` for the color animation and bind it to a `CheckBox`.

---

## 8. Common Problems

| Symptom | Cause | Fix |
|---------|-------|-----|
| The animation does not start | A free animation was never started | `start()`, or `running: true` |
| `Cannot animate non-existent property "foo"` | `property` has a typo, or the target has no such property | Check the name; for a new property, declare it |
| The item jumps at the start | `from` is different from the real value | Leave out `from`, or set the same value |
| Two animations fight | Both change the same property | Use one animation, or stop the first |
| An assignment is ignored | The animation sets the property again in the next frame | Stop the animation before you assign |
| The animation runs only once | `loops` is `1` (the default) | `loops: Animation.Infinite` |
| The window uses much CPU | An endless animation of a hidden item | `running: visible` |
| `NumberAnimation` on a color does nothing | The property is a color | `ColorAnimation` |

---

## 9. Reference

| Topic | Where to look |
|-------|---------------|
| Animation and transitions in Qt Quick | <https://doc.qt.io/qt-6/qtquick-statesanimations-animations.html> |
| `Animation` (base type) | <https://doc.qt.io/qt-6/qml-qtquick-animation.html> |
| `NumberAnimation`, `ColorAnimation` | <https://doc.qt.io/qt-6/qml-qtquick-numberanimation.html>, <https://doc.qt.io/qt-6/qml-qtquick-coloranimation.html> |
| `SequentialAnimation`, `ParallelAnimation` | <https://doc.qt.io/qt-6/qml-qtquick-sequentialanimation.html>, <https://doc.qt.io/qt-6/qml-qtquick-parallelanimation.html> |
| Easing curves | <https://doc.qt.io/qt-6/qml-qtquick-propertyanimation.html#easing.type-prop> |
| Module | Qt Quick (`find_package(Qt6 REQUIRED COMPONENTS Quick)`) |
| Imports | `import QtQuick`, `import QtQuick.Controls` (for the buttons) |
| Minimum Qt version | Qt 6.8 |
