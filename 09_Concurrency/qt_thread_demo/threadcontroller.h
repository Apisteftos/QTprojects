#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include <QThread>
#include "worker.h"

// ============================================================
// ThreadController — MODEL layer
//
// Manages QThread lifecycle and Worker object.
// Demonstrates the correct Worker Object Pattern:
//
//   1. Create Worker (no parent — will be moved to thread)
//   2. Create QThread
//   3. worker->moveToThread(thread)
//   4. Connect thread->started → worker->slot
//   5. Connect worker->finished → thread->quit
//   6. Connect thread->finished → worker->deleteLater
//   7. thread->start()
//
// CRITICAL RULES:
//   - Never give Worker a parent before moveToThread
//   - Never access Worker members directly from UI thread
//     after moveToThread (use signals/slots only)
//   - Always connect finished → deleteLater to avoid leaks
// ============================================================
class ThreadController : public QObject
{
    Q_OBJECT

public:
    explicit ThreadController(QObject* parent = nullptr);
    ~ThreadController();

    bool isRunning() const;

public slots:
    void startPrimeSearch(int upTo);
    void startFileProcessing(int fileCount);
    void startMonitoring(int intervalMs);
    void stopAll();

signals:
    // Forwarded from Worker — UI connects to these
    void primesFound(int count, qint64 elapsedMs);
    void progressUpdated(int current, int total, const QString& message);
    void fileProcessed(int index, const QString& filename, int sizeKb);
    void allFilesProcessed(int total, qint64 elapsedMs);
    void monitoringData(double cpu, double mem, qint64 timestamp);
    void taskFinished();
    void errorOccurred(const QString& error);

    // Internal signals to invoke Worker slots across threads
    // (Qt routes these to the worker's thread automatically)
    void requestPrimeSearch(int upTo);
    void requestFileProcessing(int fileCount);
    void requestMonitoring(int intervalMs);
    void requestStop();

private slots:
    void onWorkerFinished();

private:
    void setupThread();
    void teardownThread();

    QThread* m_thread = nullptr;
    Worker*  m_worker = nullptr;
};

#endif // THREADCONTROLLER_H
