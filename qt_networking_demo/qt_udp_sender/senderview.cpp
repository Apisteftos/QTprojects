#include "senderview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDateTime>

SenderView::SenderView(QWidget* parent)
    : QWidget(parent), m_autoActive(false)
{
    setWindowTitle("Qt UDP Sender");
    setMinimumSize(480, 620);

    QVBoxLayout* main = new QVBoxLayout(this);
    main->setSpacing(8);
    main->setContentsMargins(12, 12, 12, 12);

    // ── Target group ──────────────────────────────────────────
    QGroupBox* targetGroup = new QGroupBox("Target", this);
    QHBoxLayout* targetRow = new QHBoxLayout(targetGroup);
    targetRow->addWidget(new QLabel("Host:", this));
    m_hostInput = new QLineEdit("127.0.0.1", this);
    targetRow->addWidget(m_hostInput);
    targetRow->addWidget(new QLabel("Port:", this));
    m_portInput = new QSpinBox(this);
    m_portInput->setRange(1, 65535);
    m_portInput->setValue(5000);
    m_portInput->setFixedWidth(80);
    targetRow->addWidget(m_portInput);
    main->addWidget(targetGroup);

    // ── Manual data group ─────────────────────────────────────
    QGroupBox* dataGroup = new QGroupBox("Send Data Packet", this);
    QHBoxLayout* dataRow = new QHBoxLayout(dataGroup);
    m_payloadInput = new QLineEdit(this);
    m_payloadInput->setPlaceholderText("Payload text...");
    m_btnSendData  = new QPushButton("Send", this);
    dataRow->addWidget(m_payloadInput);
    dataRow->addWidget(m_btnSendData);
    main->addWidget(dataGroup);

    // ── Telemetry group ───────────────────────────────────────
    QGroupBox* telemGroup = new QGroupBox("Send Telemetry", this);
    QVBoxLayout* telemLayout = new QVBoxLayout(telemGroup);

    QHBoxLayout* telemRow1 = new QHBoxLayout();
    telemRow1->addWidget(new QLabel("Sensor:", this));
    m_sensorCombo = new QComboBox(this);
    m_sensorCombo->addItems({"temperature", "pressure", "humidity", "rpm", "voltage"});
    telemRow1->addWidget(m_sensorCombo);
    telemRow1->addWidget(new QLabel("Value:", this));
    m_valueInput = new QDoubleSpinBox(this);
    m_valueInput->setRange(-9999, 9999);
    m_valueInput->setValue(23.5);
    telemRow1->addWidget(m_valueInput);
    telemRow1->addWidget(new QLabel("Unit:", this));
    m_unitCombo = new QComboBox(this);
    m_unitCombo->addItems({"celsius", "bar", "%", "rpm", "V"});
    telemRow1->addWidget(m_unitCombo);
    telemLayout->addLayout(telemRow1);

    m_btnSendTelemetry = new QPushButton("Send Telemetry", this);
    telemLayout->addWidget(m_btnSendTelemetry);
    main->addWidget(telemGroup);

    // ── Auto send group ───────────────────────────────────────
    QGroupBox* autoGroup = new QGroupBox("Auto Send (simulates sensor stream)", this);
    QHBoxLayout* autoRow = new QHBoxLayout(autoGroup);
    autoRow->addWidget(new QLabel("Interval (ms):", this));
    m_intervalInput = new QSpinBox(this);
    m_intervalInput->setRange(100, 5000);
    m_intervalInput->setValue(500);
    m_intervalInput->setSingleStep(100);
    autoRow->addWidget(m_intervalInput);
    m_btnAutoToggle = new QPushButton("Start Auto Send", this);
    m_btnAutoToggle->setCheckable(true);
    autoRow->addWidget(m_btnAutoToggle);
    main->addWidget(autoGroup);

    // ── Ping group ────────────────────────────────────────────
    QGroupBox* pingGroup = new QGroupBox("Ping (latency measurement)", this);
    QHBoxLayout* pingRow = new QHBoxLayout(pingGroup);
    m_btnPing = new QPushButton("Send Ping", this);
    pingRow->addWidget(m_btnPing);
    main->addWidget(pingGroup);

    // ── Stats ─────────────────────────────────────────────────
    m_statsLabel = new QLabel("Sent: 0  |  Lost: 0", this);
    m_statsLabel->setStyleSheet("color: #555; font-style: italic;");
    main->addWidget(m_statsLabel);

    // ── Log ───────────────────────────────────────────────────
    m_log = new QTextEdit(this);
    m_log->setReadOnly(true);
    m_log->setStyleSheet("font-family: monospace; font-size: 12px;");
    main->addWidget(m_log);

    // ── Wiring ────────────────────────────────────────────────
    connect(m_btnSendData,      &QPushButton::clicked, this, &SenderView::onSendDataClicked);
    connect(m_btnSendTelemetry, &QPushButton::clicked, this, &SenderView::onSendTelemetryClicked);
    connect(m_btnPing,          &QPushButton::clicked, this, &SenderView::onPingClicked);
    connect(m_btnAutoToggle,    &QPushButton::clicked, this, &SenderView::onAutoToggleClicked);
    connect(m_payloadInput,     &QLineEdit::returnPressed, this, &SenderView::onSendDataClicked);
}

void SenderView::appendLog(const QString& msg) {
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    m_log->append(QString("<span style='color:#888'>[%1]</span> %2").arg(time, msg));
}

void SenderView::setAutoSending(bool active) {
    m_autoActive = active;
    m_btnAutoToggle->setText(active ? "Stop Auto Send" : "Start Auto Send");
    m_btnAutoToggle->setChecked(active);
    m_intervalInput->setEnabled(!active);
}

void SenderView::updateStats(int sent, int lost) {
    m_statsLabel->setText(QString("Sent: %1  |  Lost: %2").arg(sent).arg(lost));
}

void SenderView::onSendDataClicked() {
    QString payload = m_payloadInput->text().trimmed();
    if (payload.isEmpty()) payload = "test packet";
    emit sendDataRequested(m_hostInput->text(),
                           static_cast<quint16>(m_portInput->value()),
                           payload);
    m_payloadInput->clear();
}

void SenderView::onSendTelemetryClicked() {
    emit sendTelemetryRequested(
        m_hostInput->text(),
        static_cast<quint16>(m_portInput->value()),
        m_sensorCombo->currentText(),
        m_valueInput->value(),
        m_unitCombo->currentText()
    );
}

void SenderView::onPingClicked() {
    emit sendPingRequested(m_hostInput->text(),
                           static_cast<quint16>(m_portInput->value()));
}

void SenderView::onAutoToggleClicked() {
    if (!m_autoActive) {
        emit startAutoRequested(m_hostInput->text(),
                                static_cast<quint16>(m_portInput->value()),
                                m_intervalInput->value());
    } else {
        emit stopAutoRequested();
    }
}
