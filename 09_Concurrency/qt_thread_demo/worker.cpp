#include "worker.h"
#include <QThread>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QDateTime>
#include <cmath>

Worker::Worker(QObject* parent)
    : QObject(parent)
{}

// ── Demo 1: Heavy computation ──────────────────────────────
// Counts prime numbers up to N.
// On the UI thread this would freeze the window.
// On a background thread the UI stays responsive.
void Worker::findPrimes(int upTo) {
    QElapsedTimer timer;
    timer.start();

    int count = 0;
    for (int i = 2; i <= upTo; i++) {
        if (isPrime(i)) count++;

        // Report progress every 10000 numbers
        if (i % 10000 == 0) {
            emit progressUpdated(i, upTo,
                QString("Checking primes... %1/%2").arg(i).arg(upTo));
        }
    }

    emit primesFound(count, timer.elapsed());
    emit finished();
}

bool Worker::isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i <= std::sqrt(n); i += 2)
        if (n % i == 0) return false;
    return true;
}

// ── Demo 2: Simulated file processing ─────────────────────
// Processes N "files" with artificial delay per file.
// Reports progress after each file.
void Worker::processFiles(int fileCount) {
    QElapsedTimer timer;
    timer.start();

    QStringList extensions = {".cpp", ".h", ".json", ".xml", ".csv", ".log"};

    for (int i = 0; i < fileCount; i++) {
        // Simulate processing time (10-50ms per file)
        int delay = 10 + QRandomGenerator::global()->bounded(40);
        QThread::msleep(delay);

        // Generate fake filename and size
        QString ext  = extensions[QRandomGenerator::global()->bounded(extensions.size())];
        QString name = QString("file_%1%2").arg(i + 1).arg(ext);
        int     size = 10 + QRandomGenerator::global()->bounded(990);

        emit fileProcessed(i + 1, name, size);
        emit progressUpdated(i + 1, fileCount,
            QString("Processing %1...").arg(name));
    }

    emit allFilesProcessed(fileCount, timer.elapsed());
    emit finished();
}

// ── Demo 3: Continuous monitoring ─────────────────────────
// Runs in a loop, emitting data at regular intervals.
// Stopped via stopMonitoring() from the UI thread.
// std::atomic<bool> makes the stop flag thread-safe.
void Worker::startMonitoring(int intervalMs) {
    m_stopMonitoring = false;

    while (!m_stopMonitoring) {
        // Simulate sensor readings
        double cpu = 10.0 + QRandomGenerator::global()->bounded(800) / 10.0;
        double mem = 200.0 + QRandomGenerator::global()->bounded(6000) / 10.0;
        qint64 ts  = QDateTime::currentMSecsSinceEpoch();

        emit monitoringData(cpu, mem, ts);

        QThread::msleep(intervalMs);
    }

    emit finished();
}

void Worker::stopMonitoring() {
    // atomic write — safe to call from any thread
    m_stopMonitoring = true;
}
