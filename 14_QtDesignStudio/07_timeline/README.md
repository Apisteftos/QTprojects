# 07_timeline

Record an animation with keyframes. In lessons 05 and 06 the animation was triggered by a state change and ran from the old value to the new one. A **timeline** is different: you decide the value of a property at chosen moments, and Design Studio fills in the values between them.

**Tested with Qt Design Studio 4.8.2** (Flatpak) on Ubuntu 26.04.1 LTS. The final QML was run with Qt 6.12.0 without warnings, and the values were sampled while it ran (section 4).

---

## 1. What You Build

An animated intro of a card. It plays once when the application starts, takes 2 seconds and has four steps:

1. The card fades in.
2. The leaf grows from nothing and spins into place.
3. The title fades in and slides up.
4. The subtitle fades in.

| File | Purpose |
|------|---------|
| `Screen01.ui.qml` | The scene and its timeline |
| `App.qml` | The window |
| `leaf.png` | The image from lesson 02 |

---

## 2. Terms

| Term | Meaning |
|------|---------|
| **Timeline** | A list of animated properties with keyframes, between a start frame and an end frame |
| **Frame** | A position on the timeline. Frame numbers are not seconds |
| **Keyframe** | The value of a property at one frame. Design Studio adds the values between two keyframes automatically, evenly spaced |
| **Playhead** | The marker you drag along the timeline to see the scene at a frame |
| **Animation** | The part that plays the timeline. It has a start frame, an end frame and a duration in milliseconds. A timeline can have several animations |
| **Easing curve** | Makes the change between two keyframes non-linear (slow start, overshoot and so on) |

In this lesson the timeline has 1000 frames and the animation plays them in 2000 ms, so one frame is 2 ms. The frame numbers only matter relative to each other.

Compared with lesson 06:

| | Transition (06) | Timeline (07) |
|---|-----------------|---------------|
| Starts | On a state change | Whenever you start it: at startup, on a state, from a slider |
| Values | Old value to the value of the new state | Any values at any frames |
| Best for | Smooth switches between screens | Intros, motion with several steps, animations the user controls |

---

## 3. Steps

### 3.1 Create the project

Create `Timeline_intro` with the settings of the [area README](../README.md) and import `leaf.png`. Keep the `import` line that Design Studio generated in `App.qml`.

### 3.2 Build the finished scene first

In `Screen01.ui.qml` build the card, the leaf and the two texts as they should look **at the end** of the animation. Use `x` and `y` for the positions, not anchors, because the timeline will animate `y`.

Give the items clear ids: `card`, `leaf`, `titleText`, `subtitleText`.

### 3.3 Add the timeline

1. Open **View > Views > Timeline**.
2. In the Timeline view, click the **Plus** button. This adds a timeline and opens the Timeline Settings dialog.
3. The default timeline has 1000 frames. Set the **Animation ID** to `introAnimation`, and set the **Duration** to `2000`. Keep **Running in base state** ticked, so that the animation runs when the application starts.
4. Click **Close** to save the settings.

If you clear **Running in base state**, the animation runs only when another state is applied. That is how you start an animation from a button (see lesson 08).

### 3.4 Record the keyframes

Do this for each property in the table in section 4.1. Example for the card opacity:

1. Select `card` in the Navigator.
2. In the Properties panel, open **Visibility > Opacity**, click the **Action** button next to the field and choose **Insert Keyframe**.
3. In the Timeline, check that the playhead is at frame 0, expand the `card` row, and click the **Record** button (per-property recording) for opacity to start recording.
4. Enter `0` as the opacity and press Enter. This is the keyframe at frame 0.
5. Move the playhead to frame 250, enter `1`. This is the second keyframe.
6. Click the record button again to **stop recording**. Otherwise later changes are recorded as well.

You can right-click a keyframe marker and select **Edit Keyframe** to change its value or frame.

Repeat for the other properties.

### 3.5 Add easing

Right-click a keyframe and open the **Easing Curve Editor**. Choose a preset, for example `OutBack` for the leaf scale, which overshoots slightly and settles.

### 3.6 Preview

- Drag the playhead along the timeline to see any moment.
- Press the **Play** button or the **Space** bar in the Timeline view to play the animation.
- Press **Alt+P** for the Live Preview of the whole application.

### 3.7 Read the code

Open the Text Editor and find the `Timeline` item at the end of `Screen01.ui.qml`. Each row you recorded is a `KeyframeGroup`.

---

## 4. The Code

### 4.1 What is animated

| Item | Property | Frame 0 | Later frames |
|------|----------|---------|--------------|
| `card` | `opacity` | 0 | 250: 1 |
| `leaf` | `scale` | 0 | 500: 1 (easing `OutBack`) |
| `leaf` | `rotation` | -180 | 500: 0 (easing `OutCubic`) |
| `titleText` | `opacity` | 0 | 400: 0, 700: 1 |
| `titleText` | `y` | 174 | 400: 174, 700: 144 (easing `OutCubic`) |
| `subtitleText` | `opacity` | 0 | 600: 0, 900: 1 |

The scene above the timeline has the **final** values: `leaf` at `x` 112, `y` 28, `titleText` at `y` 144, and so on.

### 4.2 The scene

```qml
import QtQuick
import QtQuick.Timeline

Rectangle {
    id: root
    width: 800
    height: 480
    color: "#eef2f0"

    Rectangle {
        id: card
        width: 320
        height: 260
        color: "#ffffff"
        radius: 16
        border.color: "#c9d6cf"
        border.width: 2
        anchors.centerIn: parent

        Image {
            id: leaf
            x: 112
            y: 28
            width: 96
            height: 96
            source: "leaf.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: titleText
            x: 0
            y: 144
            width: 320
            text: "Timeline intro"
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter
            color: "#1d4d38"
        }

        Text {
            id: subtitleText
            x: 0
            y: 188
            width: 320
            text: "Keyframes in Qt Design Studio"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            color: "#5b6b63"
        }
    }

    // the Timeline follows in 4.3
}
```

### 4.3 The timeline

```qml
Timeline {
    id: timeline
    startFrame: 0
    endFrame: 1000
    enabled: true

    animations: [
        TimelineAnimation {
            id: introAnimation
            running: true
            loops: 1
            duration: 2000
            from: 0
            to: 1000
        }
    ]

    KeyframeGroup {
        target: leaf
        property: "scale"

        Keyframe {
            frame: 0
            value: 0
        }

        Keyframe {
            frame: 500
            value: 1
            easing.type: Easing.OutBack
        }
    }

    // more KeyframeGroup blocks for card.opacity, leaf.rotation,
    // titleText.opacity, titleText.y and subtitleText.opacity
    // (the complete file is Screen01.ui.qml)
}
```

| Part | Meaning |
|------|---------|
| `import QtQuick.Timeline` | The Qt Quick Timeline module. Design Studio adds it for you |
| `Timeline` | The container: the frame range and the list of animations |
| `TimelineAnimation` | Plays the frames from `from` to `to` in `duration` milliseconds. `running: true` starts it at startup, `loops: 1` plays it once. `loops: Animation.Infinite` repeats it |
| `KeyframeGroup` | All keyframes of **one property** of **one item** (`target` and `property`) |
| `Keyframe { frame; value }` | The value at one frame |
| `easing.type` | The curve of the change that **ends** at this keyframe |

Before the first keyframe of a group, a property keeps its value from the scene. After the last keyframe it keeps the last value. That is why `titleText.opacity` needs a keyframe at frame 400 with the value 0: it holds the title invisible until then.

Numbers measured with Qt 6.12.0 while the animation played:

| Time | Card opacity | Leaf scale | Leaf rotation | Title opacity | Title `y` |
|------|--------------|------------|---------------|---------------|-----------|
| 100 ms | 0.16 | 0.34 | -140 | 0 | 174 |
| 500 ms | 1.0 | 1.09 | -21 | 0 | 174 |
| 1000 ms | 1.0 | 1.0 | 0 | 0.4 | 150 |
| 3000 ms | 1.0 | 1.0 | 0 | 1.0 | 144 |

The leaf scale at 500 ms is above 1, because the `OutBack` easing overshoots before it settles.

---

## 5. Try This

1. Change `duration` to 6000 and watch the intro in slow motion. Then set 800.
2. Set `loops: Animation.Infinite` to play the intro again and again. Change it back.
3. Add a keyframe group for `leaf.opacity` that starts at 0 and reaches 1 at frame 200.
4. Move the title keyframes: let the title start before the leaf is done.
5. Clear **Running in base state** in the Timeline Settings. The animation no longer plays at startup. Nothing starts it yet. Lesson 08 shows how to start it from a button.
6. Add a `OutBounce` easing to the last keyframe of the title `y`.

---

## 6. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| Nothing animates | The animation is not running: tick **Running in base state**, or check `running: true` |
| A property jumps instead of moving | It has only one keyframe, or it is anchored. Anchored positions cannot be keyframed. Use `x` and `y` |
| The scene looks wrong after recording | The record button was still on. Later edits were recorded as keyframes. Stop the recording and undo |
| The scene in the 2D view shows the first frame | Expected. The 2D view shows the playhead frame. Move the playhead to the end to see the final design |
| The title is visible too early | It needs a keyframe with the value 0 at a later frame to hold the value |
| Two timelines fight over one property | Only keyframe a property in one timeline at a time |
| The easing seems to have no effect | The easing belongs to the keyframe where the change **ends**. Set it on the later keyframe |

---

## 7. Git

Commit this README, `Screen01.ui.qml`, `App.qml` and `leaf.png`. See the area README for the files that stay out of Git.

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Creating timeline animations | <https://doc.qt.io/qtdesignstudio/studio-timeline.html> |
| The Timeline view | <https://doc.qt.io/qtdesignstudio/qtquick-timeline-view.html> |
| Timeline animation (tutorial) | <https://doc.qt.io/qtdesignstudio/animation-tutorial.html> |
| Log In UI: Timeline (tutorial) | <https://doc.qt.io/qtdesignstudio/qtdesignstudio-loginui4-example.html> |
| Editing easing curves | <https://doc.qt.io/qtdesignstudio/qtquick-editing-easing-curves.html> |
| Qt Quick Timeline module | <https://doc.qt.io/qt-6/qtquicktimeline-index.html> |
