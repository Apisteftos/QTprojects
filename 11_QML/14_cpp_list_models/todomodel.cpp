#include "todomodel.h"

TodoModel::TodoModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_items = {
        {"Write the report", false},
        {"Buy coffee", true},
        {"Call the dentist", false},
    };
}

int TodoModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : count();   // a list has no children: only the invisible root has rows
}

QVariant TodoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= count())
        return {};

    const Item &item = m_items.at(index.row());
    switch (role) {
    case TitleRole:
        return item.title;
    case DoneRole:
        return item.done;
    }
    return {};
}

QHash<int, QByteArray> TodoModel::roleNames() const
{
    // These names become the "required property" names in a QML delegate
    return {
        {TitleRole, "title"},
        {DoneRole, "done"},
    };
}

bool TodoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= count())
        return false;

    Item &item = m_items[index.row()];
    if (role == DoneRole && item.done != value.toBool()) {
        item.done = value.toBool();
        emit dataChanged(index, index, {DoneRole});   // tell the views which cell changed
        emit remainingChanged();
        return true;
    }
    if (role == TitleRole && item.title != value.toString()) {
        item.title = value.toString();
        emit dataChanged(index, index, {TitleRole});
        return true;
    }
    return false;
}

Qt::ItemFlags TodoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractListModel::flags(index);   // the invisible root item is not editable
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

int TodoModel::remaining() const
{
    int n = 0;
    for (const Item &item : m_items)
        n += item.done ? 0 : 1;
    return n;
}

void TodoModel::add(const QString &title)
{
    const QString trimmed = title.trimmed();
    if (trimmed.isEmpty())
        return;

    // Always: begin..., change the data, end...  The views get exactly the right signals.
    beginInsertRows(QModelIndex(), count(), count());
    m_items.append({trimmed, false});
    endInsertRows();
    emit countChanged();
    emit remainingChanged();
}

void TodoModel::remove(int row)
{
    if (row < 0 || row >= count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    emit countChanged();
    emit remainingChanged();
}

void TodoModel::setDone(int row, bool done)
{
    // The same as the write from a view, but with a clear name for QML
    setData(index(row), done, DoneRole);
}

void TodoModel::clearDone()
{
    // From the end to the start, so that the row numbers stay valid
    for (int row = count() - 1; row >= 0; --row) {
        if (m_items.at(row).done)
            remove(row);
    }
}
