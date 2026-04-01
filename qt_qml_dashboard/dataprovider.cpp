#include "dataprovider.h"
#include <QtMath>
#include <QDateTime>

DataProvider::DataProvider(QObject* parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(m_intervalMs);
    connect(m_timer, &QTimer::timeout, this, &DataProvider::onTick);
}

void DataProvider::onTick() {
    m_sampleCount++;

    // Simulate realistic CPU load with occasional spikes
    double base = 30.0 + 20.0 * qSin(m_sampleCount * 0.1);
    double spike = (m_sampleCount % 20 == 0) ? 40.0 : 0.0;
    double noise = QRandomGenerator::global()->bounded(100) / 10.0 - 5.0;
    m_cpuLoad = qBound(0.0, base + spike + noise, 100.0);

    // Memory slowly grows then drops (GC simulation)
    m_memoryMb = 200.0 + 300.0 * (1.0 - qExp(-m_sampleCount * 0.02))
                 + QRandomGenerator::global()->bounded(50);
    if (m_sampleCount % 30 == 0) m_memoryMb -= 150; // GC event
    m_memoryMb = qBound(100.0, m_memoryMb, 800.0);

    // Network throughput — bursty pattern
    bool burst = (m_sampleCount % 15 < 5);
    m_networkKbps = burst
        ? 500.0 + QRandomGenerator::global()->bounded(500)
        : 50.0  + QRandomGenerator::global()->bounded(100);

    // Update history lists
    addToHistory(m_cpuHistory,     m_cpuLoad);
    addToHistory(m_memoryHistory,  m_memoryMb);
    addToHistory(m_networkHistory, m_networkKbps);

    checkAlerts();
    emit dataChanged();
}

void DataProvider::addToHistory(QVariantList& list, double value) {
    list.append(value);
    while (list.size() > HISTORY_SIZE)
        list.removeFirst();
}

void DataProvider::checkAlerts() {
    if (m_cpuLoad > m_cpuThreshold && !m_hasAlert) {
        m_hasAlert     = true;
        m_alertMessage = QString("CPU load exceeded %1% threshold! Current: %2%")
            .arg(m_cpuThreshold, 0, 'f', 0)
            .arg(m_cpuLoad,      0, 'f', 1);
        emit alertChanged();
    } else if (m_cpuLoad <= m_cpuThreshold && m_hasAlert) {
        m_hasAlert     = false;
        m_alertMessage = "";
        emit alertChanged();
    }
}

QString DataProvider::statusText() const {
    if (!m_timer->isActive()) return "Stopped";
    return QString("Running — %1 samples @ %2ms")
        .arg(m_sampleCount).arg(m_intervalMs);
}

void DataProvider::start() {
    m_timer->start();
    emit runningChanged();
    emit dataChanged();
}

void DataProvider::stop() {
    m_timer->stop();
    emit runningChanged();
    emit dataChanged();
}

void DataProvider::reset() {
    m_timer->stop();
    m_sampleCount = 0;
    m_cpuLoad     = 0.0;
    m_memoryMb    = 0.0;
    m_networkKbps = 0.0;
    m_hasAlert    = false;
    m_alertMessage = "";
    m_cpuHistory.clear();
    m_memoryHistory.clear();
    m_networkHistory.clear();
    emit runningChanged();
    emit dataChanged();
    emit alertChanged();
}

void DataProvider::dismissAlert() {
    m_hasAlert     = false;
    m_alertMessage = "";
    emit alertChanged();
}

void DataProvider::setIntervalMs(int ms) {
    m_intervalMs = qBound(100, ms, 5000);
    m_timer->setInterval(m_intervalMs);
    emit settingsChanged();
    emit dataChanged();
}

void DataProvider::setCpuThreshold(double t) {
    m_cpuThreshold = qBound(10.0, t, 100.0);
    emit settingsChanged();
}
