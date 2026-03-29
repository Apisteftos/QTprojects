#ifndef TEST_NOTEREPOSITORY_H
#define TEST_NOTEREPOSITORY_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QSqlDatabase>
#include <QTemporaryFile>
#include <QDir>
#include "noterepository.h"

// ============================================================
// TestNoteRepository — unit tests for NoteRepository
//
// KEY CONCEPT: Unit tests must be ISOLATED.
// Each test gets a fresh in-memory database — no leftover
// data from previous tests, no dependency on disk files.
//
// We use ":memory:" as the database path — SQLite's special
// in-memory database. Fast, isolated, auto-destroyed.
//
// QTest structure:
//   private slots beginning with "test" → test cases
//   initTestCase()  → runs ONCE before all tests
//   cleanupTestCase() → runs ONCE after all tests
//   init()          → runs before EACH test
//   cleanup()       → runs after EACH test
//
// Test macros:
//   QVERIFY(condition)          → fails if false
//   QCOMPARE(actual, expected)  → fails if not equal
//   QVERIFY_THROWS_EXCEPTION    → fails if no exception
//   QFAIL("message")            → always fails
// ============================================================
class TestNoteRepository : public QObject
{
    Q_OBJECT

private slots:
    // ── Lifecycle ─────────────────────────────────────────────
    void initTestCase() {
        // Runs once before all tests
        qDebug() << "=== TestNoteRepository starting ===";
    }

    void cleanupTestCase() {
        qDebug() << "=== TestNoteRepository done ===";
    }

    void init() {
        // Runs before EACH test — fresh in-memory database
        // ":memory:" = SQLite in-memory DB, no file on disk
        m_repo = new NoteRepository(":memory:");
        bool opened = m_repo->open();
        QVERIFY2(opened, "Database should open successfully");
    }

    void cleanup() {
        // Runs after EACH test — destroy repo
        delete m_repo;
        m_repo = nullptr;

        // Remove the named connection to avoid Qt warnings
        QSqlDatabase::removeDatabase("notes_connection");
    }

    // ── Test cases ────────────────────────────────────────────

    void test_openDatabase() {
        QVERIFY(m_repo->isOpen());
    }

    void test_initiallyEmpty() {
        QCOMPARE(m_repo->noteCount(), 0);
        QVERIFY(m_repo->getAllNotes().isEmpty());
    }

    void test_createNote_basic() {
        bool ok = m_repo->createNote("Test Title", "Test Body");
        QVERIFY2(ok, "createNote should return true");
        QCOMPARE(m_repo->noteCount(), 1);
    }

    void test_createNote_emptyTitleFails() {
        bool ok = m_repo->createNote("", "Some body");
        QVERIFY2(!ok, "Empty title should fail");
        QCOMPARE(m_repo->noteCount(), 0);
    }

    void test_createNote_whitespaceOnlyTitleFails() {
        bool ok = m_repo->createNote("   ", "Some body");
        QVERIFY2(!ok, "Whitespace-only title should fail");
    }

    void test_createNote_emptyBodyAllowed() {
        bool ok = m_repo->createNote("Title", "");
        QVERIFY2(ok, "Empty body should be allowed");
    }

    void test_createMultipleNotes() {
        m_repo->createNote("Note 1", "Body 1");
        m_repo->createNote("Note 2", "Body 2");
        m_repo->createNote("Note 3", "Body 3");
        QCOMPARE(m_repo->noteCount(), 3);
    }

    void test_getAllNotes_returnsCorrectData() {
        m_repo->createNote("Alpha", "Body A");
        m_repo->createNote("Beta",  "Body B");

        QList<Note> notes = m_repo->getAllNotes();
        QCOMPARE(notes.size(), 2);

        // getAllNotes returns newest first (ORDER BY updated_at DESC)
        // Both notes created at same time so order may vary
        // Just verify both exist with correct data
        // so Beta should be first
        // Verify both notes exist (order may vary at same timestamp)
        QStringList titles;
        for (const Note& n : notes) titles << n.title;
        QVERIFY(titles.contains("Alpha"));
        QVERIFY(titles.contains("Beta"));
        QVERIFY(notes[0].id > 0);
        QVERIFY(notes[0].createdAt.isValid());
        QVERIFY(notes[0].updatedAt.isValid());
    }

    void test_getNoteById_existing() {
        m_repo->createNote("FindMe", "Content");
        QList<Note> all = m_repo->getAllNotes();
        QVERIFY(!all.isEmpty());

        int id = all.first().id;
        Note found = m_repo->getNoteById(id);

        QCOMPARE(found.id,    id);
        QCOMPARE(found.title, QString("FindMe"));
        QCOMPARE(found.body,  QString("Content"));
    }

    void test_getNoteById_nonExistent() {
        Note note = m_repo->getNoteById(9999);
        QCOMPARE(note.id, 0); // default constructed Note has id = 0
    }

    void test_updateNote_changesContent() {
        m_repo->createNote("Original", "Old body");
        int id = m_repo->getAllNotes().first().id;

        bool ok = m_repo->updateNote(id, "Updated", "New body");
        QVERIFY2(ok, "updateNote should succeed");

        Note updated = m_repo->getNoteById(id);
        QCOMPARE(updated.title, QString("Updated"));
        QCOMPARE(updated.body,  QString("New body"));
    }

    void test_updateNote_updatedAtChanges() {
        m_repo->createNote("Note", "Body");
        int id = m_repo->getAllNotes().first().id;
        QDateTime before = m_repo->getNoteById(id).updatedAt;

        // Small delay to ensure timestamp differs
        QTest::qWait(1100);

        m_repo->updateNote(id, "Note", "Changed body");
        QDateTime after = m_repo->getNoteById(id).updatedAt;

        QVERIFY2(after > before, "updatedAt should be newer after update");
    }

    void test_deleteNote_removesFromDB() {
        m_repo->createNote("ToDelete", "body");
        int id = m_repo->getAllNotes().first().id;
        QCOMPARE(m_repo->noteCount(), 1);

        bool ok = m_repo->deleteNote(id);
        QVERIFY2(ok, "deleteNote should succeed");
        QCOMPARE(m_repo->noteCount(), 0);
    }

    void test_deleteNote_nonExistentIsOk() {
        // Deleting non-existent id should not crash
        bool ok = m_repo->deleteNote(9999);
        QVERIFY(ok); // DELETE WHERE id=9999 affects 0 rows but doesn't error
    }

    void test_noteCount_accurate() {
        QCOMPARE(m_repo->noteCount(), 0);
        m_repo->createNote("One", "");
        QCOMPARE(m_repo->noteCount(), 1);
        m_repo->createNote("Two", "");
        QCOMPARE(m_repo->noteCount(), 2);
        int id = m_repo->getAllNotes().last().id;
        m_repo->deleteNote(id);
        QCOMPARE(m_repo->noteCount(), 1);
    }

    void test_signals_noteCreatedEmitted() {
        // QSignalSpy — captures signals for testing
        QSignalSpy spy(m_repo, &NoteRepository::noteCreated);
        m_repo->createNote("Signal Test", "body");

        QCOMPARE(spy.count(), 1);
        // The signal argument is a Note struct
        // Signals with custom types need Q_DECLARE_METATYPE
    }

    void test_signals_noteDeletedEmitted() {
        m_repo->createNote("ToDelete", "body");
        int id = m_repo->getAllNotes().first().id;

        QSignalSpy spy(m_repo, &NoteRepository::noteDeleted);
        m_repo->deleteNote(id);

        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.first().first().toInt(), id);
    }

    void test_signals_errorEmittedOnEmptyTitle() {
        QSignalSpy spy(m_repo, &NoteRepository::errorOccurred);
        m_repo->createNote("", "body");
        QCOMPARE(spy.count(), 1);
    }

private:
    NoteRepository* m_repo = nullptr;
};

#endif // TEST_NOTEREPOSITORY_H
