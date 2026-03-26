#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>

// ============================================================
// ClientView — VIEW layer (MVP)
//
// Pure UI — emits signals for user actions,
// exposes slots for presenter to update display.
// ============================================================
class ClientView : public QWidget
{
    Q_OBJECT

public:
    explicit ClientView(QWidget* parent = nullptr);

    // Presenter calls these to update UI
    void appendMessage(const QString& from, const QString& text);
    void appendSystem(const QString& text);
    void setConnected(bool connected);
    void clearInput();

signals:
    // User intentions
    void connectRequested(const QString& host, quint16 port, const QString& name);
    void disconnectRequested();
    void sendRequested(const QString& text);
    void pingRequested();

private slots:
    void onConnectClicked();
    void onSendClicked();

private:
    // Connection controls
    QLineEdit*   m_hostInput;
    QSpinBox*    m_portInput;
    QLineEdit*   m_nameInput;
    QPushButton* m_btnConnect;
    QPushButton* m_btnDisconnect;
    QPushButton* m_btnPing;
    QLabel*      m_statusLabel;

    // Chat area
    QTextEdit*   m_chatDisplay;
    QLineEdit*   m_messageInput;
    QPushButton* m_btnSend;
};

#endif // CLIENTVIEW_H
