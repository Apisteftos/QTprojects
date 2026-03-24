#include "taskpresenter.h"

TaskPresenter::TaskPresenter(TaskModel* model, TaskView* view, QObject* parent)
    : QObject(parent), m_model(model), m_view(view)
{
    // ── View → Presenter ──────────────────────────────────
    connect(m_view, &TaskView::addRequested,
            this,   &TaskPresenter::onAddRequested);
    connect(m_view, &TaskView::removeRequested,
            this,   &TaskPresenter::onRemoveRequested);
    connect(m_view, &TaskView::toggleRequested,
            this,   &TaskPresenter::onToggleRequested);
    connect(m_view, &TaskView::clearAllRequested,
            this,   &TaskPresenter::onClearAllRequested);

    // ── Model → Presenter ─────────────────────────────────
    connect(m_model, &TaskModel::taskAdded,
            this,    &TaskPresenter::onTaskAdded);
    connect(m_model, &TaskModel::taskRemoved,
            this,    &TaskPresenter::onTaskRemoved);
    connect(m_model, &TaskModel::taskUpdated,
            this,    &TaskPresenter::onTaskUpdated);
    connect(m_model, &TaskModel::modelReset,
            this,    &TaskPresenter::onModelReset);

    // Initial state
    m_view->setTasks(m_model->tasks());
    refreshStats();
}

// ── View → Presenter handlers ──────────────────────────────
void TaskPresenter::onAddRequested(const QString& title) {
    m_model->addTask(title);   // Delegate to model
    m_view->clearInput();      // Update view
}

void TaskPresenter::onRemoveRequested(int id) {
    m_model->removeTask(id);
}

void TaskPresenter::onToggleRequested(int id) {
    m_model->toggleDone(id);
}

void TaskPresenter::onClearAllRequested() {
    m_model->clearAll();
}

// ── Model → Presenter handlers ─────────────────────────────
void TaskPresenter::onTaskAdded(const Task& task) {
    m_view->addTaskItem(task);
    refreshStats();
}

void TaskPresenter::onTaskRemoved(int id) {
    m_view->removeTaskItem(id);
    refreshStats();
}

void TaskPresenter::onTaskUpdated(const Task& task) {
    m_view->updateTaskItem(task);
    refreshStats();
}

void TaskPresenter::onModelReset() {
    m_view->setTasks(m_model->tasks());
    refreshStats();
}

void TaskPresenter::refreshStats() {
    m_view->updateStats(m_model->count(), m_model->doneCount());
}
