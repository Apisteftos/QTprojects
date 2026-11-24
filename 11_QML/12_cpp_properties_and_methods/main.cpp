// ============================================================
// 12_cpp_properties_and_methods
// A C++ class used from QML: Q_PROPERTY, Q_INVOKABLE and QML_ELEMENT. The interface is in Main.qml, the logic is in counter.h/.cpp.
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
    engine.loadFromModule("cpp_properties_and_methods", "Main");

    return app.exec();
}
