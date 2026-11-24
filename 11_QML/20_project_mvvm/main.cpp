// ============================================================
// 20_project_mvvm
// Project: MVVM. A plain C++ model, a ViewModel that QML binds to, and a QML view.
// main.cpp owns the ViewModel and hands it to QML.
// ============================================================

#include "bookingviewmodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // The ViewModel belongs to main(). It is declared BEFORE the engine,
    // so the engine (and all QML objects) is destroyed first and never sees a dead object.
    BookingViewModel viewModel;

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // QML gets the object as the value of a "required property" of Main.qml.
    engine.setInitialProperties({{"viewModel", QVariant::fromValue(&viewModel)}});
    engine.loadFromModule("project_mvvm", "Main");

    return app.exec();
}
