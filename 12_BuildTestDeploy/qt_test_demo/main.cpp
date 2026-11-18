#include <QTest>
#include <QApplication>
#include "test_noterepository.h"
#include "test_notemodel.h"

// ============================================================
// Test runner — runs all test suites and reports results
//
// QTest::qExec() runs one test class and returns:
//   0 = all tests passed
//   >0 = number of failures
//
// We accumulate results and return the total failure count
// so CI systems (like GitHub Actions) can detect failures.
// ============================================================
int main(int argc, char* argv[])
{
    // QApplication needed because NoteView uses widgets
    // For pure model/repo tests QCoreApplication would suffice
    QApplication app(argc, argv);

    int failures = 0;

    TestNoteRepository repoTests;
    failures += QTest::qExec(&repoTests, argc, argv);

    TestNoteModel modelTests;
    failures += QTest::qExec(&modelTests, argc, argv);

    return failures;
}
