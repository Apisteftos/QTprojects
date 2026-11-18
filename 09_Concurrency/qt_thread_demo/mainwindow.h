#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QTabWidget>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include "threadcontroller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    // Demo 1
    void onStartPrimes();

    // Demo 2
    void onStartFiles();

    // Demo 3
    void onToggleMonitoring();

    // From controller
    void onPrimesFound(int count, qint64 elapsedMs);
    void onProgressUpdated(int current, int total, const QString& message);
    void onFileProcessed(int index, const QString& filename, int sizeKb);
    void onAllFilesProcessed(int total, qint64 elapsedMs);
    void onMonitoringData(double cpu, double mem, qint64 timestamp);
    void onTaskFinished();

private:
    void setupPrimesTab(QWidget* tab);
    void setupFilesTab(QWidget* tab);
    void setupMonitorTab(QWidget* tab);
    void log(const QString& msg);

    ThreadController* m_controller;

    // Demo 1 — primes
    QSpinBox*     m_primesLimit;
    QPushButton*  m_btnPrimes;
    QProgressBar* m_primesProgress;
    QLabel*       m_primesResult;

    // Demo 2 — files
    QSpinBox*     m_fileCount;
    QPushButton*  m_btnFiles;
    QProgressBar* m_filesProgress;
    QTextEdit*    m_filesLog;

    // Demo 3 — monitoring
    QPushButton*  m_btnMonitor;
    QLabel*       m_cpuLabel;
    QLabel*       m_memLabel;
    QLineSeries*  m_cpuSeries;
    QChartView*   m_chartView;
    int           m_sampleCount = 0;
    bool          m_monitoring  = false;

    // Shared log
    QTextEdit*    m_mainLog;
};

#endif // MAINWINDOW_H
