// ============================================================
// 19_project_dashboard
// Project: an animated dashboard. A C++ backend (Dashboard) simulates measurements; QML shows cards, a bar chart and an alarm message with animations.
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
    engine.loadFromModule("project_dashboard", "Main");

    return app.exec();
}
