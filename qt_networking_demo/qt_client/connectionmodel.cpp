#include "connectionmodel.h"
#include "protocol.h"
#include <QJsonObject>

ConnectionModel::ConnectionModel(QObject* parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected,
            this,     &ConnectionModel::onConnected);
    connect(m_socket, &QTcpSocket::disconnected,
            this,     &ConnectionModel::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead,
            this,     &ConnectionModel::onReadyRead);
    connect(m_socket, &QAbstractSocket::errorOccurred,
            this,     &ConnectionModel::onError);
}

bool ConnectionModel::isConnected() const {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void ConnectionModel::connectToServer(const QString& host, quint16 port, const QString& name) {
    m_name = name;
    m_socket->connectToHost(host, port);
}

void ConnectionModel::disconnectFromServer() {
    m_socket->disconnectFromHost();
}

void ConnectionModel::sendMessage(const QString& text) {
    if (!isConnected()) return;
    m_socket->write(Protocol::makeMessage(m_name, text));
}

void ConnectionModel::sendPing() {
    if (!isConnected()) return;
    m_socket->write(Protocol::makePing());
}

void ConnectionModel::onConnected() {
    // Send connect message with our name
    m_socket->write(Protocol::makeConnect(m_name));
    emit connected();
}

void ConnectionModel::onDisconnected() {
    emit disconnected();
}

void ConnectionModel::onReadyRead() {
    m_buffer += m_socket->readAll();

    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QByteArray line = m_buffer.left(idx).trimmed();
        m_buffer = m_buffer.mid(idx + 1);

        if (line.isEmpty()) continue;

        QJsonObject msg = Protocol::parse(line);
        if (msg.isEmpty()) continue;

        QString type = msg["type"].toString();

        if (type == "broadcast") {
            emit messageReceived(
                msg["from"].toString(),
                msg["text"].toString()
            );
        } else if (type == "ack") {
            emit ackReceived(msg["status"].toString());
        } else if (type == "pong") {
            emit pongReceived();
        }
    }
}

void ConnectionModel::onError(QAbstractSocket::SocketError) {
    emit errorOccurred(m_socket->errorString());
}
