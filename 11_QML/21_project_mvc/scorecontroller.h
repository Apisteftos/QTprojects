#ifndef SCORECONTROLLER_H
#define SCORECONTROLLER_H

#include "scoremodel.h"

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

// === THE CONTROLLER ===
// It receives the INPUT of the user (what the view reports: a click, a typed name), decides what
// it means, and changes the model. It holds no data. It does not show anything and knows no QML item.
// Problems are reported with a signal; the view decides how to show them.
class ScoreController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("The controller is created in main.cpp and given to QML")

public:
    explicit ScoreController(ScoreModel &model, QObject *parent = nullptr);

    Q_INVOKABLE bool addPlayer(const QString &name);     // true when the player was added
    Q_INVOKABLE void givePoint(int row);
    Q_INVOKABLE void takePoint(int row);
    Q_INVOKABLE void removePlayer(int row);
    Q_INVOKABLE void newRound();

signals:
    void rejected(const QString &reason);

private:
    bool report(ScoreModel::Result result);              // true for Ok; otherwise sends "rejected"

    ScoreModel &m_model;
};

#endif // SCORECONTROLLER_H
