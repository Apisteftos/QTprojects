#include "scoremodel.h"

#include <algorithm>

ScoreModel::ScoreModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ScoreModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : count();
}

int ScoreModel::highestPoints() const
{
    int highest = 0;
    for (const Player &player : m_players)
        highest = std::max(highest, player.points);
    return highest;
}

int ScoreModel::total() const
{
    int sum = 0;
    for (const Player &player : m_players)
        sum += player.points;
    return sum;
}

QVariant ScoreModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid))
        return {};

    const Player &player = m_players[static_cast<std::size_t>(index.row())];
    switch (role) {
    case NameRole:
        return player.name;
    case PointsRole:
        return player.points;
    case LeadingRole:
        // Everybody with the highest points leads, but not at 0 points (nobody has scored yet)
        return player.points > 0 && player.points == highestPoints();
    default:
        return {};
    }
}

QHash<int, QByteArray> ScoreModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {PointsRole, "points"},
        {LeadingRole, "leading"},
    };
}

ScoreModel::Result ScoreModel::addPlayer(const QString &name)
{
    const QString trimmed = name.trimmed();
    if (trimmed.isEmpty())
        return Result::EmptyName;

    const bool exists = std::any_of(m_players.begin(), m_players.end(), [&](const Player &player) {
        return player.name.compare(trimmed, Qt::CaseInsensitive) == 0;
    });
    if (exists)
        return Result::DuplicateName;

    const int row = count();
    beginInsertRows(QModelIndex(), row, row);
    m_players.push_back({trimmed, 0});
    endInsertRows();
    emit countChanged();
    return Result::Ok;
}

ScoreModel::Result ScoreModel::addPoints(int row, int delta)
{
    if (row < 0 || row >= count())
        return Result::NoSuchRow;

    Player &player = m_players[static_cast<std::size_t>(row)];
    if (player.points + delta < 0)
        return Result::NoPointsLeft;
    if (delta == 0)
        return Result::Ok;

    player.points += delta;
    pointsChangedEverywhere();
    return Result::Ok;
}

ScoreModel::Result ScoreModel::removeAt(int row)
{
    if (row < 0 || row >= count())
        return Result::NoSuchRow;

    const int totalBefore = total();
    beginRemoveRows(QModelIndex(), row, row);
    m_players.erase(m_players.begin() + row);
    endRemoveRows();
    emit countChanged();
    if (total() != totalBefore)
        emit totalChanged();
    // The leader may be gone: the others must be asked again
    if (count() > 0)
        emit dataChanged(index(0), index(count() - 1), {LeadingRole});
    return Result::Ok;
}

void ScoreModel::newRound()
{
    if (total() == 0)
        return;
    for (Player &player : m_players)
        player.points = 0;
    pointsChangedEverywhere();
}

void ScoreModel::pointsChangedEverywhere()
{
    // One player's points can change who is leading, so the roles of ALL rows are announced.
    if (count() > 0)
        emit dataChanged(index(0), index(count() - 1), {PointsRole, LeadingRole});
    emit totalChanged();
}
