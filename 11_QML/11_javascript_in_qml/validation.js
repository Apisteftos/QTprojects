// A JavaScript library: plain functions, no QML items.
// ".pragma library" means that there is only one copy of this file for the whole program
// and that it cannot see the items of the QML file that uses it. This is what keeps it testable.
.pragma library

// Each function returns "" if the value is valid, or the text of the error.

function nameError(text) {
    var name = text.trim()
    if (name.length === 0)
        return "Name is required"
    if (name.length < 2)
        return "Name is too short"
    return ""
}

function emailError(text) {
    if (text.length === 0)
        return "E-mail is required"
    // something@something.xx  (a simple check, not the full standard)
    if (!/^[^\s@]+@[^\s@]+\.[^\s@]{2,}$/.test(text))
        return "This is not an e-mail address"
    return ""
}

function ageError(text) {
    if (text.length === 0)
        return "Age is required"
    var value = Number(text)
    if (!Number.isInteger(value))
        return "Age must be a whole number"
    if (value < 18 || value > 120)
        return "Age must be between 18 and 120"
    return ""
}

function passwordError(text) {
    if (text.length < 8)
        return "At least 8 characters"
    if (!/[0-9]/.test(text))
        return "At least one digit"
    return ""
}
