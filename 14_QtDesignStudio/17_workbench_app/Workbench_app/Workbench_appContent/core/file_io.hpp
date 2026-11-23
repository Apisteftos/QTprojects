#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <string>

namespace wb {

// Largest text file that is read: 5 MB.
inline constexpr std::size_t maxTextFileSize = 5 * 1024 * 1024;

// Reads a whole file as bytes (UTF-8 text). Returns an error text if the file
// does not exist, is not a regular file, is too large or cannot be read.
std::expected<std::string, std::string> readTextFile(const std::filesystem::path &path);

// Writes the text to the file and creates missing folders. Returns an error text on failure.
std::expected<void, std::string> writeTextFile(const std::filesystem::path &path,
                                               const std::string &content);

} // namespace wb
