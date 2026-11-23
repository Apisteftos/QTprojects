#include "file_kind.hpp"

#include <algorithm>
#include <array>
#include <string>

namespace wb {

namespace {

struct Extension {
    std::string_view text;
    FileKind kind;
};

constexpr std::array<Extension, 15> extensions{{
    {"txt", FileKind::Text},   {"md", FileKind::Text},    {"log", FileKind::Text},
    {"csv", FileKind::Text},   {"json", FileKind::Text},  {"mp4", FileKind::Video},
    {"mkv", FileKind::Video},  {"avi", FileKind::Video},  {"mov", FileKind::Video},
    {"webm", FileKind::Video}, {"png", FileKind::Image},  {"jpg", FileKind::Image},
    {"jpeg", FileKind::Image}, {"bmp", FileKind::Image},  {"gif", FileKind::Image},
}};

} // namespace

FileKind fileKind(std::string_view path)
{
    // Only look at the last path part, so "folder.txt/readme" is not text.
    const auto slash = path.find_last_of("/\\");
    if (slash != std::string_view::npos)
        path.remove_prefix(slash + 1);

    const auto dot = path.find_last_of('.');
    if (dot == std::string_view::npos || dot + 1 >= path.size())
        return FileKind::Unknown;

    std::string extension(path.substr(dot + 1));
    std::ranges::transform(extension, extension.begin(),
                           [](unsigned char c) { return static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c); });

    const auto it = std::ranges::find(extensions, std::string_view(extension), &Extension::text);
    return it == extensions.end() ? FileKind::Unknown : it->kind;
}

std::string_view fileKindName(FileKind kind)
{
    switch (kind) {
    case FileKind::Text:
        return "text";
    case FileKind::Video:
        return "video";
    case FileKind::Image:
        return "image";
    case FileKind::Unknown:
        break;
    }
    return "unknown";
}

} // namespace wb
