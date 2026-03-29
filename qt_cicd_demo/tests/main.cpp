#include <QTest>
#include "test_calculator.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    int failures = 0;
    TestCalculator calcTests;
    failures += QTest::qExec(&calcTests, argc, argv);
    return failures;
}
