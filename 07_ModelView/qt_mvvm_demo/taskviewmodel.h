#ifndef TASKVIEWMODEL_H
#define TASKVIEWMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include "taskmodel.h"

// ============================================================
// TaskListModel — exposes Task list to QML via QAbstractListModel
//
// QML ListView needs a model that speaks Qt's model protocol.
// We implement roleNames() so QML can access properties by name:
//   model.title, model.done, model.taskId
// ============================================================
class TaskListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum TaskRoles {
        TitleRole  = Qt::UserRole + 1,
        DoneRole,
        IdRole
    };

    explicit TaskListModel(QObject* parent = nullptr);

    // QAbstractListModel interface
    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setTasks(const QList<Task>& tasks);
    void addTask(const Task& task);
    void removeTask(int id);
    void updateTask(const Task& task);
    void reset(const QList<Task>& tasks);

private:
    QList<Task> m_tasks;
};


// ============================================================
// TaskViewModel — the VIEWMODEL layer
//
// Responsibilities:
//   - Exposes data to QML via Q_PROPERTY
//   - Exposes commands to QML via Q_INVOKABLE
//   - Observes the Model and updates properties
//   - QML binds to properties automatically — no manual connect()
//     needed on the QML side
//
// Q_PROPERTY syntax:
//   Q_PROPERTY(type name READ getter NOTIFY signal)
//   - getter   → C++ method QML reads the value from
//   - NOTIFY   → signal QML watches for changes
//   When the NOTIFY signal fires, QML re-reads the property
//   and updates all bound UI elements automatically.
// ============================================================
class TaskViewModel : public QObject
{
    Q_OBJECT

    // Properties visible to QML
    Q_PROPERTY(TaskListModel* tasks     READ tasks     CONSTANT)
    Q_PROPERTY(int   totalCount         READ totalCount NOTIFY statsChanged)
    Q_PROPERTY(int   doneCount          READ doneCount  NOTIFY statsChanged)
    Q_PROPERTY(int   remainingCount     READ remainingCount NOTIFY statsChanged)
    Q_PROPERTY(QString statusText       READ statusText NOTIFY statsChanged)

public:
    explicit TaskViewModel(TaskModel* model, QObject* parent = nullptr);

    // Property getters
    TaskListModel* tasks()          const;
    int            totalCount()     const;
    int            doneCount()      const;
    int            remainingCount() const;
    QString        statusText()     const;

    // Q_INVOKABLE — callable from QML directly
    Q_INVOKABLE void addTask(const QString& title);
    Q_INVOKABLE void removeTask(int id);
    Q_INVOKABLE void toggleDone(int id);
    Q_INVOKABLE void clearAll();

signals:
    void statsChanged();  // Triggers re-evaluation of all stats properties

private slots:
    void onTaskAdded(const Task& task);
    void onTaskRemoved(int id);
    void onTaskUpdated(const Task& task);
    void onModelReset();

private:
    TaskModel*     m_model;
    TaskListModel* m_listModel;
};

#endif
