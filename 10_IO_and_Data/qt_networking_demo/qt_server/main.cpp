#include <QCoreApplication>
#include <QTextStream>
#include "tcpserver.h"

// ============================================================
// QCoreApplication — headless event loop
//
// No QApplication (no GUI), no QGuiApplication (no window).
// QCoreApplication gives us the Qt event loop only —
// perfect for a background service / daemon.
//
// The event loop is what makes QTcpServer work asynchronously.
// Without it, no signals would fire and no connections would
// be accepted.
// ============================================================
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("qt_server");
    app.setApplicationVersion("1.0");

    QTextStream out(stdout);
    out << "========================================\n";
    out << "  Qt TCP Server — networking demo\n";
    out << "========================================\n";
    out.flush();

    TcpServer server(5000, &app);
    if (!server.start()) return 1;

    // app.exec() starts the event loop — blocks here
    // until the process is killed (Ctrl+C or SIGTERM)
    return app.exec();
}
