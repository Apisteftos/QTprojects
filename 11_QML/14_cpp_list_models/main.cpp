// ============================================================
// 14_cpp_list_models
// A C++ list model (QAbstractListModel with roles) shown in a QML ListView: a to-do list. The interface is in Main.qml, the model is in todomodel.h/.cpp.
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
    engine.loadFromModule("cpp_list_models", "Main");

    return app.exec();
}
