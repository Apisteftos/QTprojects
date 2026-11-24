// ============================================================
// 09_animations
// Animations: NumberAnimation, ColorAnimation, SequentialAnimation, ParallelAnimation and easing. All interface code is in Main.qml.
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
    engine.loadFromModule("animations", "Main");

    return app.exec();
}
