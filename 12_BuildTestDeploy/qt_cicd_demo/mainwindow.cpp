#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt CI/CD Demo — Calculator");
    setMinimumSize(400, 500);

    m_calc = new Calculator(this);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setSpacing(10);
    layout->setContentsMargins(16, 16, 16, 16);

    // Title
    QLabel* title = new QLabel(
        "<h2>Calculator</h2>"
        "<p style='color:#555'>Enter expression: e.g. <b>10 + 5</b>, "
        "<b>20 / 4</b>, <b>3.5 * 2</b></p>", this);
    layout->addWidget(title);

    // Input
    QHBoxLayout* inputRow = new QHBoxLayout();
    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("e.g. 10 + 5");
    m_input->setStyleSheet("font-size: 16px; padding: 6px;");
    m_btnCalc = new QPushButton("=", this);
    m_btnCalc->setStyleSheet(
        "background: #2E75B6; color: white; "
        "font-size: 18px; padding: 6px 16px;");
    inputRow->addWidget(m_input);
    inputRow->addWidget(m_btnCalc);
    layout->addLayout(inputRow);

    // Result
    m_result = new QLabel("Result: —", this);
    m_result->setStyleSheet(
        "font-size: 22px; font-weight: bold; "
        "color: #1F4E79; padding: 8px; "
        "background: #f0f4ff; border-radius: 4px;");
    m_result->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_result);

    // History
    QGroupBox* histGroup = new QGroupBox("History", this);
    QVBoxLayout* histLayout = new QVBoxLayout(histGroup);
    m_history = new QListWidget(this);
    m_history->setAlternatingRowColors(true);
    histLayout->addWidget(m_history);
    m_btnClear = new QPushButton("Clear History", this);
    histLayout->addWidget(m_btnClear);
    layout->addWidget(histGroup);

    setCentralWidget(central);

    // Wiring
    connect(m_btnCalc, &QPushButton::clicked,
            this,      &MainWindow::onCalculate);
    connect(m_input,   &QLineEdit::returnPressed,
            this,      &MainWindow::onCalculate);
    connect(m_btnClear,&QPushButton::clicked,
            this,      &MainWindow::onClear);
    connect(m_calc,    &Calculator::resultReady,
            this,      &MainWindow::onResultReady);
    connect(m_calc,    &Calculator::errorOccurred,
            this,      &MainWindow::onErrorOccurred);
    connect(m_calc,    &Calculator::historyChanged,
            this,      &MainWindow::onHistoryChanged);
}

void MainWindow::onCalculate() {
    bool ok;
    m_calc->evaluate(m_input->text(), &ok);
}

void MainWindow::onClear() {
    m_calc->clearHistory();
    m_input->clear();
    m_result->setText("Result: —");
    m_result->setStyleSheet(
        "font-size: 22px; font-weight: bold; "
        "color: #1F4E79; padding: 8px; "
        "background: #f0f4ff; border-radius: 4px;");
}

void MainWindow::onResultReady(double result, const QString& expression) {
    Q_UNUSED(expression)
    m_result->setText(QString("= %1").arg(result));
    m_result->setStyleSheet(
        "font-size: 22px; font-weight: bold; "
        "color: #1b5e20; padding: 8px; "
        "background: #e8f5e9; border-radius: 4px;");
    m_input->clear();
}

void MainWindow::onErrorOccurred(const QString& error) {
    m_result->setText("Error: " + error);
    m_result->setStyleSheet(
        "font-size: 18px; font-weight: bold; "
        "color: #b71c1c; padding: 8px; "
        "background: #ffebee; border-radius: 4px;");
}

void MainWindow::onHistoryChanged() {
    m_history->clear();
    for (const QString& entry : m_calc->history())
        m_history->addItem(entry);
}
