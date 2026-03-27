#ifndef NOTEPRESENTER_H
#define NOTEPRESENTER_H

#include <QObject>
#include "notemodel.h"
#include "noteview.h"

// ============================================================
// NotePresenter — PRESENTER layer (MVP)
//
// Wires NoteModel ↔ NoteView.
// Decides what happens when user creates/edits/deletes a note.
// ============================================================
class NotePresenter : public QObject
{
    Q_OBJECT

public:
    explicit NotePresenter(NoteModel* model,
                           NoteView*  view,
                           QObject*   parent = nullptr);

private slots:
    // From View
    void onNewNoteRequested();
    void onSaveNoteRequested(const QString& title, const QString& body);
    void onDeleteNoteRequested(int id);
    void onNoteSelected(int index);
    void onSearchChanged(const QString& query);

    // From Model
    void onNoteAdded(int index, const Note& note);
    void onNoteUpdated(int index, const Note& note);
    void onNoteDeleted(int id);
    void onNotesReloaded();
    void onErrorOccurred(const QString& error);

private:
    NoteModel* m_model;
    NoteView*  m_view;
    bool       m_isNewNote = false;
    int        m_currentId = -1;
};

#endif // NOTEPRESENTER_H
