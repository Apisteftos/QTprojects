#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QObject>
#include <QList>
#include <QString>

struct Task {
    int     id;
    QString title;
    bool    done;
};

// MODEL — pure data, no UI knowledge
class TaskModel : public QObject
{
    Q_OBJECT

public:
    explicit TaskModel(QObject* parent = nullptr);

    QList<Task> tasks()    const;
    int         count()    const;
    int         doneCount() const;

public slots:
    void addTask(const QString& title);
    void removeTask(int id);
    void toggleDone(int id);
    void clearAll();

signals:
    void taskAdded(const Task& task);
    void taskRemoved(int id);
    void taskUpdated(const Task& task);
    void modelReset();

private:
    QList<Task> m_tasks;
    int         m_nextId;
};

#endif
