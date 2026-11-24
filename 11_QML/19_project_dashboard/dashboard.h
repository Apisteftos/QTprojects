#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QList>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QtQml/qqmlregistration.h>

// The backend of the dashboard. It simulates a machine: a timer produces a new measurement
// again and again. The class knows nothing about the interface; QML binds to its properties
// and reacts to its signal.
class Dashboard : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(double load READ load NOTIFY changed)                 // percent, 0 to 100
    Q_PROPERTY(double temperature READ temperature NOTIFY changed)   // degrees Celsius
    Q_PROPERTY(int requests READ requests NOTIFY changed)            // per second
    Q_PROPERTY(QString level READ level NOTIFY levelChanged)         // "ok", "warning" or "critical"
    Q_PROPERTY(QList<double> history READ history NOTIFY changed)    // the last loads, the newest at the end
    Q_PROPERTY(int capacity READ capacity CONSTANT)                  // how many values the history keeps
    Q_PROPERTY(int samples READ samples NOTIFY changed)              // how many measurements so far
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)

public:
    explicit Dashboard(QObject *parent = nullptr);

    double load() const { return m_load; }
    double temperature() const { return m_temperature; }
    int requests() const { return m_requests; }
    QString level() const { return m_level; }
    QList<double> history() const { return m_history; }
    int capacity() const { return kCapacity; }
    int samples() const { return m_samples; }

    bool running() const { return m_timer.isActive(); }
    void setRunning(bool running);

    int interval() const { return m_timer.interval(); }
    void setInterval(int milliseconds);

    Q_INVOKABLE void advance();   // one measurement now (the timer calls it, and so can a test)
    Q_INVOKABLE void reset();

signals:
    void changed();                 // one signal for all measurements: they change together
    void levelChanged();
    void runningChanged();
    void intervalChanged();

    // An event with data: sent when the level gets WORSE (not when it gets better)
    void alarmRaised(const QString &level, const QString &message);

private:
    static double loadAt(int sample);                  // the simulated measurement, the same every run
    static QString levelFor(double load);
    static int rank(const QString &level);

    static constexpr int kCapacity = 24;

    QTimer m_timer;
    double m_load = 0.0;
    double m_temperature = 0.0;
    int m_requests = 0;
    int m_samples = 0;
    QString m_level = QStringLiteral("ok");
    QList<double> m_history;
};

#endif // DASHBOARD_H
