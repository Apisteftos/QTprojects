// ============================================================
// 01_Setup_and_CMake
// Minimal Qt6 console program that checks that Qt, CMake and
// the C++23 compiler work together.
// ============================================================

#include <QCoreApplication>
#include <QString>
#include <QStringList>

#include <print>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // === EXAMPLE 1: Qt version ===
    std::println("Qt runtime version:      {}", qVersion());
    std::println("Qt compile-time version: {}", QT_VERSION_STR);

    std::println("");

    // === EXAMPLE 2: QString to std::string ===
    const QString greeting = QStringLiteral("Hello from Qt");
    std::println("{}", greeting.toStdString());

    std::println("");

    // === EXAMPLE 3: Command line arguments ===
    const QStringList arguments = app.arguments();
    std::println("Argument count: {}", arguments.size());
    for (const QString& argument : arguments)
    {
        std::println("  {}", argument.toStdString());
    }

    return 0;
}
