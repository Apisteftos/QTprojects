// ============================================================
// 02_items_and_properties
// Items, built-in and custom properties, id, parent, Rectangle,
// Text and Image. All the interface code is in Main.qml.
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
    engine.loadFromModule("items_and_properties", "Main");

    return app.exec();
}
