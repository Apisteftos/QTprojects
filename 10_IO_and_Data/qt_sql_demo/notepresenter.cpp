#include "notepresenter.h"
#include <QMessageBox>

NotePresenter::NotePresenter(NoteModel* model, NoteView* view, QObject* parent)
    : QObject(parent), m_model(model), m_view(view),
      m_isNewNote(false), m_currentId(-1)
{
    // View → Presenter
    connect(m_view, &NoteView::newNoteRequested,
            this,   &NotePresenter::onNewNoteRequested);
    connect(m_view, &NoteView::saveNoteRequested,
            this,   &NotePresenter::onSaveNoteRequested);
    connect(m_view, &NoteView::deleteNoteRequested,
            this,   &NotePresenter::onDeleteNoteRequested);
    connect(m_view, &NoteView::noteSelected,
            this,   &NotePresenter::onNoteSelected);
    connect(m_view, &NoteView::searchChanged,
            this,   &NotePresenter::onSearchChanged);

    // Model → Presenter
    connect(m_model, &NoteModel::noteAdded,
            this,    &NotePresenter::onNoteAdded);
    connect(m_model, &NoteModel::noteUpdated,
            this,    &NotePresenter::onNoteUpdated);
    connect(m_model, &NoteModel::noteDeleted,
            this,    &NotePresenter::onNoteDeleted);
    connect(m_model, &NoteModel::notesReloaded,
            this,    &NotePresenter::onNotesReloaded);
    connect(m_model, &NoteModel::errorOccurred,
            this,    &NotePresenter::onErrorOccurred);

    // Initial load from database
    m_model->load();
}

void NotePresenter::onNewNoteRequested() {
    m_isNewNote = true;
    m_currentId = -1;
    m_view->clearEditor();
    m_view->setEditorEnabled(true);
}

void NotePresenter::onSaveNoteRequested(const QString& title, const QString& body) {
    if (title.trimmed().isEmpty()) {
        QMessageBox::warning(m_view, "Validation", "Title cannot be empty.");
        return;
    }
    if (m_isNewNote) {
        m_model->addNote(title, body);
        m_isNewNote = false;
    } else if (m_currentId >= 0) {
        m_model->updateNote(m_currentId, title, body);
    }
}

void NotePresenter::onDeleteNoteRequested(int id) {
    auto btn = QMessageBox::question(m_view, "Delete Note",
        "Are you sure you want to delete this note?");
    if (btn == QMessageBox::Yes) {
        m_currentId = -1;
        m_model->deleteNote(id);
    }
}

void NotePresenter::onNoteSelected(int index) {
    m_isNewNote = false;
    Note note = m_model->noteAt(index);
    if (note.id > 0) {
        m_currentId = note.id;
        m_view->showNote(note);
    }
}

void NotePresenter::onSearchChanged(const QString& query) {
    if (query.isEmpty())
        m_model->clearSearch();
    else
        m_model->search(query);
}

void NotePresenter::onNoteAdded(int index, const Note& note) {
    m_currentId = note.id;
    m_view->addNoteToList(note);
    m_view->showNote(note);
    m_view->updateStatusBar(m_model->count());
    Q_UNUSED(index)
}

void NotePresenter::onNoteUpdated(int index, const Note& note) {
    m_view->updateNoteInList(index, note);
    m_view->updateStatusBar(m_model->count());
}

void NotePresenter::onNoteDeleted(int id) {
    m_view->removeNoteFromList(id);
    m_view->updateStatusBar(m_model->count());
    // Show first note if any remain
    if (m_model->count() > 0) {
        m_currentId = m_model->noteAt(0).id;
        m_view->showNote(m_model->noteAt(0));
    }
}

void NotePresenter::onNotesReloaded() {
    m_view->populateList(m_model->notes());
    m_view->updateStatusBar(m_model->count());
    if (m_model->count() > 0) {
        m_currentId = m_model->noteAt(0).id;
        m_view->showNote(m_model->noteAt(0));
    }
}

void NotePresenter::onErrorOccurred(const QString& error) {
    QMessageBox::critical(m_view, "Database Error", error);
}
