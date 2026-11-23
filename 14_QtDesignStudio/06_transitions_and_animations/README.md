# 06_transitions_and_animations

Animate the change between states. In `05_states` the lamp jumped from one look to the next. Here the color fades, the opacity changes smoothly and a marker slides under the selected button.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings. A switch from `off` to `on` was sampled while it ran: after 120 ms the lamp color and the marker were between the old and the new value, after 700 ms they were at the new value.

The same idea written by hand is in `11_QML/09_animations`.

---

## 1. What You Build

The scene of lesson 05 with one addition: a small bar (the marker) below the buttons that moves to the active button. All changes between the three states are animated.

| What changes | Animation | Duration |
|--------------|-----------|----------|
| Lamp color | `ColorAnimation` | 300 ms |
| Lamp opacity | `NumberAnimation` with `InOutQuad` easing | 300 ms |
| Card border color | `ColorAnimation` | 300 ms |
| Marker position `x` | `NumberAnimation` with `OutCubic` easing | 250 ms |

The status text `Off`, `Dim`, `On` still switches instantly. A text cannot be blended between two values.

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The scene with states, connections and the transition |
| `App.qml` | The window (as in lesson 03) |

---

## 2. Terms

| Term | Meaning |
|------|---------|
| **Transition** | Describes which animations run when the state changes from one state to another |
| **`from` and `to`** | The states the transition is for. `"*"` means any state |
| **Animation** | Interpolates a property from its old value to its new value over a duration. `ColorAnimation` is for colors, `NumberAnimation` for numbers such as `x`, `opacity` and `width` |
| **Easing** | The speed curve of the animation: constant, slow start, slow end, bounce and so on |

Animations at the top level of a transition run in parallel. To run them one after the other, put them in a `SequentialAnimation`.

A transition does not say what the end value is. The end value comes from the state (`PropertyChanges`), and the animation only decides how the property travels there. This is why you design the states first.

---

## 3. Steps

### 3.1 Start from lesson 05

Copy your project from lesson 05 and name it `Transitions_demo`, or create a new one and rebuild the scene. Keep the `import` line that Design Studio generated in `App.qml`.

### 3.2 Add the marker

1. Drag a **Rectangle** into the card (the item with the buttons). Name it `marker`.
2. Set `width` 96, `height` 4, color `#2e7d5a`, `radius` 2.
3. Place it under the first button (`x` 24, `y` 318). Do **not** use anchors for its horizontal position, because the states change its `x`.
4. Select the state `dim` and set the marker `x` to 132. Select `on` and set it to 240.

Position values (`x`, `y`) can be animated directly. Anchors cannot, so for moving items use `x` and `y`.

### 3.3 Add a transition

1. Open **View > Views > Transitions** (the Transition Editor).
2. Click the **Add Transition** button. This works only if at least one state exists and at least one property is changed in it.
3. In the settings of the transition (the **Settings** button), set the **Transition ID** (for example `lampTransition`), **From** to the start state and **To** to the end state. You can use `*` for any state.
4. The view shows the animated properties as rows with a blue bar. Move the bar to set the start and end frame. Pull its edges to change the duration.
5. To set an easing curve, select the bar and click the **Easing Curve Editor** button. Choose a preset (for example `InOutQuad` or `OutCubic`) and click **OK**.

Design Studio can only animate properties that you changed in a state, so set the property changes first. If a row for a property is missing in the Transition Editor, check that the state changes that property.

### 3.4 Preview

Press **Alt+P** and click the buttons. Change the durations and compare.

### 3.5 Read the code

Open the Text Editor and look at the end of the file for the `transitions` list. Design Studio writes more properties (ids, parallel and sequential containers) than the short version below, but the structure is the same.

---

## 4. The Code

The base scene and the states are as in lesson 05, with the marker added. Only the new parts are shown here. The full file is `Screen01.ui.qml`.

### 4.1 The marker

```qml
Rectangle {
    id: marker
    x: 24
    y: 318
    width: 96
    height: 4
    color: "#2e7d5a"
    radius: 2
}
```

It sits inside `panel`, below the button row.

### 4.2 The states change the marker position

```qml
State {
    name: "dim"

    PropertyChanges {
        target: lamp
        color: "#ffd54a"
        opacity: 0.45
    }

    PropertyChanges {
        target: statusText
        text: "Dim"
    }

    PropertyChanges {
        target: marker
        x: 132
    }
}
```

The `on` state sets the marker to `x: 240`. The `off` state is empty, so the marker stays at the base value `24`.

### 4.3 The transition

```qml
transitions: [
    Transition {
        id: lampTransition
        from: "*"
        to: "*"

        ColorAnimation {
            target: lamp
            property: "color"
            duration: 300
        }

        NumberAnimation {
            target: lamp
            property: "opacity"
            duration: 300
            easing.type: Easing.InOutQuad
        }

        ColorAnimation {
            target: panel
            property: "border.color"
            duration: 300
        }

        NumberAnimation {
            target: marker
            property: "x"
            duration: 250
            easing.type: Easing.OutCubic
        }
    }
]
```

| Part | Meaning |
|------|---------|
| `from: "*"`, `to: "*"` | The transition is used for every change between states |
| `ColorAnimation { target: lamp; property: "color" }` | Fades the lamp color from the old value to the one the new state sets |
| `NumberAnimation { target: marker; property: "x" }` | Slides the marker |
| `duration` | How long the movement takes, in milliseconds |
| `easing.type: Easing.OutCubic` | Fast start, slow end. `InOutQuad` is slow at both ends |
| No `from` or `to` value on an animation | The start is the current value and the end comes from the new state |

Numbers measured with Qt 6.12.0 for a switch from `off` to `on`:

| Time after the click | Lamp color | Marker `x` |
|----------------------|------------|-----------|
| 0 ms | `#b8c2bc` (gray) | 24 |
| 120 ms | `#d2c991` (between gray and yellow) | 204 |
| 700 ms | `#ffd54a` (yellow) | 240 |

---

## 5. Try This

1. Change the marker animation to `easing.type: Easing.OutBounce` and click between the buttons.
2. Set all durations to 1500 and watch the transition slowly. Then set them to 100.
3. Remove the `ColorAnimation` for the lamp. The color jumps, but the opacity still fades.
4. Wrap the lamp color and opacity animations in a `SequentialAnimation { }`. First the color changes, then the opacity.
5. Make the transition specific: set `from: "off"` and `to: "on"` and see that the other state changes are instant again.
6. Add a second `Transition` with `to: "off"` and a short duration, so switching off is fast and switching on is slow. When several transitions match, only the best-matching one is applied.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| Nothing is animated | The transition has no animation for the changed property, or `from` and `to` do not match the states |
| The Add Transition button does nothing | The project has no state, or no property is changed in a state yet |
| The marker jumps | It is anchored horizontally, or its `x` is not changed in the states. Use `x` and remove the anchor |
| The text does not fade | Text content cannot be interpolated. Animate the `opacity` of two texts instead |
| The animation stops in the middle | A new state change arrives. The animation continues from the current value to the new target |
| A color changes without animation | The animation uses the wrong `property`. For a border it is `border.color`, for the fill `color` |
| Several transitions exist and the wrong one runs | Only the best-matching transition is used. Check `from` and `to` of each |

---

## 7. Git

Commit this README, `Screen01.ui.qml` and `App.qml`. See the area README for the files that stay out of Git.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| The Transition Editor | <https://doc.qt.io/qtdesignstudio/qtquick-transition-editor.html> |
| Animation techniques in Design Studio | <https://doc.qt.io/qtdesignstudio/quick-animation-overview.html> |
| Editing easing curves | <https://doc.qt.io/qtdesignstudio/qtquick-editing-easing-curves.html> |
| Animated state transitions (tutorial) | <https://doc.qt.io/qtdesignstudio/state-transition-animations.html> |
| `Transition` | <https://doc.qt.io/qt-6/qml-qtquick-transition.html> |
| Animation and Transitions in Qt Quick | <https://doc.qt.io/qt-6/qtquick-statesanimations-animations.html> |
| `Easing` types | <https://doc.qt.io/qt-6/qml-qtquick-propertyanimation.html#easing.type-prop> |
