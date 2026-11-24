#ifndef SCOREMODEL_H
#define SCOREMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QtQml/qqmlregistration.h>

#include <vector>

// === THE MODEL ===
// The data (players and their points) and the rules for them. It is a Qt model, so a view can
// show it directly and is told about every change by signals. It knows no controller and no view.
class ScoreModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("The model is created in main.cpp and given to QML")

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int total READ total NOTIFY totalChanged)

public:
    enum Role { NameRole = Qt::UserRole + 1, PointsRole, LeadingRole };

    // What happened to a request. The model reports WHAT, the controller decides what to tell the user.
    enum class Result { Ok, EmptyName, DuplicateName, NoSuchRow, NoPointsLeft };

    explicit ScoreModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const { return static_cast<int>(m_players.size()); }
    int total() const;

    Result addPlayer(const QString &name);
    Result addPoints(int row, int delta);     // delta can be negative; the points never go below 0
    Result removeAt(int row);
    void newRound();                          // all points back to 0, the players stay

signals:
    void countChanged();
    void totalChanged();

private:
    struct Player
    {
        QString name;
        int points = 0;
    };

    int highestPoints() const;
    void pointsChangedEverywhere();

    std::vector<Player> m_players;
};

#endif // SCOREMODEL_H
