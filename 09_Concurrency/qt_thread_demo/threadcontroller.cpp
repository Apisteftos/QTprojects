#include "threadcontroller.h"

ThreadController::ThreadController(QObject* parent)
    : QObject(parent)
{
    setupThread();
}

ThreadController::~ThreadController() {
    teardownThread();
}

void ThreadController::setupThread() {
    // ── Worker Object Pattern ──────────────────────────────
    // Step 1: Create worker WITHOUT a parent
    //         (parent must be set after moveToThread, or not at all)
    m_worker = new Worker();

    // Step 2: Create the thread
    m_thread = new QThread(this);

    // Step 3: Move worker to the thread
    //         After this, all Worker slots run on m_thread
    m_worker->moveToThread(m_thread);

    // Step 4: Wire Worker signals → Controller signals (forwarding)
    connect(m_worker, &Worker::primesFound,
            this,     &ThreadController::primesFound);
    connect(m_worker, &Worker::progressUpdated,
            this,     &ThreadController::progressUpdated);
    connect(m_worker, &Worker::fileProcessed,
            this,     &ThreadController::fileProcessed);
    connect(m_worker, &Worker::allFilesProcessed,
            this,     &ThreadController::allFilesProcessed);
    connect(m_worker, &Worker::monitoringData,
            this,     &ThreadController::monitoringData);
    connect(m_worker, &Worker::errorOccurred,
            this,     &ThreadController::errorOccurred);
    connect(m_worker, &Worker::finished,
            this,     &ThreadController::onWorkerFinished);

    // Step 5: Wire internal request signals → Worker slots
    //         Qt delivers these on m_thread automatically
    connect(this,     &ThreadController::requestPrimeSearch,
            m_worker, &Worker::findPrimes);
    connect(this,     &ThreadController::requestFileProcessing,
            m_worker, &Worker::processFiles);
    connect(this,     &ThreadController::requestMonitoring,
            m_worker, &Worker::startMonitoring);
    connect(this,     &ThreadController::requestStop,
            m_worker, &Worker::stopMonitoring);

    // Step 6: Clean shutdown chain
    //         thread finished → delete worker safely
    connect(m_thread, &QThread::finished,
            m_worker, &QObject::deleteLater);

    // Step 7: Start the thread (event loop begins)
    m_thread->start();
}

void ThreadController::teardownThread() {
    if (m_thread && m_thread->isRunning()) {
        emit requestStop();          // ask worker to stop loops
        m_thread->quit();            // stop the event loop
        m_thread->wait(3000);        // wait max 3 seconds
    }
}

bool ThreadController::isRunning() const {
    return m_thread && m_thread->isRunning();
}

void ThreadController::startPrimeSearch(int upTo) {
    // Emit signal — Qt delivers it to Worker on m_thread
    emit requestPrimeSearch(upTo);
}

void ThreadController::startFileProcessing(int fileCount) {
    emit requestFileProcessing(fileCount);
}

void ThreadController::startMonitoring(int intervalMs) {
    emit requestMonitoring(intervalMs);
}

void ThreadController::stopAll() {
    emit requestStop();
}

void ThreadController::onWorkerFinished() {
    emit taskFinished();
}
