// Tests of the core library. No test framework: a failed check prints a line,
// and the program returns 1 if at least one check failed.

#include <cmath>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>

#include "color_tools.hpp"
#include "countdown.hpp"
#include "file_io.hpp"
#include "file_kind.hpp"
#include "recent_files.hpp"
#include "text_tools.hpp"
#include "unit_converter.hpp"

namespace {

int checks = 0;
int failures = 0;

void check(bool condition, const std::string &what)
{
    ++checks;
    if (!condition) {
        ++failures;
        std::cout << std::format("FAILED: {}\n", what);
    }
}

bool near(double a, double b, double eps = 1e-6)
{
    return std::abs(a - b) < eps;
}

void testUnits()
{
    const auto feet = wb::convertUnit("Length", "Meter", "Foot", 12.5);
    check(feet && near(*feet, 41.01049, 1e-4), "12.5 m is about 41.0105 ft");

    const auto km = wb::convertUnit("Length", "Mile", "Kilometer", 1.0);
    check(km && near(*km, 1.609344), "1 mile is 1.609344 km");

    const auto same = wb::convertUnit("Weight", "Gram", "Gram", 7.0);
    check(same && near(*same, 7.0), "same unit does not change the value");

    const auto pounds = wb::convertUnit("Weight", "Kilogram", "Pound", 1.0);
    check(pounds && near(*pounds, 2.2046226, 1e-6), "1 kg is about 2.2046 lb");

    const auto fahrenheit = wb::convertUnit("Temperature", "Celsius", "Fahrenheit", 100.0);
    check(fahrenheit && near(*fahrenheit, 212.0), "100 C is 212 F");
    const auto kelvin = wb::convertUnit("Temperature", "Fahrenheit", "Kelvin", 32.0);
    check(kelvin && near(*kelvin, 273.15), "32 F is 273.15 K");

    check(!wb::convertUnit("Length", "Meter", "Gram", 1.0), "unknown unit is an error");
    check(!wb::convertUnit("Speed", "Meter", "Foot", 1.0), "unknown category is an error");

    check(wb::unitCategories().size() == 3, "three categories");
    check(wb::unitsOf("Length").size() == 6, "six length units");
    check(wb::unitsOf("Nothing").empty(), "unknown category has no units");
    check(wb::unitsOf("Length")[0] == "Meter" && wb::unitsOf("Length")[1] == "Foot", "length starts with Meter and Foot");
    check(wb::unitsOf("Temperature")[1] == "Fahrenheit", "temperature starts with Celsius and Fahrenheit");

    check(wb::formatNumber(41.0105) == "41.0105", "format keeps 4 decimals");
    check(wb::formatNumber(2.5) == "2.5", "format removes trailing zeros");
    check(wb::formatNumber(3.0) == "3", "format removes the dot");
    check(wb::formatNumber(-0.00001) == "0", "format has no negative zero");
}

void testColors()
{
    check(wb::toHex({46, 125, 90}) == "#2E7D5A", "hex of 46,125,90");
    check(wb::toHex({-5, 300, 0}) == "#00FF00", "hex clamps the channels");

    const auto full = wb::parseHex("#2e7d5a");
    check(full && *full == wb::Rgb{46, 125, 90}, "parse #2e7d5a");
    const auto noHash = wb::parseHex("2E7D5A");
    check(noHash && *noHash == wb::Rgb{46, 125, 90}, "parse without #");
    const auto shortForm = wb::parseHex("#fa0");
    check(shortForm && *shortForm == wb::Rgb{255, 170, 0}, "parse the short form #fa0");

    check(!wb::parseHex(""), "empty text is invalid");
    check(!wb::parseHex("#12"), "two digits are invalid");
    check(!wb::parseHex("#12345"), "five digits are invalid");
    check(!wb::parseHex("#GG0000"), "non-hex characters are invalid");

    check(wb::toRgbText({46, 125, 90}) == "rgb(46, 125, 90)", "rgb text");
    const auto back = wb::parseHex(wb::toHex({1, 2, 3}));
    check(back && *back == wb::Rgb{1, 2, 3}, "round trip");
}

void testCountdown()
{
    wb::Countdown timer;
    timer.setTotal(272);
    check(timer.text() == "04:32", "text of 272 seconds");
    check(wb::formatClock(0) == "00:00" && wb::formatClock(-4) == "00:00", "clock of 0 and negative");
    check(wb::formatClock(3725) == "62:05", "minutes beyond 59");
    check(!timer.running(), "not running after setTotal");

    check(!timer.tick(10) && timer.remaining() == 272, "tick does nothing while stopped");

    timer.start();
    check(timer.running(), "running after start");
    check(!timer.tick(2) && timer.remaining() == 270, "tick reduces the time");
    check(near(timer.progress(), 2.0 / 272.0), "progress after 2 seconds");

    timer.stop();
    timer.tick(5);
    check(timer.remaining() == 270, "stop pauses the time");

    timer.start();
    check(timer.tick(1000), "tick returns true when the time is over");
    check(timer.remaining() == 0 && !timer.running(), "finished countdown stops");
    check(near(timer.progress(), 1.0), "progress is 1 when finished");
    check(!timer.tick(1), "finished is reported only once");

    timer.start();
    check(timer.running() && timer.remaining() == 272, "start after the end begins again");

    timer.reset();
    check(!timer.running() && timer.remaining() == 272, "reset");

    wb::Countdown empty;
    empty.start();
    check(!empty.running() && near(empty.progress(), 0.0), "total 0 cannot run");
    empty.setTotal(-5);
    check(empty.total() == 0, "negative total becomes 0");
}

void testText()
{
    const std::u16string text = u"Water the plants.\nThe plants like water.\nwater";

    check(wb::findNext(text, u"plants", 0, true) == 10, "first match");
    check(wb::findNext(text, u"plants", 11, true) == 22, "next match");
    check(!wb::findNext(text, u"plants", 23, true), "no match after the last");
    check(!wb::findNext(text, u"Plants", 0, true), "case sensitive");
    check(wb::findNext(text, u"PLANTS", 0, false) == 10, "case insensitive");
    check(!wb::findNext(text, u"", 0, false), "empty query never matches");
    check(!wb::findNext(u"ab", u"abc", 0, false), "query longer than the text");

    check(wb::countMatches(text, u"water", false) == 3, "three matches of water");
    check(wb::countMatches(text, u"water", true) == 2, "two case-sensitive matches");
    check(wb::countMatches(u"aaaa", u"aa", true) == 2, "matches do not overlap");

    check(wb::countLines(u"") == 0, "empty text has no lines");
    check(wb::countLines(u"one") == 1, "one line");
    check(wb::countLines(u"one\ntwo") == 2, "two lines");
    check(wb::countLines(u"one\ntwo\n") == 2, "final line break");
    check(wb::countLines(u"\n\n") == 2, "empty lines count");

    check(wb::noteFileName(u"Garden plan") == "garden-plan.txt", "file name from a title");
    check(wb::noteFileName(u"  A/B: C?? ") == "a-b-c.txt", "unsafe characters");
    check(wb::noteFileName(u"") == "note.txt", "empty title");
    check(wb::noteFileName(u"???") == "note.txt", "only unsafe characters");
    check(wb::noteFileName(u"Plan 2027") == "plan-2027.txt", "digits are kept");
}

void testKinds()
{
    check(wb::fileKind("/data/notes.TXT") == wb::FileKind::Text, "txt in capitals");
    check(wb::fileKind("a/readme.md") == wb::FileKind::Text, "md");
    check(wb::fileKind("clip.mp4") == wb::FileKind::Video, "mp4");
    check(wb::fileKind("C:\\clips\\clip.WebM") == wb::FileKind::Video, "windows path");
    check(wb::fileKind("leaf.png") == wb::FileKind::Image, "png");
    check(wb::fileKind("photo.jpeg") == wb::FileKind::Image, "jpeg");
    check(wb::fileKind("archive.zip") == wb::FileKind::Unknown, "unknown extension");
    check(wb::fileKind("noextension") == wb::FileKind::Unknown, "no extension");
    check(wb::fileKind("folder.txt/readme") == wb::FileKind::Unknown, "dot in a folder name");
    check(wb::fileKind("trailingdot.") == wb::FileKind::Unknown, "trailing dot");
    check(wb::fileKindName(wb::FileKind::Video) == "video", "kind name");
    check(wb::fileKindName(wb::FileKind::Unknown) == "unknown", "unknown name");
}

void testRecent()
{
    wb::RecentFiles recent(3);
    recent.add("a.txt");
    recent.add("b.txt");
    recent.add("c.txt");
    check(recent.items().front() == "c.txt", "newest first");

    recent.add("a.txt");
    check(recent.items().size() == 3 && recent.items().front() == "a.txt", "existing entry moves to the front");

    recent.add("d.txt"); // list was a, c, b: b is the oldest
    check(recent.items().size() == 3 && recent.items().back() == "c.txt", "oldest entry is dropped");

    recent.add("");
    check(recent.items().size() == 3, "empty path is ignored");

    recent.remove("d.txt");
    check(recent.items().size() == 2, "remove");
    recent.clear();
    check(recent.items().empty(), "clear");
}

void testFiles()
{
    const auto folder = std::filesystem::temp_directory_path() / "workbench_core_test";
    std::filesystem::remove_all(folder);
    const auto file = folder / "sub" / "note.txt";

    const auto written = wb::writeTextFile(file, "line one\nline two\n");
    check(written.has_value(), "write creates the folders");

    const auto read = wb::readTextFile(file);
    check(read && *read == "line one\nline two\n", "read returns what was written");

    check(!wb::readTextFile(folder / "missing.txt"), "missing file is an error");
    check(!wb::readTextFile(folder), "a folder is not a text file");

    const auto big = folder / "big.txt";
    check(wb::writeTextFile(big, std::string(wb::maxTextFileSize + 1, 'x')).has_value(), "write a big file");
    check(!wb::readTextFile(big), "a too large file is refused");

    std::filesystem::remove_all(folder);
}

} // namespace

int main()
{
    testUnits();
    testColors();
    testCountdown();
    testText();
    testKinds();
    testRecent();
    testFiles();

    std::cout << std::format("{} checks, {} failed\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
