#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

// ============================================================
// PROTOCOL — JSON messages over TCP, newline delimited
//
// Every message is a JSON object followed by '\n'
// This makes framing simple — read until '\n' = one message
//
// Message types:
//   connect   → client announces its name
//   message   → client sends a chat message
//   broadcast → server sends to all clients
//   ack       → server confirms receipt
//   error     → server reports an error
//   ping      → keepalive from client
//   pong      → keepalive response from server
// ============================================================

namespace Protocol {

// Build a connect message
inline QByteArray makeConnect(const QString& name) {
    QJsonObject obj;
    obj["type"] = "connect";
    obj["name"] = name;
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
}

// Build a chat message
inline QByteArray makeMessage(const QString& from, const QString& text) {
    QJsonObject obj;
    obj["type"] = "message";
    obj["from"] = from;
    obj["text"] = text;
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
}

// Build a broadcast (server → all clients)
inline QByteArray makeBroadcast(const QString& from, const QString& text) {
    QJsonObject obj;
    obj["type"] = "broadcast";
    obj["from"] = from;
    obj["text"] = text;
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
}

// Build an ack
inline QByteArray makeAck(const QString& status = "ok") {
    QJsonObject obj;
    obj["type"] = "ack";
    obj["status"] = status;
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
}

// Build a ping
inline QByteArray makePing() {
    QJsonObject obj;
    obj["type"] = "ping";
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
}

// Build a pong
inline QByteArray makePong() {
    QJsonObject obj;
    obj["type"] = "pong";
    return QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
}

// Parse raw bytes into a QJsonObject
inline QJsonObject parse(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data.trimmed());
    if (doc.isObject()) return doc.object();
    return QJsonObject();
}

} // namespace Protocol

#endif // PROTOCOL_H
