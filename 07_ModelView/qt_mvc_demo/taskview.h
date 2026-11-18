#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "taskmodel.h"

// ============================================================
// TaskView — the VIEW layer
//
// Responsibilities:
//   - Renders data to the screen
//   - Emits signals when the user does something
//   - Does NOT modify the model directly
//   - Does NOT contain business logic
//   - Receives updates via public slots
// ============================================================
class TaskView : public QWidget
{
    Q_OBJECT

public:
    explicit TaskView(QWidget* parent = nullptr);

    // Called by Presenter to push data into the view
    void setTasks(const QList<Task>& tasks);
    void addTaskItem(const Task& task);
    void removeTaskItem(int id);
    void updateTaskItem(const Task& task);
    void clearInput();
    void updateStats(int total, int done);

signals:
    // User intentions — Presenter listens to these
    void addRequested(const QString& title);
    void removeRequested(int id);
    void toggleRequested(int id);
    void clearAllRequested();

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onItemDoubleClicked(QListWidgetItem* item);

private:
    QListWidget* m_list;
    QLineEdit*   m_input;
    QPushButton* m_btnAdd;
    QPushButton* m_btnRemove;
    QPushButton* m_btnClear;
    QLabel*      m_statsLabel;
};

#endif // TASKVIEW_H
