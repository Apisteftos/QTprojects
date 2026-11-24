#include "scorecontroller.h"

ScoreController::ScoreController(ScoreModel &model, QObject *parent)
    : QObject(parent)
    , m_model(model)
{
}

bool ScoreController::report(ScoreModel::Result result)
{
    switch (result) {
    case ScoreModel::Result::Ok:
        return true;
    case ScoreModel::Result::EmptyName:
        emit rejected(tr("Type a name first"));
        break;
    case ScoreModel::Result::DuplicateName:
        emit rejected(tr("This name is already in the game"));
        break;
    case ScoreModel::Result::NoSuchRow:
        emit rejected(tr("This player does not exist any more"));
        break;
    case ScoreModel::Result::NoPointsLeft:
        emit rejected(tr("A player cannot have less than 0 points"));
        break;
    }
    return false;
}

bool ScoreController::addPlayer(const QString &name)
{
    return report(m_model.addPlayer(name));
}

void ScoreController::givePoint(int row)
{
    report(m_model.addPoints(row, +1));
}

void ScoreController::takePoint(int row)
{
    report(m_model.addPoints(row, -1));
}

void ScoreController::removePlayer(int row)
{
    report(m_model.removeAt(row));
}

void ScoreController::newRound()
{
    m_model.newRound();
}
