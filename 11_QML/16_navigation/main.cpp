// ============================================================
// 16_navigation
// Navigation: StackView, SwipeView, TabBar, Drawer and Loader in a multi-page app. Main.qml holds the frame, the other .qml files are the pages.
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
    engine.loadFromModule("navigation", "Main");

    return app.exec();
}
