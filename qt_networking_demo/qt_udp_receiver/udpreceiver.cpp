#include "udpreceiver.h"
#include "udpprotocol.h"
#include <QJsonObject>
#include <QDateTime>
#include <QTextStream>

UdpReceiver::UdpReceiver(quint16 port, QObject* parent)
    : QObject(parent)
    , m_port(port)
    , m_receivedCount(0)
    , m_lastSeq(-1)
    , m_lostCount(0)
{
    m_socket = new QUdpSocket(this);
    connect(m_socket, &QUdpSocket::readyRead,
            this,     &UdpReceiver::onReadyRead);
}

bool UdpReceiver::start() {
    // bind() — this is what makes us a "receiver"
    // Any datagram sent to this port will fire readyRead
    if (!m_socket->bind(QHostAddress::Any, m_port)) {
        log(QString("Failed to bind to port %1: %2")
            .arg(m_port).arg(m_socket->errorString()));
        return false;
    }
    log(QString("Listening for UDP datagrams on port %1").arg(m_port));
    log("Waiting for packets...\n");
    return true;
}

void UdpReceiver::onReadyRead() {
    // hasPendingDatagrams — multiple datagrams may have arrived
    while (m_socket->hasPendingDatagrams()) {
        QByteArray  datagram;
        QHostAddress senderAddr;
        quint16      senderPort;

        datagram.resize(m_socket->pendingDatagramSize());

        // readDatagram — reads ONE complete datagram
        // Unlike TCP readAll(), each call = exactly one packet
        m_socket->readDatagram(datagram.data(), datagram.size(),
                               &senderAddr, &senderPort);

        QJsonObject msg = UdpProtocol::parse(datagram);
        if (msg.isEmpty()) {
            log(QString("Received non-JSON datagram from %1:%2")
                .arg(senderAddr.toString()).arg(senderPort));
            continue;
        }

        // Detect lost packets via sequence number gaps
        int seq = msg["seq"].toInt(-1);
        if (seq >= 0 && m_lastSeq >= 0) {
            int expected = m_lastSeq + 1;
            if (seq > expected) {
                int lost = seq - expected;
                m_lostCount += lost;
                log(QString("⚠ PACKET LOSS detected: expected seq=%1, got seq=%2 (%3 lost)")
                    .arg(expected).arg(seq).arg(lost));
            }
        }
        if (seq >= 0) m_lastSeq = seq;
        m_receivedCount++;

        QString type = msg["type"].toString();
        if      (type == "data")      handleData(msg, senderAddr, senderPort);
        else if (type == "telemetry") handleTelemetry(msg, senderAddr);
        else if (type == "ping")      handlePing(msg, senderAddr, senderPort);
        else {
            log(QString("Unknown type '%1' from %2")
                .arg(type, senderAddr.toString()));
        }

        // Print running stats every 10 packets
        if (m_receivedCount % 10 == 0) {
            log(QString("── Stats: received=%1  lost=%2  loss rate=%3%")
                .arg(m_receivedCount).arg(m_lostCount)
                .arg(m_receivedCount > 0
                     ? QString::number(100.0 * m_lostCount / m_receivedCount, 'f', 1)
                     : "0.0"));
        }
    }
}

void UdpReceiver::handleData(const QJsonObject& msg,
                              const QHostAddress& sender, quint16 port) {
    Q_UNUSED(port)
    log(QString("[DATA] seq=%1  from=%2  payload=\"%3\"")
        .arg(msg["seq"].toInt())
        .arg(sender.toString())
        .arg(msg["payload"].toString()));
}

void UdpReceiver::handleTelemetry(const QJsonObject& msg,
                                   const QHostAddress& sender) {
    log(QString("[TELEM] seq=%1  from=%2  %3=%4 %5")
        .arg(msg["seq"].toInt())
        .arg(sender.toString())
        .arg(msg["sensor"].toString())
        .arg(msg["value"].toDouble(), 0, 'f', 2)
        .arg(msg["unit"].toString()));
}

void UdpReceiver::handlePing(const QJsonObject& msg,
                              const QHostAddress& sender, quint16 senderPort) {
    int seq = msg["seq"].toInt();
    log(QString("[PING] seq=%1  from=%2:%3 → sending pong")
        .arg(seq).arg(sender.toString()).arg(senderPort));

    // Reply pong directly to sender's address and port
    QByteArray pong = UdpProtocol::makePong(seq);
    m_socket->writeDatagram(pong, sender, senderPort);
}

void UdpReceiver::log(const QString& msg) {
    QTextStream out(stdout);
    out << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
        << "  " << msg << "\n";
    out.flush();
}
