#ifndef NOTEMODEL_H
#define NOTEMODEL_H

#include <QObject>
#include <QList>
#include "noterepository.h"

// ============================================================
// NoteModel — MODEL layer
//
// Sits between NoteRepository (SQL) and the Presenter.
// Owns the in-memory list of notes.
// Delegates persistence to NoteRepository.
//
// Responsibilities:
//   - Caches notes in memory (avoids hitting DB on every render)
//   - Provides business logic (validation, filtering)
//   - Emits signals when data changes
//   - Knows nothing about UI
// ============================================================
class NoteModel : public QObject
{
    Q_OBJECT

public:
    explicit NoteModel(NoteRepository* repo, QObject* parent = nullptr);

    bool        load();
    QList<Note> notes()      const { return m_notes; }
    int         count()      const { return m_notes.size(); }
    Note        noteAt(int index) const;
    int         indexById(int id) const;

public slots:
    void addNote(const QString& title, const QString& body);
    void updateNote(int id, const QString& title, const QString& body);
    void deleteNote(int id);
    void search(const QString& query);
    void clearSearch();

signals:
    void noteAdded(int index, const Note& note);
    void noteUpdated(int index, const Note& note);
    void noteDeleted(int id);
    void notesReloaded();
    void errorOccurred(const QString& error);

private slots:
    void onNoteCreated(const Note& note);
    void onNoteUpdated(const Note& note);
    void onNoteDeleted(int id);

private:
    NoteRepository* m_repo;
    QList<Note>     m_notes;
    QList<Note>     m_allNotes; // full list for search reset
};

#endif // NOTEMODEL_H
