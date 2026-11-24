// ============================================================
// 21_project_mvc
// Project: MVC. A Qt model holds the data, a controller handles the input,
// the QML view shows the model and sends input to the controller. main.cpp owns both objects.
// ============================================================

#include "scorecontroller.h"
#include "scoremodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Both objects belong to main() and are declared BEFORE the engine: the engine dies first.
    ScoreModel model;
    ScoreController controller(model);
    model.addPlayer("Ana");
    model.addPlayer("Ben");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.setInitialProperties({
        {"scores", QVariant::fromValue(&model)},
        {"controller", QVariant::fromValue(&controller)},
    });
    engine.loadFromModule("project_mvc", "Main");

    return app.exec();
}
