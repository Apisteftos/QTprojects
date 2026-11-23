#include "file_io.hpp"

#include <format>
#include <fstream>
#include <iterator>
#include <system_error>

namespace wb {

std::expected<std::string, std::string> readTextFile(const std::filesystem::path &path)
{
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec))
        return std::unexpected(std::format("Not a file: {}", path.string()));

    const auto size = std::filesystem::file_size(path, ec);
    if (ec)
        return std::unexpected(std::format("Cannot read the size of {}", path.string()));
    if (size > maxTextFileSize)
        return std::unexpected(std::format("File is too large ({} bytes, limit {})", size, maxTextFileSize));

    std::ifstream in(path, std::ios::binary);
    if (!in)
        return std::unexpected(std::format("Cannot open {}", path.string()));

    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

std::expected<void, std::string> writeTextFile(const std::filesystem::path &path,
                                               const std::string &content)
{
    std::error_code ec;
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path(), ec);
        if (ec)
            return std::unexpected(std::format("Cannot create the folder {}", path.parent_path().string()));
    }

    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out)
        return std::unexpected(std::format("Cannot open {} for writing", path.string()));
    out << content;
    if (!out)
        return std::unexpected(std::format("Cannot write {}", path.string()));
    return {};
}

} // namespace wb
