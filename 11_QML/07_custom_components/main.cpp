// ============================================================
// 07_custom_components
// Your own QML types in separate files: Card.qml, AppButton.qml, property alias and Component. The window is in Main.qml.
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
    engine.loadFromModule("custom_components", "Main");

    return app.exec();
}
