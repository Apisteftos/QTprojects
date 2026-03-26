#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

// ============================================================
// ConnectionModel — MODEL layer
//
// Owns the QTcpSocket. Handles all raw TCP I/O.
// Knows nothing about the UI.
// ============================================================
class ConnectionModel : public QObject
{
    Q_OBJECT

public:
    explicit ConnectionModel(QObject* parent = nullptr);

    bool isConnected() const;

public slots:
    void connectToServer(const QString& host, quint16 port, const QString& name);
    void disconnectFromServer();
    void sendMessage(const QString& text);
    void sendPing();

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString& from, const QString& text);
    void ackReceived(const QString& status);
    void errorOccurred(const QString& error);
    void pongReceived();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);

private:
    QTcpSocket* m_socket;
    QByteArray  m_buffer;
    QString     m_name;
};

#endif // CONNECTIONMODEL_H
