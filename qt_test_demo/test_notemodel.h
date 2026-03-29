#ifndef TEST_NOTEMODEL_H
#define TEST_NOTEMODEL_H

#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QSqlDatabase>
#include "noterepository.h"
#include "notemodel.h"

// ============================================================
// TestNoteModel — unit tests for NoteModel
//
// NoteModel depends on NoteRepository.
// We give it a real in-memory repository — this is an
// INTEGRATION TEST at the Model+Repository level.
//
// We do NOT mock the repository here because:
//   - SQLite in-memory is fast enough
//   - Mocking in C++ requires extra infrastructure
//   - The real repo gives us more confidence
//
// Key things we test:
//   - load() populates the model from the repo
//   - addNote / updateNote / deleteNote modify state
//   - search() filters correctly
//   - signals fire correctly
//   - count() is accurate
// ============================================================
class TestNoteModel : public QObject
{
    Q_OBJECT

private slots:
    void init() {
        m_repo  = new NoteRepository(":memory:");
        m_repo->open();
        m_model = new NoteModel(m_repo);
    }

    void cleanup() {
        delete m_model;
        delete m_repo;
        m_model = nullptr;
        m_repo  = nullptr;
        QSqlDatabase::removeDatabase("notes_connection");
    }

    // ── Tests ─────────────────────────────────────────────────

    void test_initiallyEmpty() {
        m_model->load();
        QCOMPARE(m_model->count(), 0);
        QVERIFY(m_model->notes().isEmpty());
    }

    void test_load_populatesFromRepo() {
        // Add directly to repo, then load model
        m_repo->createNote("Note A", "Body A");
        m_repo->createNote("Note B", "Body B");

        m_model->load();
        QCOMPARE(m_model->count(), 2);
    }

    void test_addNote_increasesCount() {
        m_model->load();
        QCOMPARE(m_model->count(), 0);

        m_model->addNote("New Note", "Body");
        QCOMPARE(m_model->count(), 1);
    }

    void test_addNote_emitsNoteAdded() {
        m_model->load();
        QSignalSpy spy(m_model, &NoteModel::noteAdded);

        m_model->addNote("Test", "Body");

        QCOMPARE(spy.count(), 1);
    }

    void test_addNote_emptyTitleEmitsError() {
        m_model->load();
        QSignalSpy errorSpy(m_model, &NoteModel::errorOccurred);
        QSignalSpy addSpy(m_model,   &NoteModel::noteAdded);

        m_model->addNote("", "Body");

        QCOMPARE(addSpy.count(),   0); // no note added
        QCOMPARE(errorSpy.count(), 1); // error emitted
    }

    void test_noteAt_returnsCorrectNote() {
        m_model->load();
        m_model->addNote("First",  "Body 1");
        m_model->addNote("Second", "Body 2");

        // Most recently added is at index 0 (prepend)
        Note first = m_model->noteAt(0);
        QCOMPARE(first.title, QString("Second"));
    }

    void test_noteAt_invalidIndexReturnsEmpty() {
        m_model->load();
        Note n = m_model->noteAt(999);
        QCOMPARE(n.id, 0);
    }

    void test_deleteNote_decreasesCount() {
        m_model->load();
        m_model->addNote("ToDelete", "body");
        QCOMPARE(m_model->count(), 1);

        int id = m_model->noteAt(0).id;
        m_model->deleteNote(id);
        QCOMPARE(m_model->count(), 0);
    }

    void test_deleteNote_emitsNoteDeleted() {
        m_model->load();
        m_model->addNote("Note", "body");
        int id = m_model->noteAt(0).id;

        QSignalSpy spy(m_model, &NoteModel::noteDeleted);
        m_model->deleteNote(id);

        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.first().first().toInt(), id);
    }

    void test_updateNote_changesContent() {
        m_model->load();
        m_model->addNote("Original", "Old");
        int id = m_model->noteAt(0).id;

        m_model->updateNote(id, "Updated", "New");

        Note n = m_model->noteAt(0);
        QCOMPARE(n.title, QString("Updated"));
        QCOMPARE(n.body,  QString("New"));
    }

    void test_updateNote_emitsNoteUpdated() {
        m_model->load();
        m_model->addNote("Note", "Body");
        int id = m_model->noteAt(0).id;

        QSignalSpy spy(m_model, &NoteModel::noteUpdated);
        m_model->updateNote(id, "Changed", "Body");

        QCOMPARE(spy.count(), 1);
    }

    void test_search_findsMatchingTitle() {
        m_model->load();
        m_model->addNote("Qt Tutorial",  "body");
        m_model->addNote("CMake Guide",  "body");
        m_model->addNote("Qt Networking","body");

        m_model->search("Qt");
        QCOMPARE(m_model->count(), 2);
    }

    void test_search_findsMatchingBody() {
        m_model->load();
        m_model->addNote("Note 1", "contains keyword here");
        m_model->addNote("Note 2", "nothing special");

        m_model->search("keyword");
        QCOMPARE(m_model->count(), 1);
        QCOMPARE(m_model->noteAt(0).title, QString("Note 1"));
    }

    void test_search_caseInsensitive() {
        m_model->load();
        m_model->addNote("Qt Framework", "body");

        m_model->search("qt");        // lowercase
        QCOMPARE(m_model->count(), 1);
        m_model->clearSearch();

        m_model->search("QT");        // uppercase
        QCOMPARE(m_model->count(), 1);
    }

    void test_search_noMatchReturnsEmpty() {
        m_model->load();
        m_model->addNote("Qt Tutorial", "body");

        m_model->search("Python");
        QCOMPARE(m_model->count(), 0);
    }

    void test_clearSearch_restoresAll() {
        m_model->load();
        m_model->addNote("Qt Tutorial",  "body");
        m_model->addNote("CMake Guide",  "body");
        m_model->addNote("Qt Networking","body");

        m_model->search("Qt");
        QCOMPARE(m_model->count(), 2);

        m_model->clearSearch();
        QCOMPARE(m_model->count(), 3); // all restored
    }

    void test_indexById_found() {
        m_model->load();
        m_model->addNote("Note", "body");
        int id  = m_model->noteAt(0).id;
        int idx = m_model->indexById(id);
        QCOMPARE(idx, 0);
    }

    void test_indexById_notFound() {
        m_model->load();
        int idx = m_model->indexById(9999);
        QCOMPARE(idx, -1);
    }

private:
    NoteRepository* m_repo  = nullptr;
    NoteModel*      m_model = nullptr;
};

#endif // TEST_NOTEMODEL_H
