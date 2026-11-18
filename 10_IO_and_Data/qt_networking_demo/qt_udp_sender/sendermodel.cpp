#include "sendermodel.h"
#include "udpprotocol.h"
#include <QJsonObject>
#include <QDateTime>
#include <QRandomGenerator>

SenderModel::SenderModel(QObject* parent)
    : QObject(parent)
    , m_seq(0)
    , m_sentCount(0)
    , m_lostCount(0)
    , m_pingSeq(0)
    , m_pingSentTime(0)
    , m_autoPort(0)
{
    m_socket = new QUdpSocket(this);
    m_timer  = new QTimer(this);

    // ── KEY: bind to receive pong replies ────────────────────
    // Even though we're a "sender", we bind to a port so we
    // can receive responses (pong). QUdpSocket is bidirectional.
    m_socket->bind(QHostAddress::Any, 5001);

    connect(m_socket, &QUdpSocket::readyRead,
            this,     &SenderModel::onReadyRead);
    connect(m_timer,  &QTimer::timeout,
            this,     &SenderModel::onAutoSendTick);
}

void SenderModel::sendData(const QString& host, quint16 port, const QString& payload) {
    QByteArray datagram = UdpProtocol::makeData(m_seq++, payload);
    // writeDatagram — no connection needed, just address + port
    m_socket->writeDatagram(datagram, QHostAddress(host), port);
    m_sentCount++;
    emit dataSent(m_seq - 1, "data");
    emit statsUpdated(m_sentCount, m_lostCount);
}

void SenderModel::sendTelemetry(const QString& host, quint16 port,
                                const QString& sensor, double value, const QString& unit) {
    QByteArray datagram = UdpProtocol::makeTelemetry(m_seq++, sensor, value, unit);
    m_socket->writeDatagram(datagram, QHostAddress(host), port);
    m_sentCount++;
    emit dataSent(m_seq - 1, "telemetry");
    emit statsUpdated(m_sentCount, m_lostCount);
}

void SenderModel::sendPing(const QString& host, quint16 port) {
    m_pingSeq      = m_seq++;
    m_pingSentTime = QDateTime::currentMSecsSinceEpoch();
    QByteArray datagram = UdpProtocol::makePing(m_pingSeq);
    m_socket->writeDatagram(datagram, QHostAddress(host), port);
    m_sentCount++;
    emit dataSent(m_pingSeq, "ping");
    emit statsUpdated(m_sentCount, m_lostCount);
}

void SenderModel::startAutoSend(const QString& host, quint16 port, int intervalMs) {
    m_autoHost = host;
    m_autoPort = port;
    m_timer->start(intervalMs);
}

void SenderModel::stopAutoSend() {
    m_timer->stop();
}

void SenderModel::onAutoSendTick() {
    // Simulate a temperature sensor reading
    double value = 20.0 + QRandomGenerator::global()->bounded(100) / 10.0;
    sendTelemetry(m_autoHost, m_autoPort, "temperature", value, "celsius");
}

void SenderModel::onReadyRead() {
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        QHostAddress senderAddr;
        quint16      senderPort;

        m_socket->readDatagram(datagram.data(), datagram.size(),
                               &senderAddr, &senderPort);

        QJsonObject msg = UdpProtocol::parse(datagram);
        if (msg.isEmpty()) continue;

        QString type = msg["type"].toString();

        if (type == "pong") {
            int seq      = msg["seq"].toInt();
            int latencyMs = static_cast<int>(
                QDateTime::currentMSecsSinceEpoch() - m_pingSentTime);
            emit pongReceived(seq, latencyMs);
        }
    }
}
