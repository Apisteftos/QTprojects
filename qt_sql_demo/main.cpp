#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("qt_sql_demo");
    app.setOrganizationName("QtTutorial");

    // Ensure app data directory exists
    QString dataDir = QStandardPaths::writableLocation(
                          QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);

    MainWindow window;
    window.show();
    return app.exec();
}
