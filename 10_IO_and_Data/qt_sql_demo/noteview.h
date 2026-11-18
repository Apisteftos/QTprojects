#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include "noterepository.h"

// ============================================================
// NoteView — VIEW layer (MVP)
//
// Classic two-panel notes UI:
//   Left  — list of notes + search
//   Right — editor (title + body)
// ============================================================
class NoteView : public QWidget
{
    Q_OBJECT

public:
    explicit NoteView(QWidget* parent = nullptr);

    // Presenter → View
    void populateList(const QList<Note>& notes);
    void addNoteToList(const Note& note);
    void updateNoteInList(int index, const Note& note);
    void removeNoteFromList(int id);
    void showNote(const Note& note);
    void clearEditor();
    void setEditorEnabled(bool enabled);
    void updateStatusBar(int total);

signals:
    // View → Presenter (user intentions)
    void newNoteRequested();
    void saveNoteRequested(const QString& title, const QString& body);
    void deleteNoteRequested(int id);
    void noteSelected(int index);
    void searchChanged(const QString& query);

private slots:
    void onListItemClicked(QListWidgetItem* item);
    void onSaveClicked();
    void onDeleteClicked();

private:
    QListWidget* m_noteList;
    QLineEdit*   m_searchInput;
    QPushButton* m_btnNew;
    QPushButton* m_btnDelete;
    QLineEdit*   m_titleEdit;
    QTextEdit*   m_bodyEdit;
    QPushButton* m_btnSave;
    QLabel*      m_statusLabel;
    QLabel*      m_timestampLabel;

    int          m_selectedId = -1;
};

#endif // NOTEVIEW_H
