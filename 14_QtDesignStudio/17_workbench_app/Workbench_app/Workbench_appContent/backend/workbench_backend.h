#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QVariantList>

#include "countdown.hpp"
#include "recent_files.hpp"

// The bridge between QML and the core library.
// This class only converts types (QString <-> std::string, QUrl <-> path) and
// sends change signals. The decisions are made in the core library.
class WorkbenchBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList recentFiles READ recentFiles NOTIFY recentFilesChanged FINAL)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged FINAL)
    Q_PROPERTY(QString countdownText READ countdownText NOTIFY countdownChanged FINAL)
    Q_PROPERTY(double countdownProgress READ countdownProgress NOTIFY countdownChanged FINAL)
    Q_PROPERTY(bool countdownRunning READ countdownRunning NOTIFY countdownChanged FINAL)

public:
    explicit WorkbenchBackend(QObject *parent = nullptr);

    QStringList recentFiles() const;
    QString lastError() const { return m_lastError; }
    QString countdownText() const;
    double countdownProgress() const { return m_countdown.progress(); }
    bool countdownRunning() const { return m_countdown.running(); }

    // Files
    Q_INVOKABLE QUrl fileUrl(const QString &path) const;
    Q_INVOKABLE QString localPath(const QUrl &url) const;
    Q_INVOKABLE QString fileName(const QString &path) const;
    Q_INVOKABLE QString fileKind(const QString &path) const;
    Q_INVOKABLE QString readTextFile(const QUrl &url);
    Q_INVOKABLE void addRecent(const QString &path);
    Q_INVOKABLE void clearRecent();

    // Text
    Q_INVOKABLE int findNext(const QString &text, const QString &query, int from, bool caseSensitive) const;
    Q_INVOKABLE int countMatches(const QString &text, const QString &query, bool caseSensitive) const;
    Q_INVOKABLE int countLines(const QString &text) const;
    Q_INVOKABLE QString formatClock(int seconds) const;

    // Units
    Q_INVOKABLE QStringList categories() const;
    Q_INVOKABLE QStringList unitsOf(const QString &category) const;
    Q_INVOKABLE QString convert(const QString &category, const QString &from, const QString &to,
                                const QString &value) const;

    // Colors
    Q_INVOKABLE QString toHex(int r, int g, int b) const;
    Q_INVOKABLE QString toRgbText(int r, int g, int b) const;
    Q_INVOKABLE QVariantList parseHex(const QString &text) const; // [r, g, b] or an empty list

    // Notes
    Q_INVOKABLE QString suggestedNoteName(const QString &title) const;
    Q_INVOKABLE bool saveTextFile(const QString &path, const QString &text);
    Q_INVOKABLE QString notesFolder() const;

    // Countdown (the timer tab)
    Q_INVOKABLE void setCountdownMinutes(int minutes);
    Q_INVOKABLE void startCountdown();
    Q_INVOKABLE void stopCountdown();
    Q_INVOKABLE void resetCountdown();

signals:
    void recentFilesChanged();
    void lastErrorChanged();
    void countdownChanged();
    void countdownFinished();

private:
    void setLastError(const QString &text);
    void saveRecent() const;
    void onSecondPassed();

    wb::RecentFiles m_recent{5};
    wb::Countdown m_countdown;
    QTimer m_secondTimer;
    QString m_lastError;
};
