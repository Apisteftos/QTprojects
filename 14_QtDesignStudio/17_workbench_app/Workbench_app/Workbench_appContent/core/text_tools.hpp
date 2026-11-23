#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

namespace wb {

// All text functions work on UTF-16 (std::u16string_view), the same unit
// that QString and the QML TextArea use. A position returned here can be
// passed to TextArea.select() without any conversion.
//
// Limitation: upper/lower case comparison only folds the letters A-Z.

// Position of the next match at or after `from`. Nothing if there is none.
// An empty query never matches.
std::optional<std::size_t> findNext(std::u16string_view text,
                                    std::u16string_view query,
                                    std::size_t from,
                                    bool caseSensitive);

// Number of matches that do not overlap.
std::size_t countMatches(std::u16string_view text, std::u16string_view query, bool caseSensitive);

// Number of lines. "" has 0 lines. A final line break does not start a new line.
std::size_t countLines(std::u16string_view text);

// Turns a note title into a safe file name: "Garden plan" -> "garden-plan.txt".
// Letters and digits are kept (A-Z become lower case), everything else becomes
// one "-". An empty result becomes "note.txt".
std::string noteFileName(std::u16string_view title);

} // namespace wb
