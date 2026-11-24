// ============================================================
// 10_models_and_views
// Models and views: ListModel, ListView, Repeater and delegates. All interface code is in Main.qml.
// ============================================================

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("models_and_views", "Main");

    return app.exec();
}
