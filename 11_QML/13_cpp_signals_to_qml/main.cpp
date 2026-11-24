// ============================================================
// 13_cpp_signals_to_qml
// C++ signals that QML reacts to, and a QML_SINGLETON: a timer-driven sensor. The interface is in Main.qml, the logic is in sensor.h/.cpp.
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
    engine.loadFromModule("cpp_signals_to_qml", "Main");

    return app.exec();
}
