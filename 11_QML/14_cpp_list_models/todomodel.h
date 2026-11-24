#ifndef TODOMODEL_H
#define TODOMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QtQml/qqmlregistration.h>

// A list of to-do items as a model for QML views (ListView, Repeater ...).
class TodoModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int remaining READ remaining NOTIFY remainingChanged)   // items that are not done

public:
    // The roles: the names under which the delegate sees the data of a row
    enum Roles {
        TitleRole = Qt::UserRole + 1,
        DoneRole,
    };

    explicit TodoModel(QObject *parent = nullptr);

    // The four functions every list model needs ...
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    // ... and these two make the model editable from the delegate
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int count() const { return int(m_items.size()); }
    int remaining() const;

    Q_INVOKABLE void add(const QString &title);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void setDone(int row, bool done);
    Q_INVOKABLE void clearDone();

signals:
    void countChanged();
    void remainingChanged();

private:
    struct Item
    {
        QString title;
        bool done = false;
    };
    QList<Item> m_items;
};

#endif // TODOMODEL_H
