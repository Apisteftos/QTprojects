#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "dataprovider.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("qt_qml_dashboard");
    app.setOrganizationName("QtTutorial");

    // Create the C++ backend
    DataProvider provider;

    QQmlApplicationEngine engine;

    // Expose to QML — accessible as "dataProvider" in all QML files
    engine.rootContext()->setContextProperty("dataProvider", &provider);

    const QUrl url(u"qrc:/qt_qml_dashboard/Main.qml"_qs);
    engine.load(url);

    // Auto-start the data stream
    provider.start();

    return app.exec();
}
