#ifndef SENDERVIEW_H
#define SENDERVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>

class SenderView : public QWidget
{
    Q_OBJECT

public:
    explicit SenderView(QWidget* parent = nullptr);

    void appendLog(const QString& msg);
    void setAutoSending(bool active);
    void updateStats(int sent, int lost);

signals:
    void sendDataRequested(const QString& host, quint16 port, const QString& payload);
    void sendTelemetryRequested(const QString& host, quint16 port,
                                const QString& sensor, double value, const QString& unit);
    void sendPingRequested(const QString& host, quint16 port);
    void startAutoRequested(const QString& host, quint16 port, int intervalMs);
    void stopAutoRequested();

private slots:
    void onSendDataClicked();
    void onSendTelemetryClicked();
    void onPingClicked();
    void onAutoToggleClicked();

private:
    // Target
    QLineEdit*      m_hostInput;
    QSpinBox*       m_portInput;

    // Manual data
    QLineEdit*      m_payloadInput;
    QPushButton*    m_btnSendData;

    // Telemetry
    QComboBox*      m_sensorCombo;
    QDoubleSpinBox* m_valueInput;
    QComboBox*      m_unitCombo;
    QPushButton*    m_btnSendTelemetry;

    // Auto send
    QSpinBox*       m_intervalInput;
    QPushButton*    m_btnAutoToggle;

    // Ping
    QPushButton*    m_btnPing;

    // Stats + log
    QLabel*         m_statsLabel;
    QTextEdit*      m_log;

    bool            m_autoActive;
};

#endif // SENDERVIEW_H
