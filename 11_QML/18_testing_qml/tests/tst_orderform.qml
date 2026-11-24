import QtQuick
import QtTest
import ".."

TestCase {
    id: testCase
    name: "OrderForm"
    visible: true
    when: windowShown

    Component { id: formComponent; OrderForm { width: 300 } }
    SignalSpy { id: orderedSpy; signalName: "ordered" }

    property OrderForm form: null

    function init() {
        form = createTemporaryObject(formComponent, testCase)
        orderedSpy.target = form
        orderedSpy.clear()
    }

    // helper: type a text into the name field like a user
    function typeName(text) {
        const field = findChild(form, "nameField")
        field.forceActiveFocus()
        verify(field.activeFocus, "the field has no focus")
        for (let i = 0; i < text.length; ++i)
            keyClick(text[i])
    }

    function test_startState() {
        compare(form.valid, false)
        compare(findChild(form, "orderButton").enabled, false)
        compare(findChild(form, "hint").visible, false)       // nothing typed: no hint yet
        compare(findChild(form, "total").text, "Total: 2.50")
    }

    function test_hintForShortName() {
        typeName("A")
        compare(findChild(form, "nameField").text, "A")
        compare(findChild(form, "hint").visible, true)
        compare(findChild(form, "orderButton").enabled, false)
    }

    function test_orderFlow() {
        typeName("Al")
        const button = findChild(form, "orderButton")
        compare(button.enabled, true)
        compare(findChild(form, "hint").visible, false)

        mouseClick(findChild(findChild(form, "stepper"), "plus"))     // quantity 2
        compare(findChild(form, "total").text, "Total: 5.00")

        mouseClick(button)
        compare(form.status, "Ordered 2 x for Al")
        compare(findChild(form, "status").text, "Ordered 2 x for Al")
        compare(orderedSpy.count, 1)
        compare(orderedSpy.signalArguments[0][0], "Al")
        compare(orderedSpy.signalArguments[0][1], 2)
    }

    // --- tryCompare waits up to 5 seconds until the property has the value (for animations, timers) ---
    function test_changedUnitPrice() {
        form.unitPrice = 1000
        tryCompare(findChild(form, "total"), "text", "Total: 10.00")
    }
}
