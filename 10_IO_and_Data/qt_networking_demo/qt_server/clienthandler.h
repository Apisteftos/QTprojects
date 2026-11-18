#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

// ============================================================
// ClientHandler — owns one QTcpSocket (one connected client)
//
// Responsibilities:
//   - Reads incoming data from the socket
//   - Buffers partial messages (TCP may split JSON across packets)
//   - Emits messageReceived when a complete JSON line arrives
//   - Emits disconnected when client drops
//   - Sends raw bytes back to this client
//
// KEY CONCEPT — TCP is a stream, not a message protocol.
// You can receive half a JSON object in one read() call and
// the rest in the next. The buffer + split('\n') pattern
// handles this correctly.
// ============================================================
class ClientHandler : public QObject
{
    Q_OBJECT

public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);

    void    send(const QByteArray& data);
    QString name()     const { return m_name; }
    void    setName(const QString& n) { m_name = n; }
    QString address()  const;
    qintptr socketId() const;

signals:
    void messageReceived(ClientHandler* client, const QByteArray& raw);
    void clientDisconnected(ClientHandler* client);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* m_socket;
    QByteArray  m_buffer;   // accumulates partial TCP data
    QString     m_name;
};

#endif // CLIENTHANDLER_H
