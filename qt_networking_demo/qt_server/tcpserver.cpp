#include "tcpserver.h"
#include "protocol.h"
#include <QJsonObject>
#include <QDateTime>
#include <QTextStream>

TcpServer::TcpServer(quint16 port, QObject* parent)
    : QObject(parent), m_port(port)
{
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection,
            this,     &TcpServer::onNewConnection);
}

bool TcpServer::start() {
    if (!m_server->listen(QHostAddress::Any, m_port)) {
        log(QString("Failed to start: %1").arg(m_server->errorString()));
        return false;
    }
    log(QString("Server listening on port %1").arg(m_port));
    log("Waiting for connections...");
    return true;
}

void TcpServer::onNewConnection() {
    while (m_server->hasPendingConnections()) {
        QTcpSocket* socket = m_server->nextPendingConnection();

        // Wrap socket in a ClientHandler
        ClientHandler* client = new ClientHandler(socket, this);

        connect(client, &ClientHandler::messageReceived,
                this,   &TcpServer::onMessageReceived);
        connect(client, &ClientHandler::clientDisconnected,
                this,   &TcpServer::onClientDisconnected);

        m_clients.append(client);
        log(QString("[+] New connection from %1  (total: %2)")
            .arg(client->address()).arg(m_clients.size()));

        // Tell everyone someone joined
        broadcast(Protocol::makeBroadcast("server",
            QString("A new client connected from %1").arg(client->address())));
    }
}

void TcpServer::onMessageReceived(ClientHandler* client, const QByteArray& raw) {
    QJsonObject msg = Protocol::parse(raw);
    if (msg.isEmpty()) return;

    QString type = msg["type"].toString();

    if (type == "connect") {
        // Client announces its name
        QString name = msg["name"].toString("anonymous");
        client->setName(name);
        log(QString("[~] %1 identified as '%2'").arg(client->address(), name));

        client->send(Protocol::makeAck("welcome"));
        broadcast(Protocol::makeBroadcast("server",
            QString("%1 joined the chat").arg(name)), client);

    } else if (type == "message") {
        // Relay message to all other clients
        QString text = msg["text"].toString();
        log(QString("[>] %1: %2").arg(client->name(), text));
        broadcast(Protocol::makeBroadcast(client->name(), text), client);
        client->send(Protocol::makeAck());

    } else if (type == "ping") {
        client->send(Protocol::makePong());

    } else {
        log(QString("[?] Unknown message type '%1' from %2").arg(type, client->name()));
    }
}

void TcpServer::onClientDisconnected(ClientHandler* client) {
    log(QString("[-] %1 ('%2') disconnected  (remaining: %3)")
        .arg(client->address(), client->name())
        .arg(m_clients.size() - 1));

    broadcast(Protocol::makeBroadcast("server",
        QString("%1 left the chat").arg(client->name())), client);

    m_clients.removeOne(client);
    client->deleteLater();
}

void TcpServer::broadcast(const QByteArray& data, ClientHandler* exclude) {
    for (ClientHandler* c : m_clients) {
        if (c != exclude) c->send(data);
    }
}

void TcpServer::log(const QString& msg) {
    QTextStream out(stdout);
    out << QDateTime::currentDateTime().toString("hh:mm:ss")
        << "  " << msg << "\n";
    out.flush();
}
