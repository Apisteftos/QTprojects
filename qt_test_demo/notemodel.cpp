#include "notemodel.h"

NoteModel::NoteModel(NoteRepository* repo, QObject* parent)
    : QObject(parent), m_repo(repo)
{
    connect(m_repo, &NoteRepository::noteCreated,
            this,   &NoteModel::onNoteCreated);
    connect(m_repo, &NoteRepository::noteUpdated,
            this,   &NoteModel::onNoteUpdated);
    connect(m_repo, &NoteRepository::noteDeleted,
            this,   &NoteModel::onNoteDeleted);
    connect(m_repo, &NoteRepository::errorOccurred,
            this,   &NoteModel::errorOccurred);
}

bool NoteModel::load() {
    m_notes    = m_repo->getAllNotes();
    m_allNotes = m_notes;
    emit notesReloaded();
    return true;
}

Note NoteModel::noteAt(int index) const {
    if (index >= 0 && index < m_notes.size())
        return m_notes[index];
    return Note{};
}

int NoteModel::indexById(int id) const {
    for (int i = 0; i < m_notes.size(); i++)
        if (m_notes[i].id == id) return i;
    return -1;
}

void NoteModel::addNote(const QString& title, const QString& body) {
    m_repo->createNote(title, body);
}

void NoteModel::updateNote(int id, const QString& title, const QString& body) {
    m_repo->updateNote(id, title, body);
}

void NoteModel::deleteNote(int id) {
    m_repo->deleteNote(id);
}

void NoteModel::search(const QString& query) {
    if (query.trimmed().isEmpty()) {
        m_notes = m_allNotes;
        emit notesReloaded();
        return;
    }
    m_notes.clear();
    for (const Note& n : m_allNotes) {
        if (n.title.contains(query, Qt::CaseInsensitive) ||
            n.body.contains(query,  Qt::CaseInsensitive)) {
            m_notes.append(n);
        }
    }
    emit notesReloaded();
}

void NoteModel::clearSearch() {
    m_notes = m_allNotes;
    emit notesReloaded();
}

void NoteModel::onNoteCreated(const Note& note) {
    m_notes.prepend(note);
    m_allNotes.prepend(note);
    emit noteAdded(0, note);
}

void NoteModel::onNoteUpdated(const Note& note) {
    int idx = indexById(note.id);
    if (idx >= 0) {
        m_notes[idx] = note;
        // Move to top (updated_at changes)
        m_notes.move(idx, 0);
        // Sync allNotes
        for (int i = 0; i < m_allNotes.size(); i++) {
            if (m_allNotes[i].id == note.id) {
                m_allNotes[i] = note;
                m_allNotes.move(i, 0);
                break;
            }
        }
        emit noteUpdated(0, note);
    }
}

void NoteModel::onNoteDeleted(int id) {
    int idx = indexById(id);
    if (idx >= 0) m_notes.removeAt(idx);
    for (int i = 0; i < m_allNotes.size(); i++) {
        if (m_allNotes[i].id == id) { m_allNotes.removeAt(i); break; }
    }
    emit noteDeleted(id);
}
