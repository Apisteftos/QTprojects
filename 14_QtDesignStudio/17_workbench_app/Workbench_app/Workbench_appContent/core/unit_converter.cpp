#include "unit_converter.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <format>

namespace wb {

namespace {

// A unit of a linear category: value_in_base = value * factor.
struct LinearUnit {
    std::string_view name;
    double factor;
};

// The order matters: the screen starts with the first two units of a category,
// so each category starts with a metric unit and an imperial unit.
constexpr std::array<LinearUnit, 6> lengthUnits{{
    {"Meter", 1.0},
    {"Foot", 0.3048},
    {"Kilometer", 1000.0},
    {"Mile", 1609.344},
    {"Centimeter", 0.01},
    {"Inch", 0.0254},
}};

constexpr std::array<LinearUnit, 4> weightUnits{{
    {"Kilogram", 1.0},
    {"Pound", 0.45359237},
    {"Gram", 0.001},
    {"Ounce", 0.028349523125},
}};

constexpr std::array<std::string_view, 3> temperatureUnits{"Celsius", "Fahrenheit", "Kelvin"};

template <std::size_t N>
std::expected<double, std::string> convertLinear(const std::array<LinearUnit, N> &table,
                                                 std::string_view from,
                                                 std::string_view to,
                                                 double value)
{
    auto find = [&](std::string_view name) {
        return std::ranges::find(table, name, &LinearUnit::name);
    };
    const auto fromIt = find(from);
    const auto toIt = find(to);
    if (fromIt == table.end())
        return std::unexpected(std::format("Unknown unit: {}", from));
    if (toIt == table.end())
        return std::unexpected(std::format("Unknown unit: {}", to));
    return value * fromIt->factor / toIt->factor;
}

std::expected<double, std::string> convertTemperature(std::string_view from,
                                                      std::string_view to,
                                                      double value)
{
    auto known = [](std::string_view name) {
        return std::ranges::find(temperatureUnits, name) != temperatureUnits.end();
    };
    if (!known(from))
        return std::unexpected(std::format("Unknown unit: {}", from));
    if (!known(to))
        return std::unexpected(std::format("Unknown unit: {}", to));

    // Go through Celsius.
    double celsius = value;
    if (from == "Fahrenheit")
        celsius = (value - 32.0) * 5.0 / 9.0;
    else if (from == "Kelvin")
        celsius = value - 273.15;

    if (to == "Fahrenheit")
        return celsius * 9.0 / 5.0 + 32.0;
    if (to == "Kelvin")
        return celsius + 273.15;
    return celsius;
}

} // namespace

std::vector<std::string> unitCategories()
{
    return {"Length", "Weight", "Temperature"};
}

std::vector<std::string> unitsOf(std::string_view category)
{
    std::vector<std::string> names;
    if (category == "Length") {
        for (const auto &unit : lengthUnits)
            names.emplace_back(unit.name);
    } else if (category == "Weight") {
        for (const auto &unit : weightUnits)
            names.emplace_back(unit.name);
    } else if (category == "Temperature") {
        for (const auto name : temperatureUnits)
            names.emplace_back(name);
    }
    return names;
}

std::expected<double, std::string> convertUnit(std::string_view category,
                                               std::string_view from,
                                               std::string_view to,
                                               double value)
{
    if (category == "Length")
        return convertLinear(lengthUnits, from, to, value);
    if (category == "Weight")
        return convertLinear(weightUnits, from, to, value);
    if (category == "Temperature")
        return convertTemperature(from, to, value);
    return std::unexpected(std::format("Unknown category: {}", category));
}

std::string formatNumber(double value)
{
    if (!std::isfinite(value))
        return "-";
    std::string text = std::format("{:.4f}", value);
    // Remove trailing zeros and a trailing dot.
    while (text.back() == '0')
        text.pop_back();
    if (text.back() == '.')
        text.pop_back();
    if (text == "-0")
        text = "0";
    return text;
}

} // namespace wb
