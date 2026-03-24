#include "mainwindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt Signals & Slots Demo");
    setMinimumWidth(400);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel* title = new QLabel("<h2>Signals &amp; Slots Demo</h2>", this);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // --- Demo 1: Button → Counter ---
    mainLayout->addWidget(new QLabel("<b>Demo 1:</b> Button click → Counter increment", this));
    m_counter = new Counter(this);
    m_counterDisplay = new QLabel("Count: 0", this);
    m_counterDisplay->setStyleSheet("font-size: 18px; color: #2266cc;");
    mainLayout->addWidget(m_counterDisplay);

    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* btnIncrement = new QPushButton("Increment", this);
    QPushButton* btnReset     = new QPushButton("Reset", this);
    btnRow->addWidget(btnIncrement);
    btnRow->addWidget(btnReset);
    mainLayout->addLayout(btnRow);

    connect(btnIncrement, &QPushButton::clicked, m_counter, &Counter::increment);
    connect(btnReset,     &QPushButton::clicked, m_counter, &Counter::reset);
    connect(m_counter, &Counter::valueChanged, this, [this](int v) {
        m_counterDisplay->setText(QString("Count: %1").arg(v));
    });

    // --- Demo 2: SpinBox <-> Counter ---
    mainLayout->addWidget(new QLabel("<b>Demo 2:</b> SpinBox synced to same counter", this));
    m_spinBox = new QSpinBox(this);
    m_spinBox->setRange(0, 100);
    mainLayout->addWidget(m_spinBox);

    connect(m_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            m_counter, &Counter::setValue);
    connect(m_counter, &Counter::valueChanged,
            m_spinBox, &QSpinBox::setValue);

    // --- Demo 3: LineEdit → Label ---
    mainLayout->addWidget(new QLabel("<b>Demo 3:</b> LineEdit text mirrors to label", this));
    QLineEdit* input = new QLineEdit(this);
    input->setPlaceholderText("Type something...");
    mainLayout->addWidget(input);

    QLabel* mirror = new QLabel("(empty)", this);
    mirror->setStyleSheet("color: grey; font-style: italic;");
    mainLayout->addWidget(mirror);

    connect(input, &QLineEdit::textChanged, mirror, &QLabel::setText);

    mainLayout->addStretch();
}
