#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "taskmodel.h"
#include "taskviewmodel.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    // Create the Model and ViewModel
    TaskModel*     model     = new TaskModel(&app);
    TaskViewModel* viewModel = new TaskViewModel(model, &app);

    QQmlApplicationEngine engine;

    // Expose ViewModel to QML under the name "viewModel"
    // QML accesses it as: viewModel.tasks, viewModel.addTask(...)
    engine.rootContext()->setContextProperty("viewModel", viewModel);

    // Load the QML file
    const QUrl url(u"qrc:/qt_mvvm_demo/Main.qml"_qs);
    engine.load(url);

    return app.exec();
}
