#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDateTime>
#include <QValueAxis>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt Threading Demo — QThread + Worker Pattern");
    setMinimumSize(700, 550);

    m_controller = new ThreadController(this);

    // ── Connect controller signals ─────────────────────────
    connect(m_controller, &ThreadController::primesFound,
            this,         &MainWindow::onPrimesFound);
    connect(m_controller, &ThreadController::progressUpdated,
            this,         &MainWindow::onProgressUpdated);
    connect(m_controller, &ThreadController::fileProcessed,
            this,         &MainWindow::onFileProcessed);
    connect(m_controller, &ThreadController::allFilesProcessed,
            this,         &MainWindow::onAllFilesProcessed);
    connect(m_controller, &ThreadController::monitoringData,
            this,         &MainWindow::onMonitoringData);
    connect(m_controller, &ThreadController::taskFinished,
            this,         &MainWindow::onTaskFinished);

    // ── Central widget with tabs ───────────────────────────
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QTabWidget* tabs = new QTabWidget(this);

    QWidget* primesTab  = new QWidget();
    QWidget* filesTab   = new QWidget();
    QWidget* monitorTab = new QWidget();

    setupPrimesTab(primesTab);
    setupFilesTab(filesTab);
    setupMonitorTab(monitorTab);

    tabs->addTab(primesTab,  "Demo 1 — Prime Search");
    tabs->addTab(filesTab,   "Demo 2 — File Processing");
    tabs->addTab(monitorTab, "Demo 3 — Live Monitor");

    mainLayout->addWidget(tabs);

    // ── Shared log ─────────────────────────────────────────
    QGroupBox* logGroup = new QGroupBox("Activity Log", this);
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    m_mainLog = new QTextEdit(this);
    m_mainLog->setReadOnly(true);
    m_mainLog->setMaximumHeight(120);
    m_mainLog->setStyleSheet("font-family: monospace; font-size: 11px;");
    logLayout->addWidget(m_mainLog);
    mainLayout->addWidget(logGroup);

    setCentralWidget(central);
    log("Application started. Thread pool ready.");
}

// ── Demo 1 setup ───────────────────────────────────────────
void MainWindow::setupPrimesTab(QWidget* tab) {
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(10);
    layout->setContentsMargins(12, 12, 12, 12);

    QLabel* desc = new QLabel(
        "<b>Heavy computation on background thread.</b><br>"
        "Counts prime numbers up to N. The UI stays fully "
        "responsive while the background thread works.", tab);
    desc->setWordWrap(true);
    layout->addWidget(desc);

    QHBoxLayout* row = new QHBoxLayout();
    row->addWidget(new QLabel("Count primes up to:", tab));
    m_primesLimit = new QSpinBox(tab);
    m_primesLimit->setRange(1000, 5000000);
    m_primesLimit->setValue(500000);
    m_primesLimit->setSingleStep(100000);
    row->addWidget(m_primesLimit);
    m_btnPrimes = new QPushButton("Start Search", tab);
    row->addWidget(m_btnPrimes);
    layout->addLayout(row);

    m_primesProgress = new QProgressBar(tab);
    m_primesProgress->setRange(0, 100);
    layout->addWidget(m_primesProgress);

    m_primesResult = new QLabel("Result will appear here...", tab);
    m_primesResult->setStyleSheet("font-size: 14px; color: #2266cc;");
    m_primesResult->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_primesResult);

    layout->addStretch();

    connect(m_btnPrimes, &QPushButton::clicked, this, &MainWindow::onStartPrimes);
}

// ── Demo 2 setup ───────────────────────────────────────────
void MainWindow::setupFilesTab(QWidget* tab) {
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(10);
    layout->setContentsMargins(12, 12, 12, 12);

    QLabel* desc = new QLabel(
        "<b>Long-running task with progress reporting.</b><br>"
        "Simulates processing files one by one. "
        "Each file emits a signal back to the UI thread.", tab);
    desc->setWordWrap(true);
    layout->addWidget(desc);

    QHBoxLayout* row = new QHBoxLayout();
    row->addWidget(new QLabel("Number of files:", tab));
    m_fileCount = new QSpinBox(tab);
    m_fileCount->setRange(5, 200);
    m_fileCount->setValue(20);
    row->addWidget(m_fileCount);
    m_btnFiles = new QPushButton("Process Files", tab);
    row->addWidget(m_btnFiles);
    layout->addLayout(row);

    m_filesProgress = new QProgressBar(tab);
    layout->addWidget(m_filesProgress);

    m_filesLog = new QTextEdit(tab);
    m_filesLog->setReadOnly(true);
    m_filesLog->setStyleSheet("font-family: monospace; font-size: 11px;");
    layout->addWidget(m_filesLog);

    connect(m_btnFiles, &QPushButton::clicked, this, &MainWindow::onStartFiles);
}

// ── Demo 3 setup ───────────────────────────────────────────
void MainWindow::setupMonitorTab(QWidget* tab) {
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(10);
    layout->setContentsMargins(12, 12, 12, 12);

    QLabel* desc = new QLabel(
        "<b>Continuous background monitoring loop.</b><br>"
        "Worker runs indefinitely, stopped via atomic flag. "
        "Simulates CPU/memory sensor readings.", tab);
    desc->setWordWrap(true);
    layout->addWidget(desc);

    QHBoxLayout* statsRow = new QHBoxLayout();
    m_cpuLabel = new QLabel("CPU: ---%", tab);
    m_cpuLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2266cc;");
    m_memLabel = new QLabel("MEM: --- MB", tab);
    m_memLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #cc6622;");
    statsRow->addWidget(m_cpuLabel);
    statsRow->addWidget(m_memLabel);
    layout->addLayout(statsRow);

    // Live chart
    m_cpuSeries = new QLineSeries();
    m_cpuSeries->setName("CPU %");

    QChart* chart = new QChart();
    chart->addSeries(m_cpuSeries);
    chart->setTitle("CPU Load — Live");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 30);
    chart->axes(Qt::Vertical).first()->setRange(0, 100);
    chart->legend()->hide();

    m_chartView = new QChartView(chart, tab);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(m_chartView);

    m_btnMonitor = new QPushButton("Start Monitoring", tab);
    layout->addWidget(m_btnMonitor);

    connect(m_btnMonitor, &QPushButton::clicked,
            this,         &MainWindow::onToggleMonitoring);
}

// ── Slots ──────────────────────────────────────────────────
void MainWindow::onStartPrimes() {
    m_btnPrimes->setEnabled(false);
    m_primesProgress->setValue(0);
    m_primesResult->setText("Searching...");
    log(QString("Starting prime search up to %1").arg(m_primesLimit->value()));
    m_controller->startPrimeSearch(m_primesLimit->value());
}

void MainWindow::onStartFiles() {
    m_btnFiles->setEnabled(false);
    m_filesProgress->setValue(0);
    m_filesLog->clear();
    log(QString("Starting processing of %1 files").arg(m_fileCount->value()));
    m_controller->startFileProcessing(m_fileCount->value());
}

void MainWindow::onToggleMonitoring() {
    if (!m_monitoring) {
        m_monitoring = true;
        m_sampleCount = 0;
        m_cpuSeries->clear();
        m_btnMonitor->setText("Stop Monitoring");
        log("Monitoring started — background thread running");
        m_controller->startMonitoring(500);
    } else {
        m_monitoring = false;
        m_btnMonitor->setText("Start Monitoring");
        log("Monitoring stop requested...");
        m_controller->stopAll();
    }
}

void MainWindow::onPrimesFound(int count, qint64 elapsedMs) {
    m_primesResult->setText(
        QString("Found %1 primes in %2 ms").arg(count).arg(elapsedMs));
    m_primesProgress->setValue(100);
}

void MainWindow::onProgressUpdated(int current, int total, const QString& message) {
    int pct = total > 0 ? (current * 100 / total) : 0;
    m_primesProgress->setValue(pct);
    m_filesProgress->setValue(pct);
    Q_UNUSED(message)
}

void MainWindow::onFileProcessed(int index, const QString& filename, int sizeKb) {
    m_filesLog->append(QString("[%1] %2 — %3 KB")
        .arg(index, 3).arg(filename, -20).arg(sizeKb));
}

void MainWindow::onAllFilesProcessed(int total, qint64 elapsedMs) {
    m_filesLog->append(QString("\n✓ Done: %1 files in %2 ms").arg(total).arg(elapsedMs));
    log(QString("File processing complete: %1 files, %2 ms").arg(total).arg(elapsedMs));
}

void MainWindow::onMonitoringData(double cpu, double mem, qint64 timestamp) {
    Q_UNUSED(timestamp)
    m_cpuLabel->setText(QString("CPU: %1%").arg(cpu, 0, 'f', 1));
    m_memLabel->setText(QString("MEM: %1 MB").arg(mem, 0, 'f', 0));

    // Update live chart — keep last 30 samples
    m_cpuSeries->append(m_sampleCount++, cpu);
    if (m_cpuSeries->count() > 30) {
        m_cpuSeries->removePoints(0, 1);
        m_chartView->chart()->axes(Qt::Horizontal).first()
            ->setRange(m_sampleCount - 30, m_sampleCount);
    }
}

void MainWindow::onTaskFinished() {
    m_btnPrimes->setEnabled(true);
    m_btnFiles->setEnabled(true);
    log("Task finished.");
}

void MainWindow::log(const QString& msg) {
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_mainLog->append(QString("[%1] %2").arg(time, msg));
}
