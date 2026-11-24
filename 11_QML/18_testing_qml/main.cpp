// ============================================================
// 18_testing_qml
// Testing QML: Qt Quick Test (TestCase), qmllint and qmlformat. The tests are in the folder tests/ and run with ctest.
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
    engine.loadFromModule("testing_qml", "Main");

    return app.exec();
}
