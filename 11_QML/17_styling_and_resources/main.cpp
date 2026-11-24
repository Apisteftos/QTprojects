// ============================================================
// 17_styling_and_resources
// Styling: Material and Universal styles, a Theme singleton, fonts, images and resource files. The style is chosen in qtquickcontrols2.conf (a resource).
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
    engine.loadFromModule("styling_and_resources", "Main");

    return app.exec();
}
