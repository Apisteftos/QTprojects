import QtQuick
import QtTest
import ".."                          // the folder above: the types Stepper and OrderForm

TestCase {
    id: testCase
    name: "Stepper"
    visible: true                      // a TestCase is invisible by default; its children must be visible for mouse and keys
    when: windowShown                  // wait until the test window is visible

    // The component to test. Every test creates its own object, so the tests do not depend on each other.
    Component {
        id: stepperComponent
        Stepper { from: 0; to: 3; value: 1 }
    }

    SignalSpy {
        id: limitSpy
        signalName: "limitReached"
    }

    property Stepper stepper: null

    function init() {                  // runs before every test
        stepper = createTemporaryObject(stepperComponent, testCase)   // destroyed after the test
        verify(stepper, "the component could not be created")
        limitSpy.target = stepper
        limitSpy.clear()
    }

    function test_initialValue() {
        compare(stepper.value, 1)
    }

    function test_incrementAndDecrement() {
        stepper.increment()
        compare(stepper.value, 2)
        stepper.decrement()
        stepper.decrement()
        compare(stepper.value, 0)
    }

    function test_limitSendsSignal() {
        stepper.setValue(3)
        stepper.increment()
        compare(stepper.value, 3)               // did not move
        compare(limitSpy.count, 1)              // one signal
        compare(limitSpy.signalArguments[0][0], "max")

        stepper.setValue(0)
        stepper.decrement()
        compare(limitSpy.count, 2)
        compare(limitSpy.signalArguments[1][0], "min")
    }

    function test_setValueClamps() {
        stepper.setValue(100)
        compare(stepper.value, 3)
        stepper.setValue(-100)
        compare(stepper.value, 0)
    }

    // --- the user interface: find the buttons and click them like a user ---
    function test_buttons() {
        const plus = findChild(stepper, "plus")
        const minus = findChild(stepper, "minus")
        const label = findChild(stepper, "valueLabel")
        verify(plus && minus && label)

        mouseClick(plus)
        compare(stepper.value, 2)
        compare(label.text, "2")

        mouseClick(plus)
        compare(stepper.value, 3)
        compare(plus.enabled, false)            // at the limit the button is disabled
        mouseClick(plus)                        // a click on a disabled button does nothing
        compare(stepper.value, 3)

        mouseClick(minus)
        compare(stepper.value, 2)
        compare(plus.enabled, true)
    }

    // --- a bug that was found once: stays in the tests as a guard ---
    function test_noSignalInTheMiddle() {
        stepper.increment()
        stepper.decrement()
        compare(limitSpy.count, 0)
    }
}
