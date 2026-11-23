# 13_figma_import

Bring a design from Figma into Qt Design Studio. A designer draws the screens in Figma, exports them with the plugin **Qt Bridge for Figma**, and you import the result into a Design Studio project, where each Figma component becomes a Qt Design Studio component.

**Status: reference lesson, not tested.** This lesson has no code and no sample project. The import needs a Qt Design Studio **Enterprise** license, and the Design Studio used for this tutorial (4.8.2, Flatpak, Community) does not have it. Everything below comes from the Qt documentation. Check it against your own installation before you rely on it.

---

## 1. What It Is

| Part | Meaning |
|------|---------|
| **Qt Bridge for Figma** | A plugin for Figma that exports a design to a `.qtbridge` archive |
| **`.qtbridge` archive** | A file with the metadata and the assets of the design. It is a zip file inside |
| **Import** | Design Studio reads the archive and creates QML components, images and a scene from it |

According to the Qt documentation, each Figma component is exported as a Qt Design Studio component, and each Figma component instance becomes a component instance in the files that are generated at the import.

---

## 2. License

| Question | Answer |
|----------|--------|
| Who needs the plugin? | The person who exports in Figma. You need both a Figma account and a Qt account |
| Who needs the Enterprise license? | The Design Studio that **imports** the `.qtbridge` file. The Qt documentation states that Qt Bridge for Figma is included in the Qt Design Studio Enterprise license |
| What if the license is missing? | Forum users report that `.qtbridge` does not appear in the file type list of the import dialog, and the import fails with a "file type not supported" error |

Qt offers a free evaluation of the Enterprise edition on its Design Studio product page, if you want to try the workflow.

---

## 3. Export From Figma

Following the Qt documentation:

1. Open the **Qt Bridge for Figma** plugin page and choose **Try it out**, then select your logged-in Figma account. After the plugin is installed you can also start it in Figma from **Plugins > Qt Bridge for Figma**.
2. After the plugin loads, choose **Run**.
3. Check the hints in the **Node Info** section of the plugin and apply the ones that make sense.
4. Choose **Export** to save the `.qtbridge` file on your computer.
5. Read the log of the export.

## 4. Import Into Design Studio

1. Open or create a project in Design Studio.
2. Drag the `.qtbridge` file into the **2D**, **3D**, **Assets** or **Navigator** view. Or use the **Plus** button of the Assets view to add it as an asset.
3. Choose **Import** in the dialog and wait until the process is finished.
4. The generated components appear in the project, under **Components > My Components**, and the scene shows the imported screens.

If you edit the design in Figma and export again, name the new file exactly like the first export, so that the import updates the existing files.

---

## 5. Organize the Figma Design

The Qt documentation lists guidelines for the best result. The main ideas:

| Guideline | Why |
|-----------|-----|
| Use Figma **components** for everything that is reused (buttons, cards, list rows) | Each one becomes a Design Studio component (lesson 04) |
| Use **component variants** for the states of a component (normal, pressed, disabled) | The variants become the states of the Design Studio component (lessons 05 and 06) |
| Name the layers clearly, without special characters | The names become ids and file names. Lesson 12 showed that some ids are rejected |
| Add or remove layers across variants to create the differences | This is described as fine and encouraged |
| Start from the Qt Quick control template components for Figma, if you need real controls | They generate functional QML controls (lesson 10) |

---

## 6. If You Do Not Have the Enterprise License

You can still use a Figma design, with more manual work:

1. In Figma, export the images and icons as PNG files.
2. Import them into your project as assets and place them as Image items (lesson 02).
3. Rebuild the layout with Rectangles, Text and anchors (lessons 02 and 03), using the Figma screens as a visual guide. Take colors and sizes from the Figma inspector.

You lose the automatic components, but the result is a clean QML project.

---

## 7. Common Problems

| Problem | Cause and fix |
|---------|---------------|
| `.qtbridge` is not in the file type list | The import plugin is missing: no Enterprise license, or the plugin is not installed in this Design Studio |
| "File format not supported" at the import | The same cause. Try the evaluation of the Enterprise edition |
| Layers have strange ids after the import | Layer names in Figma have spaces or special characters. Rename them in Figma and export again |
| The imported design looks different | Effects or constraints in Figma that have no QML equivalent. Check the export log and the hints in Node Info |
| The second import creates duplicates | The new archive was not named exactly like the first one |

---

## 8. Reference

| Topic | Where to look |
|-------|---------------|
| Exporting designs from Figma | <https://doc.qt.io/qtdesignstudio/figmaqtbridge.html> |
| Setting up Qt Bridge for Figma | <https://doc.qt.io/qtdesignstudio/qtbridge-figma-setup.html> |
| Using Qt Bridge for Figma | <https://doc.qt.io/qtdesignstudio/qtbridge-figma-using.html> |
| Using Figma Quick Control template components | <https://doc.qt.io/qtdesignstudio/qtbridge-figma-template.html> |
| Qt Bridge overview | <https://doc.qt.io/qtdesignstudio/qtbridge-overview.html> |
| Importing 2D assets | <https://doc.qt.io/qtdesignstudio/studio-importing-2d.html> |
