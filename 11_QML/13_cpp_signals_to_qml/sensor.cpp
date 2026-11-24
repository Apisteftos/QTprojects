#include "sensor.h"

#include <QtMath>

Sensor::Sensor(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(300);
    // The timer's signal calls our member function. The signal-slot connection is plain Qt Core.
    connect(&m_timer, &QTimer::timeout, this, &Sensor::advance);
}

void Sensor::setInterval(int milliseconds)
{
    milliseconds = qMax(20, milliseconds);
    if (milliseconds == m_timer.interval())
        return;
    m_timer.setInterval(milliseconds);
    emit intervalChanged();
}

void Sensor::setThreshold(double threshold)
{
    if (qFuzzyCompare(threshold, m_threshold))
        return;
    m_threshold = threshold;
    emit thresholdChanged();
}

void Sensor::start()
{
    if (m_timer.isActive())
        return;
    m_timer.start();
    emit runningChanged();
}

void Sensor::stop()
{
    if (!m_timer.isActive())
        return;
    m_timer.stop();
    emit runningChanged();
}

double Sensor::readingAt(int number)
{
    // A smooth wave between 5 and 35, rounded to one decimal place
    return qRound((20.0 + 15.0 * qSin(number * 0.4)) * 10.0) / 10.0;
}

void Sensor::advance()
{
    const double previous = m_value;
    ++m_readings;
    m_value = readingAt(m_readings);
    emit valueChanged();

    // The alarm is an event: it is sent once, when the value goes ABOVE the threshold
    if (previous <= m_threshold && m_value > m_threshold)
        emit alarm(m_value, m_readings);
}
