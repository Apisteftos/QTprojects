#include "text_tools.hpp"

namespace wb {

namespace {

char16_t fold(char16_t c, bool caseSensitive)
{
    if (!caseSensitive && c >= u'A' && c <= u'Z')
        return static_cast<char16_t>(c - u'A' + u'a');
    return c;
}

bool matchesAt(std::u16string_view text, std::u16string_view query, std::size_t pos, bool caseSensitive)
{
    if (pos + query.size() > text.size())
        return false;
    for (std::size_t i = 0; i < query.size(); ++i) {
        if (fold(text[pos + i], caseSensitive) != fold(query[i], caseSensitive))
            return false;
    }
    return true;
}

} // namespace

std::optional<std::size_t> findNext(std::u16string_view text,
                                    std::u16string_view query,
                                    std::size_t from,
                                    bool caseSensitive)
{
    if (query.empty() || query.size() > text.size())
        return std::nullopt;
    for (std::size_t pos = from; pos + query.size() <= text.size(); ++pos) {
        if (matchesAt(text, query, pos, caseSensitive))
            return pos;
    }
    return std::nullopt;
}

std::size_t countMatches(std::u16string_view text, std::u16string_view query, bool caseSensitive)
{
    std::size_t count = 0;
    std::size_t from = 0;
    while (const auto pos = findNext(text, query, from, caseSensitive)) {
        ++count;
        from = *pos + query.size();
    }
    return count;
}

std::size_t countLines(std::u16string_view text)
{
    if (text.empty())
        return 0;
    std::size_t lines = 1;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == u'\n' && i + 1 < text.size())
            ++lines;
    }
    return lines;
}

std::string noteFileName(std::u16string_view title)
{
    std::string name;
    bool lastWasDash = true; // no dash at the start
    for (const char16_t c : title) {
        const bool isDigit = c >= u'0' && c <= u'9';
        const bool isLower = c >= u'a' && c <= u'z';
        const bool isUpper = c >= u'A' && c <= u'Z';
        if (isDigit || isLower || isUpper) {
            name.push_back(static_cast<char>(isUpper ? c - u'A' + u'a' : c));
            lastWasDash = false;
        } else if (!lastWasDash) {
            name.push_back('-');
            lastWasDash = true;
        }
    }
    if (!name.empty() && name.back() == '-')
        name.pop_back();
    if (name.empty())
        name = "note";
    return name + ".txt";
}

} // namespace wb
