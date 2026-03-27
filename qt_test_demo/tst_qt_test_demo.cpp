#include <QCoreApplication>
#include <QtTest>

// add necessary includes here

class qt_test_demo : public QObject
{
    Q_OBJECT

public:
    qt_test_demo();
    ~qt_test_demo();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

qt_test_demo::qt_test_demo() {}

qt_test_demo::~qt_test_demo() {}

void qt_test_demo::initTestCase() {}

void qt_test_demo::cleanupTestCase() {}

void qt_test_demo::test_case1() {}

QTEST_MAIN(qt_test_demo)

#include "tst_qt_test_demo.moc"
