#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

// ============================================================
// UdpReceiver — listens for incoming datagrams
//
// KEY DIFFERENCE FROM TCP SERVER:
//   - No QTcpServer equivalent needed
//   - Just ONE QUdpSocket bound to a port
//   - readDatagram() gives you the data + sender address
//   - Every datagram is independent — no connection state
//   - Multiple senders can send to the same bound socket
//
// This receiver:
//   - Logs all received packets to stdout
//   - Replies to ping with pong (back to sender's address/port)
//   - Tracks packet sequence numbers to detect lost packets
//   - Shows telemetry data formatted nicely
// ============================================================
class UdpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UdpReceiver(quint16 port, QObject* parent = nullptr);
    bool start();

private slots:
    void onReadyRead();

private:
    void handleData(const QJsonObject& msg, const QHostAddress& sender, quint16 port);
    void handleTelemetry(const QJsonObject& msg, const QHostAddress& sender);
    void handlePing(const QJsonObject& msg, const QHostAddress& sender, quint16 port);
    void log(const QString& msg);

    QUdpSocket* m_socket;
    quint16     m_port;
    int         m_receivedCount;
    int         m_lastSeq;
    int         m_lostCount;
};

#endif // UDPRECEIVER_H
