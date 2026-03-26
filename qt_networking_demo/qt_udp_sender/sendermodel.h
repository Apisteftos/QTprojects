#ifndef SENDERMODEL_H
#define SENDERMODEL_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QString>
#include <QHostAddress>

// ============================================================
// SenderModel — MODEL layer
//
// Owns the QUdpSocket on the SENDER side.
//
// KEY DIFFERENCE FROM TCP:
//   - No connectToHost() needed — UDP is connectionless
//   - writeDatagram() sends directly to an address + port
//   - Can also RECEIVE on the same socket (QUdpSocket is bidirectional)
//   - bind() is optional for sending, required for receiving
//
// This model supports:
//   - Manual single message send
//   - Auto-send mode (QTimer fires every N ms — simulates telemetry)
//   - Ping/pong round-trip latency measurement
// ============================================================
class SenderModel : public QObject
{
    Q_OBJECT

public:
    explicit SenderModel(QObject* parent = nullptr);

    bool isAutoSending() const { return m_timer->isActive(); }
    int  sentCount()     const { return m_sentCount; }
    int  lostCount()     const { return m_lostCount; }

public slots:
    void sendData(const QString& host, quint16 port, const QString& payload);
    void sendTelemetry(const QString& host, quint16 port,
                       const QString& sensor, double value, const QString& unit);
    void sendPing(const QString& host, quint16 port);
    void startAutoSend(const QString& host, quint16 port, int intervalMs);
    void stopAutoSend();

signals:
    void dataSent(int seq, const QString& type);
    void pongReceived(int seq, int latencyMs);
    void errorOccurred(const QString& error);
    void statsUpdated(int sent, int lost);

private slots:
    void onAutoSendTick();
    void onReadyRead();

private:
    QUdpSocket* m_socket;
    QTimer*     m_timer;
    int         m_seq;
    int         m_sentCount;
    int         m_lostCount;
    int         m_pingSeq;
    qint64      m_pingSentTime;
    QString     m_autoHost;
    quint16     m_autoPort;
};

#endif // SENDERMODEL_H
