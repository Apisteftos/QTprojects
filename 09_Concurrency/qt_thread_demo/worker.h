#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <atomic>

// ============================================================
// Worker — runs on a background QThread
//
// KEY CONCEPT: Worker Object Pattern (preferred over subclassing)
//
// There are TWO ways to use QThread in Qt:
//
// WAY 1 — Subclass QThread (simple but limited):
//   class MyThread : public QThread {
//       void run() override { /* work here */ }
//   };
//   Problem: can't use signals/slots easily from run()
//
// WAY 2 — Worker Object Pattern (recommended):
//   1. Create a QObject subclass (Worker) with the work logic
//   2. Create a QThread
//   3. Move the Worker to the thread: worker->moveToThread(thread)
//   4. Connect thread->started to worker->doWork
//   5. Worker's slots now run on the background thread
//
//   Benefits:
//   - Full signal/slot support across threads
//   - Clean separation of concerns
//   - Qt handles thread-safe signal delivery automatically
//
// THREE demos in this Worker:
//   1. Heavy computation (prime number search)
//   2. Simulated file processing with progress
//   3. Continuous monitoring loop
// ============================================================
class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject* parent = nullptr);

public slots:
    // These slots run on the BACKGROUND thread
    void findPrimes(int upTo);
    void processFiles(int fileCount);
    void startMonitoring(int intervalMs);
    void stopMonitoring();

signals:
    // Signals are delivered to the UI thread safely by Qt
    void primesFound(int count, qint64 elapsedMs);
    void progressUpdated(int current, int total, const QString& message);
    void fileProcessed(int index, const QString& filename, int sizeKb);
    void allFilesProcessed(int total, qint64 elapsedMs);
    void monitoringData(double cpuLoad, double memoryMb, qint64 timestamp);
    void finished();
    void errorOccurred(const QString& error);

private:
    std::atomic<bool> m_stopMonitoring{false};
    QMutex            m_mutex;

    bool isPrime(int n);
};

#endif // WORKER_H
