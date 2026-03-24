#include "taskviewmodel.h"

// ── TaskListModel ─────────────────────────────────────────────

TaskListModel::TaskListModel(QObject* parent)
    : QAbstractListModel(parent) {}

int TaskListModel::rowCount(const QModelIndex&) const {
    return m_tasks.size();
}

QVariant TaskListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_tasks.size())
        return QVariant();
    const Task& t = m_tasks[index.row()];
    switch (role) {
        case TitleRole: return t.title;
        case DoneRole:  return t.done;
        case IdRole:    return t.id;
    }
    return QVariant();
}

QHash<int, QByteArray> TaskListModel::roleNames() const {
    return {
        { TitleRole, "title"  },
        { DoneRole,  "done"   },
        { IdRole,    "taskId" }
    };
}

void TaskListModel::setTasks(const QList<Task>& tasks) {
    beginResetModel();
    m_tasks = tasks;
    endResetModel();
}

void TaskListModel::addTask(const Task& task) {
    beginInsertRows(QModelIndex(), m_tasks.size(), m_tasks.size());
    m_tasks.append(task);
    endInsertRows();
}

void TaskListModel::removeTask(int id) {
    for (int i = 0; i < m_tasks.size(); i++) {
        if (m_tasks[i].id == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_tasks.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

void TaskListModel::updateTask(const Task& task) {
    for (int i = 0; i < m_tasks.size(); i++) {
        if (m_tasks[i].id == task.id) {
            m_tasks[i] = task;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, { DoneRole, TitleRole });
            return;
        }
    }
}

void TaskListModel::reset(const QList<Task>& tasks) {
    setTasks(tasks);
}

// ── TaskViewModel ─────────────────────────────────────────────

TaskViewModel::TaskViewModel(TaskModel* model, QObject* parent)
    : QObject(parent), m_model(model)
{
    m_listModel = new TaskListModel(this);
    m_listModel->setTasks(m_model->tasks());

    // Model → ViewModel
    connect(m_model, &TaskModel::taskAdded,   this, &TaskViewModel::onTaskAdded);
    connect(m_model, &TaskModel::taskRemoved, this, &TaskViewModel::onTaskRemoved);
    connect(m_model, &TaskModel::taskUpdated, this, &TaskViewModel::onTaskUpdated);
    connect(m_model, &TaskModel::modelReset,  this, &TaskViewModel::onModelReset);
}

TaskListModel* TaskViewModel::tasks()          const { return m_listModel; }
int            TaskViewModel::totalCount()     const { return m_model->count(); }
int            TaskViewModel::doneCount()      const { return m_model->doneCount(); }
int            TaskViewModel::remainingCount() const { return m_model->count() - m_model->doneCount(); }
QString        TaskViewModel::statusText()     const {
    return QString("Total: %1  |  Done: %2  |  Remaining: %3")
        .arg(totalCount()).arg(doneCount()).arg(remainingCount());
}

// Q_INVOKABLE — called from QML
void TaskViewModel::addTask(const QString& title)  { m_model->addTask(title); }
void TaskViewModel::removeTask(int id)             { m_model->removeTask(id); }
void TaskViewModel::toggleDone(int id)             { m_model->toggleDone(id); }
void TaskViewModel::clearAll()                     { m_model->clearAll(); }

// Model signal handlers
void TaskViewModel::onTaskAdded(const Task& task)  { m_listModel->addTask(task);    emit statsChanged(); }
void TaskViewModel::onTaskRemoved(int id)          { m_listModel->removeTask(id);   emit statsChanged(); }
void TaskViewModel::onTaskUpdated(const Task& task){ m_listModel->updateTask(task); emit statsChanged(); }
void TaskViewModel::onModelReset()                 { m_listModel->reset(m_model->tasks()); emit statsChanged(); }
