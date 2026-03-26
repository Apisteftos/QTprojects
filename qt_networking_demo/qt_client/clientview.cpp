#include "clientview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDateTime>

ClientView::ClientView(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Qt TCP Client — Networking Demo");
    setMinimumSize(500, 600);

    QVBoxLayout* main = new QVBoxLayout(this);
    main->setSpacing(8);
    main->setContentsMargins(12, 12, 12, 12);

    // ── Connection group ──────────────────────────────────────
    QGroupBox* connGroup = new QGroupBox("Connection", this);
    QVBoxLayout* connLayout = new QVBoxLayout(connGroup);

    // Host + port row
    QHBoxLayout* hostRow = new QHBoxLayout();
    hostRow->addWidget(new QLabel("Host:", this));
    m_hostInput = new QLineEdit("127.0.0.1", this);
    hostRow->addWidget(m_hostInput);
    hostRow->addWidget(new QLabel("Port:", this));
    m_portInput = new QSpinBox(this);
    m_portInput->setRange(1, 65535);
    m_portInput->setValue(5000);
    m_portInput->setFixedWidth(80);
    hostRow->addWidget(m_portInput);
    connLayout->addLayout(hostRow);

    // Name + buttons row
    QHBoxLayout* nameRow = new QHBoxLayout();
    nameRow->addWidget(new QLabel("Name:", this));
    m_nameInput = new QLineEdit(this);
    m_nameInput->setPlaceholderText("Your name...");
    nameRow->addWidget(m_nameInput);
    m_btnConnect    = new QPushButton("Connect", this);
    m_btnDisconnect = new QPushButton("Disconnect", this);
    m_btnPing       = new QPushButton("Ping", this);
    m_btnDisconnect->setEnabled(false);
    m_btnPing->setEnabled(false);
    nameRow->addWidget(m_btnConnect);
    nameRow->addWidget(m_btnDisconnect);
    nameRow->addWidget(m_btnPing);
    connLayout->addLayout(nameRow);

    // Status
    m_statusLabel = new QLabel("● Disconnected", this);
    m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    connLayout->addWidget(m_statusLabel);

    main->addWidget(connGroup);

    // ── Chat display ──────────────────────────────────────────
    m_chatDisplay = new QTextEdit(this);
    m_chatDisplay->setReadOnly(true);
    m_chatDisplay->setStyleSheet("font-family: monospace; font-size: 13px;");
    main->addWidget(m_chatDisplay);

    // ── Message input row ─────────────────────────────────────
    QHBoxLayout* msgRow = new QHBoxLayout();
    m_messageInput = new QLineEdit(this);
    m_messageInput->setPlaceholderText("Type a message...");
    m_messageInput->setEnabled(false);
    m_btnSend = new QPushButton("Send", this);
    m_btnSend->setEnabled(false);
    msgRow->addWidget(m_messageInput);
    msgRow->addWidget(m_btnSend);
    main->addLayout(msgRow);

    // ── Wiring ────────────────────────────────────────────────
    connect(m_btnConnect,    &QPushButton::clicked, this, &ClientView::onConnectClicked);
    connect(m_btnDisconnect, &QPushButton::clicked, this, &ClientView::disconnectRequested);
    connect(m_btnSend,       &QPushButton::clicked, this, &ClientView::onSendClicked);
    connect(m_btnPing,       &QPushButton::clicked, this, &ClientView::pingRequested);
    connect(m_messageInput,  &QLineEdit::returnPressed, this, &ClientView::onSendClicked);
}

void ClientView::appendMessage(const QString& from, const QString& text) {
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_chatDisplay->append(
        QString("<span style='color:#888'>[%1]</span> "
                "<b style='color:#2266cc'>%2</b>: %3")
        .arg(time, from.toHtmlEscaped(), text.toHtmlEscaped())
    );
}

void ClientView::appendSystem(const QString& text) {
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_chatDisplay->append(
        QString("<span style='color:#888'>[%1]</span> "
                "<i style='color:#888'>%2</i>")
        .arg(time, text.toHtmlEscaped())
    );
}

void ClientView::setConnected(bool connected) {
    m_btnConnect->setEnabled(!connected);
    m_btnDisconnect->setEnabled(connected);
    m_btnPing->setEnabled(connected);
    m_messageInput->setEnabled(connected);
    m_btnSend->setEnabled(connected);
    m_hostInput->setEnabled(!connected);
    m_portInput->setEnabled(!connected);
    m_nameInput->setEnabled(!connected);

    if (connected) {
        m_statusLabel->setText("● Connected");
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
    } else {
        m_statusLabel->setText("● Disconnected");
        m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
}

void ClientView::clearInput() {
    m_messageInput->clear();
}

void ClientView::onConnectClicked() {
    emit connectRequested(
        m_hostInput->text(),
        static_cast<quint16>(m_portInput->value()),
        m_nameInput->text().isEmpty() ? "anonymous" : m_nameInput->text()
    );
}

void ClientView::onSendClicked() {
    QString text = m_messageInput->text().trimmed();
    if (!text.isEmpty()) emit sendRequested(text);
}
