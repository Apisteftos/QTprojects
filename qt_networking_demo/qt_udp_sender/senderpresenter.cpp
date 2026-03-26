#include "senderpresenter.h"

SenderPresenter::SenderPresenter(SenderModel* model,
                                 SenderView*  view,
                                 QObject*     parent)
    : QObject(parent), m_model(model), m_view(view)
{
    // View → Presenter
    connect(m_view, &SenderView::sendDataRequested,
            this,   &SenderPresenter::onSendDataRequested);
    connect(m_view, &SenderView::sendTelemetryRequested,
            this,   &SenderPresenter::onSendTelemetryRequested);
    connect(m_view, &SenderView::sendPingRequested,
            this,   &SenderPresenter::onSendPingRequested);
    connect(m_view, &SenderView::startAutoRequested,
            this,   &SenderPresenter::onStartAutoRequested);
    connect(m_view, &SenderView::stopAutoRequested,
            this,   &SenderPresenter::onStopAutoRequested);

    // Model → Presenter
    connect(m_model, &SenderModel::dataSent,
            this,    &SenderPresenter::onDataSent);
    connect(m_model, &SenderModel::pongReceived,
            this,    &SenderPresenter::onPongReceived);
    connect(m_model, &SenderModel::errorOccurred,
            this,    &SenderPresenter::onErrorOccurred);
    connect(m_model, &SenderModel::statsUpdated,
            this,    &SenderPresenter::onStatsUpdated);
}

void SenderPresenter::onSendDataRequested(const QString& host, quint16 port, const QString& payload) {
    m_model->sendData(host, port, payload);
}

void SenderPresenter::onSendTelemetryRequested(const QString& host, quint16 port,
                                               const QString& sensor, double value,
                                               const QString& unit) {
    m_model->sendTelemetry(host, port, sensor, value, unit);
}

void SenderPresenter::onSendPingRequested(const QString& host, quint16 port) {
    m_model->sendPing(host, port);
    m_view->appendLog("<i style='color:#2266cc'>Ping sent — waiting for pong...</i>");
}

void SenderPresenter::onStartAutoRequested(const QString& host, quint16 port, int intervalMs) {
    m_model->startAutoSend(host, port, intervalMs);
    m_view->setAutoSending(true);
    m_view->appendLog(QString("<i>Auto send started — interval: %1 ms</i>").arg(intervalMs));
}

void SenderPresenter::onStopAutoRequested() {
    m_model->stopAutoSend();
    m_view->setAutoSending(false);
    m_view->appendLog("<i>Auto send stopped.</i>");
}

void SenderPresenter::onDataSent(int seq, const QString& type) {
    m_view->appendLog(QString("→ Sent <b>%1</b> seq=%2").arg(type).arg(seq));
}

void SenderPresenter::onPongReceived(int seq, int latencyMs) {
    m_view->appendLog(
        QString("← <b style='color:green'>Pong</b> seq=%1  latency=<b>%2 ms</b>")
        .arg(seq).arg(latencyMs)
    );
}

void SenderPresenter::onErrorOccurred(const QString& error) {
    m_view->appendLog(QString("<span style='color:red'>Error: %1</span>").arg(error));
}

void SenderPresenter::onStatsUpdated(int sent, int lost) {
    m_view->updateStats(sent, lost);
}
