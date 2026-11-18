#ifndef TASKPRESENTER_H
#define TASKPRESENTER_H

#include <QObject>
#include "taskmodel.h"
#include "taskview.h"

// ============================================================
// TaskPresenter — the PRESENTER layer (MVP)
//
// Responsibilities:
//   - Sits between Model and View
//   - Listens to View signals → calls Model methods
//   - Listens to Model signals → updates View
//   - Contains the application logic (what happens when X occurs)
//   - Neither Model nor View know about each other
// ============================================================
class TaskPresenter : public QObject
{
    Q_OBJECT

public:
    explicit TaskPresenter(TaskModel* model,
                           TaskView*  view,
                           QObject*   parent = nullptr);

private slots:
    // From View
    void onAddRequested(const QString& title);
    void onRemoveRequested(int id);
    void onToggleRequested(int id);
    void onClearAllRequested();

    // From Model
    void onTaskAdded(const Task& task);
    void onTaskRemoved(int id);
    void onTaskUpdated(const Task& task);
    void onModelReset();

private:
    void refreshStats();

    TaskModel* m_model;
    TaskView*  m_view;
};

#endif // TASKPRESENTER_H
