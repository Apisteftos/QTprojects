// ============================================================
// 11_javascript_in_qml
// JavaScript in QML: functions, a JavaScript library file, Component.onCompleted, and input validation. All interface code is in Main.qml.
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
    engine.loadFromModule("javascript_in_qml", "Main");

    return app.exec();
}
