// ============================================================
// 08_states_and_transitions
// States, transitions and behaviors: an expanding panel and a lamp that follows a slider. All interface code is in Main.qml.
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
    engine.loadFromModule("states_and_transitions", "Main");

    return app.exec();
}
