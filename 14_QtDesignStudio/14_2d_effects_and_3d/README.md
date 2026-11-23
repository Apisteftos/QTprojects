# 14 – 2D Effects and a Simple 3D Scene

This lesson adds two things to a Design Studio project: **effects** on a 2D image, and a small **3D scene** with a camera, a light and three models. A Timeline turns the cube.

> **Tested:** the QML file loads without errors or warnings with Qt 6.12.0 (all types and properties resolve).
> **Not tested:** the rendering. The test machine had no GPU, so the blur, shadow, colorization and the 3D scene were not drawn. Check the result in your own Design Studio. Menu names come from the Qt Design Studio documentation.

## 1. What you build

| Left card | Right card |
|---|---|
| `leaf.png` inside a `MultiEffect` with blur, color overlay and shadow. Two sliders and a switch change the effect. | A `View3D` with a camera, a directional light, a cube, a sphere and a floor. The cube turns by Timeline. |

## 2. Create the project

1. **File > New Project**, choose a **2D application** (Qt Quick Application, Qt 6.8 or newer), name it `Effects_and_3d`.
2. Copy `leaf.png` next to the QML files (drag it into the Assets view).
3. Replace the content of `Screen01.ui.qml` with the file from this folder. Keep the `import Effects_and_3d` line that Design Studio generated in `App.qml`.

A project name must not start with a digit. Design Studio rewrites a name such as `2d_effects` to `Two_d_effects`, and the `import` line then uses that name. The module name is the project name only, without the `Content` folder suffix. If you see `QML module not found`, make the `import` line match the project name.

If Design Studio asks for the **QtQuick3D** module, open the **Components** view, press **+** and add **Qt Quick 3D**. Without it, the `View3D` item is unknown.

## 3. Effects on a 2D item

`MultiEffect` (module `QtQuick.Effects`) applies several effects in one item. It uses another item as its `source`.

```qml
Image {
    id: leaf
    source: "leaf.png"
    visible: false        // only the effect is drawn
}

MultiEffect {
    source: leaf
    anchors.fill: leaf
    blurEnabled: true
    blur: blurSlider.value
    colorization: colorSlider.value
    colorizationColor: "#d9a21b"
    shadowEnabled: shadowSwitch.checked
}
```

- The source item is hidden, otherwise you see it twice.
- Every effect has an `...Enabled` property and a strength (`blur` from 0 to 1, `colorization` from 0 to 1).
- `blurMax` sets the largest blur radius in pixels.
- The sliders are bindings: no code is needed to change the effect.

In the Design Studio you can add `MultiEffect` from the **Components** view (**Qt Quick > Effects**), then set the properties in the **Properties** view.

## 4. Effect Composer

The **Effect Composer** view builds your own effect from ready-made parts such as Color Overlay, Rain or Swirl. The result is a shader effect that you apply to a 2D item or to a 3D view.

1. Open the **Effect Composer** view (**View > Views**).
2. Press **+** to start a new composition.
3. Add one or more effect nodes and change their values.
4. Save it. The composition appears as a component you can use in your project.

Optional: the advanced mode shows the shader code. You can leave it alone as a beginner.

This part was not tried for this lesson. Effect Composer was called Qt Quick Effect Maker in older versions (renamed in Design Studio 4.4).

## 5. A simple 3D scene

A 3D scene needs four things:

| Item | Job |
|---|---|
| `View3D` | The 2D item that shows the 3D world. |
| `PerspectiveCamera` | The point of view. |
| `DirectionalLight` | Without a light you see black shapes. |
| `Model` | A shape. `source: "#Cube"`, `"#Sphere"` and `"#Rectangle"` are built-in primitives. |

```qml
View3D {
    camera: sceneCamera
    environment: SceneEnvironment {
        clearColor: "#dfe8e3"
        backgroundMode: SceneEnvironment.Color
    }
    PerspectiveCamera { position: Qt.vector3d(0, 200, 300); eulerRotation.x: -30 }
    DirectionalLight { eulerRotation.x: -35; eulerRotation.y: -60 }
    Model {
        id: cube
        source: "#Cube"
        position: Qt.vector3d(-70, 50, 0)
        materials: DefaultMaterial { diffuseColor: "#2e7d5a" }
    }
}
```

Work in the **3D** editor (top of the editor area, next to **2D**): select an item in the **Navigator**, move it with the gizmo, and look through the camera with the **Camera view** button. The 3D units are centimeters by default, so a cube is 100 units wide.

## 6. Animate the cube

The Timeline is the same as in lesson 07: one `KeyframeGroup` with two keyframes on `eulerRotation.y` (0 and 360) and a looping `TimelineAnimation`.

1. Select `cube`, open the **Timeline** view.
2. Press **Add Timeline**, then insert the keyframe on the property `eulerRotation.y`.
3. Frame 0: value 0. Frame 1000: value 360.
4. Set the animation to **Running in base state** and **Infinite** loops.

## Common problems

- **`View3D` is unknown:** add the Qt Quick 3D module in the Components view.
- **The 3D box shows only the background color:** set `camera: sceneCamera` on the `View3D`, then check in the 3D editor that the models are inside the camera view.
- **The scene is black:** there is no light, or the camera looks away from the models.
- **The leaf is shown twice:** set `visible: false` on the source image.
- **The effect is missing:** the effect needs a graphics API (OpenGL, Vulkan, Metal, Direct3D). On a machine without a GPU or in a virtual machine, effects and 3D may show nothing.
- **Do not anchor the cube:** 3D items use `position`, not anchors.

## Reference

- **Module:** `QtQuick.Effects` (`MultiEffect`), `QtQuick3D` (`View3D`, `Model`, `PerspectiveCamera`, `DirectionalLight`, `DefaultMaterial`), `QtQuick.Timeline`
- **Qt version:** Qt 6.8 or newer
- **Design Studio views:** 2D, 3D, Navigator, Components, Timeline, Effect Composer
- **Documentation:** the Qt Design Studio manual, chapters "Effects", "Effect Composer" and "3D"
