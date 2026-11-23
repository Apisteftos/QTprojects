#include "recent_files.hpp"

#include <algorithm>

namespace wb {

void RecentFiles::add(const std::string &path)
{
    if (path.empty())
        return;
    remove(path);
    m_items.insert(m_items.begin(), path);
    if (m_items.size() > m_max)
        m_items.resize(m_max);
}

void RecentFiles::remove(const std::string &path)
{
    std::erase(m_items, path);
}

} // namespace wb
