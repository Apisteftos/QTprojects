.pragma library

// Small functions without any user interface: the easiest kind of code to test.

function clamp(value, low, high) {
    return Math.min(Math.max(value, low), high)
}

// 1250 -> "12.50"   (cents as integers: no rounding problems with decimals)
function formatPrice(cents) {
    const whole = Math.floor(cents / 100)
    const rest = cents % 100
    return whole + "." + (rest < 10 ? "0" : "") + rest
}

// A name is valid with 2 to 20 characters, spaces at the ends do not count.
function isValidName(text) {
    const length = text.trim().length
    return length >= 2 && length <= 20
}
