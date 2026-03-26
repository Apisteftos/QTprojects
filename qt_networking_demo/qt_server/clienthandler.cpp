#include "clienthandler.h"
#include <QHostAddress>

ClientHandler::ClientHandler(QTcpSocket* socket, QObject* parent)
    : QObject(parent), m_socket(socket), m_name("unknown")
{
    // Take ownership of the socket
    m_socket->setParent(this);

    connect(m_socket, &QTcpSocket::readyRead,
            this,     &ClientHandler::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected,
            this,     &ClientHandler::onDisconnected);
}

void ClientHandler::send(const QByteArray& data) {
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->write(data);
}

QString ClientHandler::address() const {
    if (m_socket) return m_socket->peerAddress().toString();
    return {};
}

qintptr ClientHandler::socketId() const {
    if (m_socket) return m_socket->socketDescriptor();
    return -1;
}

void ClientHandler::onReadyRead() {
    // Append all available bytes to our buffer
    m_buffer += m_socket->readAll();

    // Process every complete line (newline = end of one JSON message)
    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QByteArray line = m_buffer.left(idx).trimmed();
        m_buffer = m_buffer.mid(idx + 1);  // keep remainder

        if (!line.isEmpty())
            emit messageReceived(this, line);
    }
}

void ClientHandler::onDisconnected() {
    emit clientDisconnected(this);
}
