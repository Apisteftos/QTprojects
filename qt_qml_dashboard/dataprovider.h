#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QObject>
#include <QTimer>
#include <QVariantList>
#include <QRandomGenerator>

// ============================================================
// DataProvider — C++ ViewModel feeding QML dashboard
//
// Simulates sensor data from a background source.
// All properties exposed via Q_PROPERTY — QML binds
// automatically with no manual connect() needed.
//
// Simulates three sensor streams:
//   - CPU load (%)
//   - Memory usage (MB)
//   - Network throughput (KB/s)
//
// Also exposes:
//   - Alert system (threshold exceeded)
//   - Connection status
//   - Historical data lists for charts
// ============================================================
class DataProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double  cpuLoad      READ cpuLoad      NOTIFY dataChanged)
    Q_PROPERTY(double  memoryMb     READ memoryMb     NOTIFY dataChanged)
    Q_PROPERTY(double  networkKbps  READ networkKbps  NOTIFY dataChanged)
    Q_PROPERTY(int     sampleCount  READ sampleCount  NOTIFY dataChanged)
    Q_PROPERTY(bool    isRunning    READ isRunning    NOTIFY runningChanged)
    Q_PROPERTY(bool    hasAlert     READ hasAlert     NOTIFY alertChanged)
    Q_PROPERTY(QString alertMessage READ alertMessage NOTIFY alertChanged)
    Q_PROPERTY(QString statusText   READ statusText   NOTIFY dataChanged)

    // Chart history — last 30 samples
    Q_PROPERTY(QVariantList cpuHistory     READ cpuHistory     NOTIFY dataChanged)
    Q_PROPERTY(QVariantList memoryHistory  READ memoryHistory  NOTIFY dataChanged)
    Q_PROPERTY(QVariantList networkHistory READ networkHistory NOTIFY dataChanged)

    // Settings
    Q_PROPERTY(int    intervalMs   READ intervalMs   WRITE setIntervalMs   NOTIFY settingsChanged)
    Q_PROPERTY(double cpuThreshold READ cpuThreshold WRITE setCpuThreshold NOTIFY settingsChanged)

public:
    explicit DataProvider(QObject* parent = nullptr);

    // Property getters
    double  cpuLoad()      const { return m_cpuLoad; }
    double  memoryMb()     const { return m_memoryMb; }
    double  networkKbps()  const { return m_networkKbps; }
    int     sampleCount()  const { return m_sampleCount; }
    bool    isRunning()    const { return m_timer->isActive(); }
    bool    hasAlert()     const { return m_hasAlert; }
    QString alertMessage() const { return m_alertMessage; }
    QString statusText()   const;

    QVariantList cpuHistory()     const { return m_cpuHistory; }
    QVariantList memoryHistory()  const { return m_memoryHistory; }
    QVariantList networkHistory() const { return m_networkHistory; }

    int    intervalMs()   const { return m_intervalMs; }
    double cpuThreshold() const { return m_cpuThreshold; }

    // Property setters
    void setIntervalMs(int ms);
    void setCpuThreshold(double t);

    // Q_INVOKABLE — callable from QML
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void dismissAlert();

signals:
    void dataChanged();
    void runningChanged();
    void alertChanged();
    void settingsChanged();

private slots:
    void onTick();

private:
    void addToHistory(QVariantList& list, double value);
    void checkAlerts();

    QTimer* m_timer;

    double  m_cpuLoad     = 0.0;
    double  m_memoryMb    = 0.0;
    double  m_networkKbps = 0.0;
    int     m_sampleCount = 0;
    bool    m_hasAlert    = false;
    QString m_alertMessage;

    QVariantList m_cpuHistory;
    QVariantList m_memoryHistory;
    QVariantList m_networkHistory;

    int    m_intervalMs   = 500;
    double m_cpuThreshold = 80.0;

    static const int HISTORY_SIZE = 30;
};

#endif // DATAPROVIDER_H
