#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace wb {

// The list behind the File > Recent menu. Newest entry first, no duplicates.
class RecentFiles {
public:
    explicit RecentFiles(std::size_t maxEntries = 5) : m_max(maxEntries) {}

    // Puts the path at the front. An existing entry moves to the front.
    // The oldest entries are dropped when the list is longer than the maximum.
    void add(const std::string &path);

    void remove(const std::string &path);
    void clear() { m_items.clear(); }

    const std::vector<std::string> &items() const { return m_items; }

private:
    std::size_t m_max;
    std::vector<std::string> m_items;
};

} // namespace wb
