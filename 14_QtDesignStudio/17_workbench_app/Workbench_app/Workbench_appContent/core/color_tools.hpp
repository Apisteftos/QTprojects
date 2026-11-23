#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace wb {

struct Rgb {
    int r = 0;
    int g = 0;
    int b = 0;

    bool operator==(const Rgb &) const = default;
};

// "#RRGGBB" in upper case. Every channel is clamped to 0..255.
std::string toHex(Rgb color);

// Accepts "#RGB", "#RRGGBB", "RGB" and "RRGGBB" (any case).
// Returns nothing for any other text.
std::optional<Rgb> parseHex(std::string_view text);

// "rgb(46, 125, 90)"
std::string toRgbText(Rgb color);

} // namespace wb
