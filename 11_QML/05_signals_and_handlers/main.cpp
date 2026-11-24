// ============================================================
// 05_signals_and_handlers
// Signals and handlers: onClicked, custom signals, Connections, TapHandler, Keys and Timer. All interface code is in Main.qml.
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
    engine.loadFromModule("signals_and_handlers", "Main");

    return app.exec();
}
