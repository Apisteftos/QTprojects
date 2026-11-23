#include "color_tools.hpp"

#include <algorithm>
#include <cctype>
#include <format>

namespace wb {

namespace {

int hexValue(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

} // namespace

std::string toHex(Rgb color)
{
    auto clamp = [](int v) { return std::clamp(v, 0, 255); };
    return std::format("#{:02X}{:02X}{:02X}", clamp(color.r), clamp(color.g), clamp(color.b));
}

std::optional<Rgb> parseHex(std::string_view text)
{
    if (!text.empty() && text.front() == '#')
        text.remove_prefix(1);
    if (text.size() != 3 && text.size() != 6)
        return std::nullopt;

    int digits[6] = {};
    for (std::size_t i = 0; i < text.size(); ++i) {
        const int value = hexValue(text[i]);
        if (value < 0)
            return std::nullopt;
        digits[i] = value;
    }

    if (text.size() == 3)
        return Rgb{digits[0] * 17, digits[1] * 17, digits[2] * 17};
    return Rgb{digits[0] * 16 + digits[1], digits[2] * 16 + digits[3], digits[4] * 16 + digits[5]};
}

std::string toRgbText(Rgb color)
{
    return std::format("rgb({}, {}, {})", std::clamp(color.r, 0, 255), std::clamp(color.g, 0, 255),
                       std::clamp(color.b, 0, 255));
}

} // namespace wb
