#include <QCoreApplication>
#include <QTextStream>
#include "udpreceiver.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QTextStream out(stdout);
    out << "========================================\n";
    out << "  Qt UDP Receiver — networking demo\n";
    out << "========================================\n";
    out.flush();

    UdpReceiver receiver(5000, &app);
    if (!receiver.start()) return 1;

    return app.exec();
}
