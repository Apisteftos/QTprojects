#include "workbench_backend.h"

#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QSettings>
#include <QStandardPaths>

#include <filesystem>
#include <string>

#include "color_tools.hpp"
#include "file_io.hpp"
#include "file_kind.hpp"
#include "text_tools.hpp"
#include "unit_converter.hpp"

namespace {

// QString (UTF-16) -> std::string (UTF-8), and back.
std::string toStd(const QString &text)
{
    return text.toStdString();
}

QString fromStd(const std::string &text)
{
    return QString::fromStdString(text);
}

QStringList fromStdList(const std::vector<std::string> &items)
{
    QStringList list;
    for (const auto &item : items)
        list.append(fromStd(item));
    return list;
}

// A view of the UTF-16 data of a QString, without a copy.
std::u16string_view view(const QString &text)
{
    // QString::utf16() returns const ushort*, a view needs const char16_t*. Both are 16 bits.
    return std::u16string_view(reinterpret_cast<const char16_t *>(text.utf16()),
                               static_cast<std::size_t>(text.size()));
}

std::filesystem::path toPath(const QString &path)
{
    return std::filesystem::path(path.toStdU16String());
}

} // namespace

WorkbenchBackend::WorkbenchBackend(QObject *parent)
    : QObject(parent)
{
    QSettings settings("Workbench", "Workbench");
    const QStringList saved = settings.value("recent").toStringList();
    // The list is stored newest first. Add the oldest first to keep the order.
    for (auto it = saved.crbegin(); it != saved.crend(); ++it)
        m_recent.add(toStd(*it));

    m_countdown.setTotal(5 * 60);
    m_secondTimer.setInterval(1000);
    connect(&m_secondTimer, &QTimer::timeout, this, &WorkbenchBackend::onSecondPassed);
}

QStringList WorkbenchBackend::recentFiles() const
{
    return fromStdList(m_recent.items());
}

QString WorkbenchBackend::countdownText() const
{
    return fromStd(m_countdown.text());
}

// ---- Files ----

QUrl WorkbenchBackend::fileUrl(const QString &path) const
{
    return QUrl::fromLocalFile(path);
}

QString WorkbenchBackend::localPath(const QUrl &url) const
{
    return url.toLocalFile();
}

QString WorkbenchBackend::fileName(const QString &path) const
{
    return QFileInfo(path).fileName();
}

QString WorkbenchBackend::fileKind(const QString &path) const
{
    const std::string_view name = wb::fileKindName(wb::fileKind(toStd(path)));
    return QString::fromUtf8(name.data(), static_cast<qsizetype>(name.size()));
}

QString WorkbenchBackend::readTextFile(const QUrl &url)
{
    const auto result = wb::readTextFile(toPath(url.toLocalFile()));
    if (!result) {
        setLastError(fromStd(result.error()));
        return {};
    }
    setLastError({});
    return QString::fromUtf8(result->data(), static_cast<qsizetype>(result->size()));
}

void WorkbenchBackend::addRecent(const QString &path)
{
    m_recent.add(toStd(path));
    saveRecent();
    emit recentFilesChanged();
}

void WorkbenchBackend::clearRecent()
{
    m_recent.clear();
    saveRecent();
    emit recentFilesChanged();
}

void WorkbenchBackend::saveRecent() const
{
    QSettings settings("Workbench", "Workbench");
    settings.setValue("recent", recentFiles());
}

// ---- Text ----

int WorkbenchBackend::findNext(const QString &text, const QString &query, int from, bool caseSensitive) const
{
    const auto pos = wb::findNext(view(text), view(query), static_cast<std::size_t>(qMax(0, from)), caseSensitive);
    return pos ? static_cast<int>(*pos) : -1;
}

int WorkbenchBackend::countMatches(const QString &text, const QString &query, bool caseSensitive) const
{
    return static_cast<int>(wb::countMatches(view(text), view(query), caseSensitive));
}

int WorkbenchBackend::countLines(const QString &text) const
{
    return static_cast<int>(wb::countLines(view(text)));
}

QString WorkbenchBackend::formatClock(int seconds) const
{
    return fromStd(wb::formatClock(seconds));
}

// ---- Units ----

QStringList WorkbenchBackend::categories() const
{
    return fromStdList(wb::unitCategories());
}

QStringList WorkbenchBackend::unitsOf(const QString &category) const
{
    return fromStdList(wb::unitsOf(toStd(category)));
}

QString WorkbenchBackend::convert(const QString &category, const QString &from, const QString &to,
                                  const QString &value) const
{
    bool ok = false;
    const double number = QLocale::c().toDouble(value.trimmed(), &ok);
    if (!ok)
        return QStringLiteral("-");

    const auto result = wb::convertUnit(toStd(category), toStd(from), toStd(to), number);
    if (!result)
        return QStringLiteral("-");
    return fromStd(wb::formatNumber(*result));
}

// ---- Colors ----

QString WorkbenchBackend::toHex(int r, int g, int b) const
{
    return fromStd(wb::toHex({r, g, b}));
}

QString WorkbenchBackend::toRgbText(int r, int g, int b) const
{
    return fromStd(wb::toRgbText({r, g, b}));
}

QVariantList WorkbenchBackend::parseHex(const QString &text) const
{
    const auto color = wb::parseHex(toStd(text));
    if (!color)
        return {};
    return {color->r, color->g, color->b};
}

// ---- Notes ----

// The folder where the Save dialog starts.
QString WorkbenchBackend::notesFolder() const
{
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

// A file name made from the title, for example "Garden plan" -> "garden-plan.txt".
QString WorkbenchBackend::suggestedNoteName(const QString &title) const
{
    return fromStd(wb::noteFileName(view(title)));
}

bool WorkbenchBackend::saveTextFile(const QString &path, const QString &text)
{
    const auto result = wb::writeTextFile(toPath(path), toStd(text));
    if (!result) {
        setLastError(fromStd(result.error()));
        return false;
    }
    setLastError({});
    return true;
}

// ---- Countdown ----

void WorkbenchBackend::setCountdownMinutes(int minutes)
{
    m_secondTimer.stop();
    m_countdown.setTotal(minutes * 60);
    emit countdownChanged();
}

void WorkbenchBackend::startCountdown()
{
    m_countdown.start();
    if (m_countdown.running())
        m_secondTimer.start();
    emit countdownChanged();
}

void WorkbenchBackend::stopCountdown()
{
    m_countdown.stop();
    m_secondTimer.stop();
    emit countdownChanged();
}

void WorkbenchBackend::resetCountdown()
{
    m_countdown.reset();
    m_secondTimer.stop();
    emit countdownChanged();
}

void WorkbenchBackend::onSecondPassed()
{
    const bool finished = m_countdown.tick(1);
    if (finished)
        m_secondTimer.stop();
    emit countdownChanged();
    if (finished)
        emit countdownFinished();
}

void WorkbenchBackend::setLastError(const QString &text)
{
    if (m_lastError == text)
        return;
    m_lastError = text;
    emit lastErrorChanged();
}
