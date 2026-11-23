#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

namespace wb {

// Names of all categories: "Length", "Weight", "Temperature".
std::vector<std::string> unitCategories();

// Names of the units of one category. Empty if the category is unknown.
std::vector<std::string> unitsOf(std::string_view category);

// Converts a value between two units of the same category.
// Returns an error text for an unknown category or unit.
std::expected<double, std::string> convertUnit(std::string_view category,
                                               std::string_view from,
                                               std::string_view to,
                                               double value);

// Formats a number for the screen: at most 4 decimals, no trailing zeros.
std::string formatNumber(double value);

} // namespace wb
