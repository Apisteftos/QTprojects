#include "taskview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QFont>

TaskView::TaskView(QWidget* parent)
    : QWidget(parent)
{
    // ── Layout ───────────────────────────────────────────────
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // Title
    QLabel* title = new QLabel("<h2>Task Manager — MVP Demo</h2>", this);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Stats
    m_statsLabel = new QLabel("Tasks: 0  |  Done: 0", this);
    m_statsLabel->setAlignment(Qt::AlignCenter);
    m_statsLabel->setStyleSheet("color: #555; font-style: italic;");
    mainLayout->addWidget(m_statsLabel);

    // Task list
    m_list = new QListWidget(this);
    m_list->setAlternatingRowColors(true);
    m_list->setToolTip("Double-click a task to mark it done/undone");
    mainLayout->addWidget(m_list);

    // Input row
    QHBoxLayout* inputRow = new QHBoxLayout();
    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("Enter task title...");
    m_btnAdd = new QPushButton("Add", this);
    m_btnAdd->setDefault(true);
    inputRow->addWidget(m_input);
    inputRow->addWidget(m_btnAdd);
    mainLayout->addLayout(inputRow);

    // Button row
    QHBoxLayout* btnRow = new QHBoxLayout();
    m_btnRemove = new QPushButton("Remove Selected", this);
    m_btnClear  = new QPushButton("Clear All", this);
    btnRow->addWidget(m_btnRemove);
    btnRow->addWidget(m_btnClear);
    mainLayout->addLayout(btnRow);

    // ── Internal signal wiring (View → View) ────────────────
    connect(m_btnAdd,    &QPushButton::clicked,
            this,        &TaskView::onAddClicked);
    connect(m_input,     &QLineEdit::returnPressed,
            this,        &TaskView::onAddClicked);
    connect(m_btnRemove, &QPushButton::clicked,
            this,        &TaskView::onRemoveClicked);
    connect(m_btnClear,  &QPushButton::clicked,
            this,        &TaskView::clearAllRequested);
    connect(m_list,      &QListWidget::itemDoubleClicked,
            this,        &TaskView::onItemDoubleClicked);
}

void TaskView::setTasks(const QList<Task>& tasks) {
    m_list->clear();
    for (const Task& t : tasks) addTaskItem(t);
}

void TaskView::addTaskItem(const Task& task) {
    QListWidgetItem* item = new QListWidgetItem(task.title);
    item->setData(Qt::UserRole, task.id);
    if (task.done) {
        item->setForeground(Qt::gray);
        item->setText("[✓] " + task.title);
    }
    m_list->addItem(item);
}

void TaskView::removeTaskItem(int id) {
    for (int i = 0; i < m_list->count(); i++) {
        if (m_list->item(i)->data(Qt::UserRole).toInt() == id) {
            delete m_list->takeItem(i);
            return;
        }
    }
}

void TaskView::updateTaskItem(const Task& task) {
    for (int i = 0; i < m_list->count(); i++) {
        QListWidgetItem* item = m_list->item(i);
        if (item->data(Qt::UserRole).toInt() == task.id) {
            item->setText(task.done ? "[✓] " + task.title : task.title);
            item->setForeground(task.done ? Qt::gray : Qt::black);
            return;
        }
    }
}

void TaskView::clearInput() {
    m_input->clear();
}

void TaskView::updateStats(int total, int done) {
    m_statsLabel->setText(
        QString("Tasks: %1  |  Done: %2  |  Remaining: %3")
            .arg(total).arg(done).arg(total - done)
    );
}

// ── Private slots (translate UI events → View signals) ──────
void TaskView::onAddClicked() {
    emit addRequested(m_input->text());
}

void TaskView::onRemoveClicked() {
    QListWidgetItem* item = m_list->currentItem();
    if (item) emit removeRequested(item->data(Qt::UserRole).toInt());
}

void TaskView::onItemDoubleClicked(QListWidgetItem* item) {
    emit toggleRequested(item->data(Qt::UserRole).toInt());
}
