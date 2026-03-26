#include "clientpresenter.h"

ClientPresenter::ClientPresenter(ConnectionModel* model,
                                 ClientView*      view,
                                 QObject*         parent)
    : QObject(parent), m_model(model), m_view(view)
{
    // View → Presenter
    connect(m_view, &ClientView::connectRequested,
            this,   &ClientPresenter::onConnectRequested);
    connect(m_view, &ClientView::disconnectRequested,
            this,   &ClientPresenter::onDisconnectRequested);
    connect(m_view, &ClientView::sendRequested,
            this,   &ClientPresenter::onSendRequested);
    connect(m_view, &ClientView::pingRequested,
            this,   &ClientPresenter::onPingRequested);

    // Model → Presenter
    connect(m_model, &ConnectionModel::connected,
            this,    &ClientPresenter::onConnected);
    connect(m_model, &ConnectionModel::disconnected,
            this,    &ClientPresenter::onDisconnected);
    connect(m_model, &ConnectionModel::messageReceived,
            this,    &ClientPresenter::onMessageReceived);
    connect(m_model, &ConnectionModel::ackReceived,
            this,    &ClientPresenter::onAckReceived);
    connect(m_model, &ConnectionModel::errorOccurred,
            this,    &ClientPresenter::onErrorOccurred);
    connect(m_model, &ConnectionModel::pongReceived,
            this,    &ClientPresenter::onPongReceived);
}

void ClientPresenter::onConnectRequested(const QString& host, quint16 port, const QString& name) {
    m_view->appendSystem(QString("Connecting to %1:%2 as '%3'...").arg(host).arg(port).arg(name));
    m_model->connectToServer(host, port, name);
}

void ClientPresenter::onDisconnectRequested() {
    m_model->disconnectFromServer();
}

void ClientPresenter::onSendRequested(const QString& text) {
    m_model->sendMessage(text);
    m_view->appendMessage("you", text);
    m_view->clearInput();
}

void ClientPresenter::onPingRequested() {
    m_model->sendPing();
    m_view->appendSystem("Ping sent...");
}

void ClientPresenter::onConnected() {
    m_view->setConnected(true);
    m_view->appendSystem("Connected to server.");
}

void ClientPresenter::onDisconnected() {
    m_view->setConnected(false);
    m_view->appendSystem("Disconnected from server.");
}

void ClientPresenter::onMessageReceived(const QString& from, const QString& text) {
    m_view->appendMessage(from, text);
}

void ClientPresenter::onAckReceived(const QString& status) {
    if (status == "welcome")
        m_view->appendSystem("Server says: welcome!");
}

void ClientPresenter::onErrorOccurred(const QString& error) {
    m_view->setConnected(false);
    m_view->appendSystem(QString("Error: %1").arg(error));
}

void ClientPresenter::onPongReceived() {
    m_view->appendSystem("Pong received from server.");
}
