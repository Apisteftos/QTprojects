#include "taskmodel.h"

TaskModel::TaskModel(QObject* parent)
    : QObject(parent), m_nextId(1) {}

QList<Task> TaskModel::tasks()    const { return m_tasks; }
int         TaskModel::count()    const { return m_tasks.size(); }
int         TaskModel::doneCount() const {
    int n = 0;
    for (const Task& t : m_tasks) if (t.done) n++;
    return n;
}

void TaskModel::addTask(const QString& title) {
    if (title.trimmed().isEmpty()) return;
    Task t{ m_nextId++, title.trimmed(), false };
    m_tasks.append(t);
    emit taskAdded(t);
}

void TaskModel::removeTask(int id) {
    for (int i = 0; i < m_tasks.size(); i++) {
        if (m_tasks[i].id == id) {
            m_tasks.removeAt(i);
            emit taskRemoved(id);
            return;
        }
    }
}

void TaskModel::toggleDone(int id) {
    for (Task& t : m_tasks) {
        if (t.id == id) {
            t.done = !t.done;
            emit taskUpdated(t);
            return;
        }
    }
}

void TaskModel::clearAll() {
    m_tasks.clear();
    emit modelReset();
}
