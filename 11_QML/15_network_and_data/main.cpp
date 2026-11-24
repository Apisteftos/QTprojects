// ============================================================
// 15_network_and_data
// A C++ backend that loads JSON (from a resource or over HTTP) and feeds a QML ListView. The interface is in Main.qml, the backend is in stationclient.h/.cpp.
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
    engine.loadFromModule("network_and_data", "Main");

    return app.exec();
}
