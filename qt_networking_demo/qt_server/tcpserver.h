#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include "clienthandler.h"

// ============================================================
// TcpServer — the background service
//
// Responsibilities:
//   - Listens on a port for incoming connections
//   - Creates a ClientHandler for each connected client
//   - Routes messages between clients (broadcast)
//   - Logs all activity to stdout (no UI — this is a daemon)
//
// This runs on QCoreApplication — no widgets, no GUI.
// The Qt event loop handles all I/O asynchronously.
// ============================================================
class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(quint16 port, QObject* parent = nullptr);

    bool start();
    int  clientCount() const { return m_clients.size(); }

private slots:
    void onNewConnection();
    void onMessageReceived(ClientHandler* client, const QByteArray& raw);
    void onClientDisconnected(ClientHandler* client);

private:
    void broadcast(const QByteArray& data, ClientHandler* exclude = nullptr);
    void log(const QString& msg);

    QTcpServer*         m_server;
    QList<ClientHandler*> m_clients;
    quint16             m_port;
};

#endif // TCPSERVER_H
