#include "stationclient.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

StationClient::StationClient(QObject *parent)
    : QAbstractListModel(parent)
{
}

int StationClient::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : count();
}

QVariant StationClient::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= count())
        return {};

    const Station &s = m_stations.at(index.row());
    switch (role) {
    case NameRole:
        return s.name;
    case CityRole:
        return s.city;
    case TemperatureRole:
        return s.temperature;
    }
    return {};
}

QHash<int, QByteArray> StationClient::roleNames() const
{
    return {
        {NameRole, "name"},
        {CityRole, "city"},
        {TemperatureRole, "temperature"},
    };
}

void StationClient::setSource(const QUrl &source)
{
    if (source == m_source)
        return;
    m_source = source;
    emit sourceChanged();
}

double StationClient::average() const
{
    if (m_stations.isEmpty())
        return 0.0;
    double sum = 0.0;
    for (const Station &s : m_stations)
        sum += s.temperature;
    return sum / double(m_stations.size());
}

void StationClient::load()
{
    abort();                       // only one request at a time
    setError(QString());
    setLoading(true);

    QNetworkRequest request(m_source);
    request.setTransferTimeout(5000);   // milliseconds: a request must not hang for ever
    m_reply = m_network.get(request);   // returns at once; the answer comes as a signal
    connect(m_reply, &QNetworkReply::finished, this, [this, reply = m_reply.data()] { finished(reply); });
}

void StationClient::abort()
{
    if (m_reply) {
        QNetworkReply *reply = m_reply;
        m_reply = nullptr;             // so that finished() ignores the answer
        reply->abort();
        reply->deleteLater();
        setLoading(false);
    }
}

void StationClient::finished(QNetworkReply *reply)
{
    reply->deleteLater();              // never delete a reply in its own signal: deleteLater()
    if (reply != m_reply)
        return;                        // an old request that was aborted
    m_reply = nullptr;
    setLoading(false);

    if (reply->error() != QNetworkReply::NoError) {
        setError(reply->errorString());
        return;
    }
    setError(parse(reply->readAll()));
}

QString StationClient::parse(const QByteArray &json)
{
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return QStringLiteral("The data are not valid JSON: %1").arg(parseError.errorString());

    const QJsonArray array = doc.object().value("stations").toArray();
    if (array.isEmpty() && !doc.object().contains("stations"))
        return QStringLiteral("The data have no \"stations\" list");

    QList<Station> stations;
    for (const QJsonValue &value : array) {
        const QJsonObject o = value.toObject();
        stations.append({o.value("name").toString(), o.value("city").toString(), o.value("temperature").toDouble()});
    }

    // Replace everything at once: the views start again
    beginResetModel();
    m_stations = stations;
    endResetModel();
    emit countChanged();
    emit averageChanged();

    m_updated = doc.object().value("updated").toString();
    emit updatedChanged();
    return QString();
}

void StationClient::setLoading(bool loading)
{
    if (loading == m_loading)
        return;
    m_loading = loading;
    emit loadingChanged();
}

void StationClient::setError(const QString &error)
{
    if (error == m_error)
        return;
    m_error = error;
    emit errorChanged();
}
