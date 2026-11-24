import QtQuick
import QtTest
import "../logic.js" as Logic

// A TestCase is an object. Every function that starts with "test_" is one test.
TestCase {
    name: "Logic"                       // shown in the output: "Logic::test_clamp"

    // --- one test, several checks ---
    function test_clamp() {
        compare(Logic.clamp(5, 0, 10), 5)
        compare(Logic.clamp(-3, 0, 10), 0)
        compare(Logic.clamp(99, 0, 10), 10)
    }

    // --- data-driven: "<name>_data()" returns the rows, the test runs once for each row ---
    function test_formatPrice_data() {
        return [
            { tag: "zero",        cents: 0,    expected: "0.00" },
            { tag: "small",       cents: 5,    expected: "0.05" },
            { tag: "round",       cents: 1200, expected: "12.00" },
            { tag: "with cents",  cents: 1250, expected: "12.50" },
            { tag: "big",         cents: 100001, expected: "1000.01" }
        ]
    }
    function test_formatPrice(data) {
        compare(Logic.formatPrice(data.cents), data.expected)
    }

    function test_isValidName_data() {
        return [
            { tag: "empty",          text: "",                      valid: false },
            { tag: "one letter",     text: "A",                     valid: false },
            { tag: "two letters",    text: "Al",                    valid: true },
            { tag: "spaces only",    text: "     ",                 valid: false },
            { tag: "spaces around",  text: "  Al  ",                valid: true },
            { tag: "20 characters",  text: "abcdefghijklmnopqrst",  valid: true },
            { tag: "21 characters",  text: "abcdefghijklmnopqrstu", valid: false }
        ]
    }
    function test_isValidName(data) {
        compare(Logic.isValidName(data.text), data.valid)
    }
}
