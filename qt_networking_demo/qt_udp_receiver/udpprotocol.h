#ifndef UDPPROTOCOL_H
#define UDPPROTOCOL_H

#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QDateTime>

// ============================================================
// UDP PROTOCOL
//
// Unlike TCP, UDP has NO connection and NO stream.
// Each datagram is INDEPENDENT — a self-contained packet.
//
// This means:
//   - No buffering needed (each readDatagram = one complete message)
//   - No newline framing needed (datagram boundary = message boundary)
//   - No guarantee of delivery — packets can be lost
//   - No guarantee of order — packet 3 can arrive before packet 2
//
// Our message format — simple JSON datagram:
//   {"type":"data", "seq":42, "payload":"...", "timestamp":"..."}
//   {"type":"ping", "seq":1}
//   {"type":"pong", "seq":1}
//   {"type":"telemetry", "seq":5, "value":98.6, "unit":"celsius"}
// ============================================================

namespace UdpProtocol {

inline QByteArray makeData(int seq, const QString& payload) {
    QJsonObject obj;
    obj["type"]      = "data";
    obj["seq"]       = seq;
    obj["payload"]   = payload;
    obj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

inline QByteArray makeTelemetry(int seq, const QString& sensor, double value, const QString& unit) {
    QJsonObject obj;
    obj["type"]      = "telemetry";
    obj["seq"]       = seq;
    obj["sensor"]    = sensor;
    obj["value"]     = value;
    obj["unit"]      = unit;
    obj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

inline QByteArray makePing(int seq) {
    QJsonObject obj;
    obj["type"] = "ping";
    obj["seq"]  = seq;
    obj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

inline QByteArray makePong(int seq) {
    QJsonObject obj;
    obj["type"] = "pong";
    obj["seq"]  = seq;
    obj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

inline QJsonObject parse(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isObject()) return doc.object();
    return QJsonObject();
}

} // namespace UdpProtocol

#endif // UDPPROTOCOL_H
