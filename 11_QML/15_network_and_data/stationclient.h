#ifndef STATIONCLIENT_H
#define STATIONCLIENT_H

#include <QAbstractListModel>
#include <QList>
#include <QNetworkAccessManager>
#include <QPointer>
#include <QUrl>
#include <QtQml/qqmlregistration.h>

class QNetworkReply;

// The backend: it loads a JSON document from a URL and offers the stations as a list model for QML.
// QML knows nothing about the network, JSON or error codes. It sees properties and rows.
class StationClient : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString updated READ updated NOTIFY updatedChanged)
    Q_PROPERTY(double average READ average NOTIFY averageChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        CityRole,
        TemperatureRole,
    };

    explicit StationClient(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QUrl source() const { return m_source; }
    void setSource(const QUrl &source);
    bool loading() const { return m_loading; }
    QString error() const { return m_error; }
    QString updated() const { return m_updated; }
    double average() const;
    int count() const { return int(m_stations.size()); }

    Q_INVOKABLE void load();                  // starts the request and returns at once
    Q_INVOKABLE void abort();

    // The parser is a normal function that works on bytes. It does not need a network, so a test can call it.
    // Returns an empty string if the data are good, or the text of the error.
    QString parse(const QByteArray &json);

signals:
    void sourceChanged();
    void loadingChanged();
    void errorChanged();
    void updatedChanged();
    void averageChanged();
    void countChanged();

private:
    struct Station
    {
        QString name;
        QString city;
        double temperature = 0.0;
    };

    void setLoading(bool loading);
    void setError(const QString &error);
    void finished(QNetworkReply *reply);

    QNetworkAccessManager m_network;
    QPointer<QNetworkReply> m_reply;
    QUrl m_source{QStringLiteral("qrc:/data/stations.json")};
    QList<Station> m_stations;
    QString m_updated;
    QString m_error;
    bool m_loading = false;
};

#endif // STATIONCLIENT_H
