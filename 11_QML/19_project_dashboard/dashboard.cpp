#include "dashboard.h"

#include <QtMath>

Dashboard::Dashboard(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(500);
    connect(&m_timer, &QTimer::timeout, this, &Dashboard::advance);
}

void Dashboard::setRunning(bool running)
{
    if (running == m_timer.isActive())
        return;
    if (running)
        m_timer.start();
    else
        m_timer.stop();
    emit runningChanged();
}

void Dashboard::setInterval(int milliseconds)
{
    milliseconds = qBound(100, milliseconds, 2000);
    if (milliseconds == m_timer.interval())
        return;
    m_timer.setInterval(milliseconds);
    emit intervalChanged();
}

double Dashboard::loadAt(int sample)
{
    // Two waves on top of each other, limited to 0 to 100, rounded to one decimal place
    const double raw = 55.0 + 35.0 * qSin(sample * 0.35) + 10.0 * qSin(sample * 1.1);
    return qRound(qBound(0.0, raw, 100.0) * 10.0) / 10.0;
}

QString Dashboard::levelFor(double load)
{
    if (load >= 85.0)
        return QStringLiteral("critical");
    if (load >= 60.0)
        return QStringLiteral("warning");
    return QStringLiteral("ok");
}

int Dashboard::rank(const QString &level)
{
    if (level == QLatin1String("critical"))
        return 2;
    if (level == QLatin1String("warning"))
        return 1;
    return 0;
}

void Dashboard::advance()
{
    ++m_samples;
    m_load = loadAt(m_samples);
    m_temperature = qRound((38.0 + 0.32 * m_load) * 10.0) / 10.0;
    m_requests = qRound(m_load * 12.5);

    m_history.append(m_load);
    if (m_history.size() > kCapacity)
        m_history.removeFirst();

    const QString previous = m_level;
    m_level = levelFor(m_load);

    // All values are set first, and only then the signals are sent: a binding never sees half of a measurement.
    emit changed();
    if (m_level != previous) {
        emit levelChanged();
        if (rank(m_level) > rank(previous))
            emit alarmRaised(m_level, tr("Load is %1 percent: level %2").arg(m_load, 0, 'f', 1).arg(m_level));
    }
}

void Dashboard::reset()
{
    m_samples = 0;
    m_load = 0.0;
    m_temperature = 0.0;
    m_requests = 0;
    m_history.clear();
    const QString previous = m_level;
    m_level = QStringLiteral("ok");
    emit changed();
    if (previous != m_level)
        emit levelChanged();
}
