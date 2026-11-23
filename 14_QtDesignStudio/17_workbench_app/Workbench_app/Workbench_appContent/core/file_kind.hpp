#pragma once

#include <string_view>

namespace wb {

enum class FileKind { Unknown, Text, Video, Image };

// Decides which tab opens a file, by its extension (any case):
//   Text:  .txt .md .log .csv .json
//   Video: .mp4 .mkv .avi .mov .webm
//   Image: .png .jpg .jpeg .bmp .gif
FileKind fileKind(std::string_view path);

// "text", "video", "image" or "unknown" (the names the QML code uses).
std::string_view fileKindName(FileKind kind);

} // namespace wb
