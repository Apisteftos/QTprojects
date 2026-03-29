#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("qt_cicd_demo");
    MainWindow window;
    window.show();
    return app.exec();
}
