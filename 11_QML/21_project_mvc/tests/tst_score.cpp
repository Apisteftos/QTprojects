// ============================================================
// tst_score.cpp
// Tests of the Model and the Controller. No window, no QML.
// ============================================================

#include "scorecontroller.h"
#include "scoremodel.h"

#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QtTest>

Q_DECLARE_METATYPE(ScoreModel::Result)

class TstScore : public QObject
{
    Q_OBJECT

private slots:
    // --- Model ---
    void modelIsValid();
    void addPlayer_data();
    void addPlayer();
    void pointsNeverBelowZero();
    void leadingWithTies();
    void signalsOfTheModel();
    void removeChangesTheLeader();
    void newRoundKeepsThePlayers();

    // --- Controller ---
    void controllerChangesTheModel();
    void controllerReportsProblems();
};

static QString nameAt(const ScoreModel &model, int row)
{
    return model.data(model.index(row), ScoreModel::NameRole).toString();
}
static int pointsAt(const ScoreModel &model, int row)
{
    return model.data(model.index(row), ScoreModel::PointsRole).toInt();
}
static bool leadingAt(const ScoreModel &model, int row)
{
    return model.data(model.index(row), ScoreModel::LeadingRole).toBool();
}

void TstScore::modelIsValid()
{
    ScoreModel model;
    QAbstractItemModelTester tester(&model);       // checks the rules of a model while we use it
    model.addPlayer("Ana");
    model.addPlayer("Ben");
    model.addPoints(0, 2);
    model.removeAt(1);
    model.newRound();
    QCOMPARE(model.count(), 1);
}

void TstScore::addPlayer_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ScoreModel::Result>("expected");

    QTest::newRow("normal") << "Ana" << ScoreModel::Result::Ok;
    QTest::newRow("empty") << "" << ScoreModel::Result::EmptyName;
    QTest::newRow("only spaces") << "    " << ScoreModel::Result::EmptyName;
    QTest::newRow("spaces are cut") << "  Cy  " << ScoreModel::Result::Ok;
}

void TstScore::addPlayer()
{
    QFETCH(QString, name);
    QFETCH(ScoreModel::Result, expected);
    ScoreModel model;
    QCOMPARE(model.addPlayer(name), expected);
    QCOMPARE(model.count(), expected == ScoreModel::Result::Ok ? 1 : 0);
    if (expected == ScoreModel::Result::Ok)
        QCOMPARE(nameAt(model, 0), name.trimmed());
}

void TstScore::pointsNeverBelowZero()
{
    ScoreModel model;
    model.addPlayer("Ana");
    QCOMPARE(model.addPoints(0, -1), ScoreModel::Result::NoPointsLeft);
    QCOMPARE(pointsAt(model, 0), 0);
    QCOMPARE(model.addPoints(0, 3), ScoreModel::Result::Ok);
    QCOMPARE(model.addPoints(0, -3), ScoreModel::Result::Ok);
    QCOMPARE(pointsAt(model, 0), 0);
    QCOMPARE(model.addPoints(5, 1), ScoreModel::Result::NoSuchRow);
}

void TstScore::leadingWithTies()
{
    ScoreModel model;
    model.addPlayer("Ana");
    model.addPlayer("Ben");
    model.addPlayer("ana");                        // the same name, other letters: refused
    QCOMPARE(model.count(), 2);

    QVERIFY(!leadingAt(model, 0));                 // nobody has scored: nobody leads
    model.addPoints(0, 1);
    QVERIFY(leadingAt(model, 0));
    QVERIFY(!leadingAt(model, 1));
    model.addPoints(1, 1);                         // a tie: both lead
    QVERIFY(leadingAt(model, 0));
    QVERIFY(leadingAt(model, 1));
    model.addPoints(1, 1);
    QVERIFY(!leadingAt(model, 0));                 // the leader of Ana changed because of Ben's points
    QVERIFY(leadingAt(model, 1));
}

void TstScore::signalsOfTheModel()
{
    ScoreModel model;
    QSignalSpy inserted(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &ScoreModel::countChanged);
    QSignalSpy totalSpy(&model, &ScoreModel::totalChanged);
    QSignalSpy dataSpy(&model, &QAbstractItemModel::dataChanged);

    model.addPlayer("Ana");
    model.addPlayer("Ben");
    QCOMPARE(inserted.count(), 2);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(totalSpy.count(), 0);

    model.addPoints(0, 2);
    QCOMPARE(totalSpy.count(), 1);
    QCOMPARE(model.total(), 2);
    QCOMPARE(dataSpy.count(), 1);
    // the signal covers ALL rows, because the leader can change in every row
    const auto args = dataSpy.takeFirst();
    QCOMPARE(args.at(0).toModelIndex().row(), 0);
    QCOMPARE(args.at(1).toModelIndex().row(), 1);

    model.addPlayer("Ana");                        // refused: no signal
    QCOMPARE(countSpy.count(), 2);
    model.addPoints(0, 0);                         // nothing changes: no signal
    QCOMPARE(totalSpy.count(), 1);
}

void TstScore::removeChangesTheLeader()
{
    ScoreModel model;
    model.addPlayer("Ana");
    model.addPlayer("Ben");
    model.addPoints(0, 3);
    model.addPoints(1, 2);
    QVERIFY(!leadingAt(model, 1));
    QSignalSpy totalSpy(&model, &ScoreModel::totalChanged);
    model.removeAt(0);
    QCOMPARE(nameAt(model, 0), QString("Ben"));
    QVERIFY(leadingAt(model, 0));                  // Ben leads now
    QCOMPARE(model.total(), 2);
    QCOMPARE(totalSpy.count(), 1);
    QCOMPARE(model.removeAt(7), ScoreModel::Result::NoSuchRow);
}

void TstScore::newRoundKeepsThePlayers()
{
    ScoreModel model;
    model.addPlayer("Ana");
    model.addPlayer("Ben");
    model.addPoints(0, 4);
    model.addPoints(1, 1);
    model.newRound();
    QCOMPARE(model.count(), 2);
    QCOMPARE(model.total(), 0);
    QVERIFY(!leadingAt(model, 0));
}

void TstScore::controllerChangesTheModel()
{
    ScoreModel model;
    ScoreController controller(model);
    QSignalSpy rejected(&controller, &ScoreController::rejected);

    QVERIFY(controller.addPlayer("Ana"));
    QVERIFY(controller.addPlayer("Ben"));
    controller.givePoint(0);
    controller.givePoint(0);
    controller.takePoint(0);
    controller.givePoint(1);
    QCOMPARE(pointsAt(model, 0), 1);
    QCOMPARE(pointsAt(model, 1), 1);
    controller.removePlayer(0);
    QCOMPARE(model.count(), 1);
    controller.newRound();
    QCOMPARE(model.total(), 0);
    QCOMPARE(rejected.count(), 0);                 // all of this was fine
}

void TstScore::controllerReportsProblems()
{
    ScoreModel model;
    ScoreController controller(model);
    QSignalSpy rejected(&controller, &ScoreController::rejected);

    QVERIFY(!controller.addPlayer("  "));
    QCOMPARE(rejected.count(), 1);
    QCOMPARE(rejected.last().at(0).toString(), QString("Type a name first"));

    controller.addPlayer("Ana");
    QVERIFY(!controller.addPlayer("ANA"));
    QCOMPARE(rejected.last().at(0).toString(), QString("This name is already in the game"));

    controller.takePoint(0);
    QCOMPARE(rejected.last().at(0).toString(), QString("A player cannot have less than 0 points"));

    controller.removePlayer(9);
    QCOMPARE(rejected.last().at(0).toString(), QString("This player does not exist any more"));
    QCOMPARE(rejected.count(), 4);
    QCOMPARE(model.count(), 1);                    // the model is unchanged by all the refused requests
}

QTEST_MAIN(TstScore)
#include "tst_score.moc"
