#ifndef SENDERPRESENTER_H
#define SENDERPRESENTER_H

#include <QObject>
#include "sendermodel.h"
#include "senderview.h"

class SenderPresenter : public QObject
{
    Q_OBJECT

public:
    explicit SenderPresenter(SenderModel* model,
                             SenderView*  view,
                             QObject*     parent = nullptr);

private slots:
    // From View
    void onSendDataRequested(const QString& host, quint16 port, const QString& payload);
    void onSendTelemetryRequested(const QString& host, quint16 port,
                                  const QString& sensor, double value, const QString& unit);
    void onSendPingRequested(const QString& host, quint16 port);
    void onStartAutoRequested(const QString& host, quint16 port, int intervalMs);
    void onStopAutoRequested();

    // From Model
    void onDataSent(int seq, const QString& type);
    void onPongReceived(int seq, int latencyMs);
    void onErrorOccurred(const QString& error);
    void onStatsUpdated(int sent, int lost);

private:
    SenderModel* m_model;
    SenderView*  m_view;
};

#endif // SENDERPRESENTER_H
