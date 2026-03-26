#ifndef CLIENTPRESENTER_H
#define CLIENTPRESENTER_H

#include <QObject>
#include "connectionmodel.h"
#include "clientview.h"

// ============================================================
// ClientPresenter — PRESENTER layer (MVP)
//
// Wires ConnectionModel ↔ ClientView.
// Contains all application logic.
// ============================================================
class ClientPresenter : public QObject
{
    Q_OBJECT

public:
    explicit ClientPresenter(ConnectionModel* model,
                             ClientView*      view,
                             QObject*         parent = nullptr);

private slots:
    // From View
    void onConnectRequested(const QString& host, quint16 port, const QString& name);
    void onDisconnectRequested();
    void onSendRequested(const QString& text);
    void onPingRequested();

    // From Model
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QString& from, const QString& text);
    void onAckReceived(const QString& status);
    void onErrorOccurred(const QString& error);
    void onPongReceived();

private:
    ConnectionModel* m_model;
    ClientView*      m_view;
};

#endif // CLIENTPRESENTER_H
