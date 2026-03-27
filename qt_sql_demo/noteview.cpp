#include "noteview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidgetItem>
#include <QFrame>

NoteView::NoteView(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* main = new QVBoxLayout(this);
    main->setContentsMargins(0, 0, 0, 0);
    main->setSpacing(0);

    // ── Splitter — left list / right editor ───────────────
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // ── LEFT PANEL ────────────────────────────────────────
    QWidget* leftPanel = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(8, 8, 8, 8);
    leftLayout->setSpacing(6);

    // Search
    m_searchInput = new QLineEdit(this);
    m_searchInput->setPlaceholderText("🔍 Search notes...");
    leftLayout->addWidget(m_searchInput);

    // Note list
    m_noteList = new QListWidget(this);
    m_noteList->setAlternatingRowColors(true);
    leftLayout->addWidget(m_noteList);

    // Buttons
    QHBoxLayout* btnRow = new QHBoxLayout();
    m_btnNew    = new QPushButton("+ New", this);
    m_btnDelete = new QPushButton("Delete", this);
    m_btnDelete->setEnabled(false);
    btnRow->addWidget(m_btnNew);
    btnRow->addWidget(m_btnDelete);
    leftLayout->addLayout(btnRow);

    splitter->addWidget(leftPanel);

    // ── RIGHT PANEL ───────────────────────────────────────
    QWidget* rightPanel = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(6);

    m_timestampLabel = new QLabel("", this);
    m_timestampLabel->setStyleSheet("color: #888; font-size: 11px;");
    rightLayout->addWidget(m_timestampLabel);

    m_titleEdit = new QLineEdit(this);
    m_titleEdit->setPlaceholderText("Note title...");
    m_titleEdit->setStyleSheet("font-size: 16px; font-weight: bold; padding: 4px;");
    rightLayout->addWidget(m_titleEdit);

    m_bodyEdit = new QTextEdit(this);
    m_bodyEdit->setPlaceholderText("Write your note here...");
    rightLayout->addWidget(m_bodyEdit);

    m_btnSave = new QPushButton("Save", this);
    m_btnSave->setStyleSheet("background: #2E75B6; color: white; padding: 6px;");
    rightLayout->addWidget(m_btnSave);

    setEditorEnabled(false);
    splitter->addWidget(rightPanel);

    splitter->setSizes({200, 500});
    main->addWidget(splitter);

    // ── Status bar ────────────────────────────────────────
    m_statusLabel = new QLabel("0 notes", this);
    m_statusLabel->setStyleSheet(
        "color: #555; font-size: 11px; padding: 2px 8px;"
        "background: #f0f0f0; border-top: 1px solid #ddd;");
    main->addWidget(m_statusLabel);

    // ── Wiring ────────────────────────────────────────────
    connect(m_btnNew,    &QPushButton::clicked,
            this,        &NoteView::newNoteRequested);
    connect(m_btnSave,   &QPushButton::clicked,
            this,        &NoteView::onSaveClicked);
    connect(m_btnDelete, &QPushButton::clicked,
            this,        &NoteView::onDeleteClicked);
    connect(m_noteList,  &QListWidget::itemClicked,
            this,        &NoteView::onListItemClicked);
    connect(m_searchInput, &QLineEdit::textChanged,
            this,          &NoteView::searchChanged);
}

void NoteView::populateList(const QList<Note>& notes) {
    m_noteList->clear();
    for (const Note& n : notes) {
        QListWidgetItem* item = new QListWidgetItem(n.title, m_noteList);
        item->setData(Qt::UserRole, n.id);
        item->setToolTip(n.updatedAt.toString("dd.MM.yyyy hh:mm"));
    }
}

void NoteView::addNoteToList(const Note& note) {
    QListWidgetItem* item = new QListWidgetItem(note.title);
    item->setData(Qt::UserRole, note.id);
    item->setToolTip(note.updatedAt.toString("dd.MM.yyyy hh:mm"));
    m_noteList->insertItem(0, item);
    m_noteList->setCurrentRow(0);
}

void NoteView::updateNoteInList(int index, const Note& note) {
    if (index >= 0 && index < m_noteList->count()) {
        QListWidgetItem* item = m_noteList->item(index);
        item->setText(note.title);
        item->setToolTip(note.updatedAt.toString("dd.MM.yyyy hh:mm"));
    }
}

void NoteView::removeNoteFromList(int id) {
    for (int i = 0; i < m_noteList->count(); i++) {
        if (m_noteList->item(i)->data(Qt::UserRole).toInt() == id) {
            delete m_noteList->takeItem(i);
            break;
        }
    }
    if (m_noteList->count() == 0) {
        clearEditor();
        setEditorEnabled(false);
        m_btnDelete->setEnabled(false);
        m_selectedId = -1;
    }
}

void NoteView::showNote(const Note& note) {
    m_selectedId = note.id;
    m_titleEdit->setText(note.title);
    m_bodyEdit->setText(note.body);
    m_timestampLabel->setText(
        QString("Created: %1   Updated: %2")
        .arg(note.createdAt.toString("dd.MM.yyyy hh:mm"))
        .arg(note.updatedAt.toString("dd.MM.yyyy hh:mm"))
    );
    setEditorEnabled(true);
    m_btnDelete->setEnabled(true);
}

void NoteView::clearEditor() {
    m_titleEdit->clear();
    m_bodyEdit->clear();
    m_timestampLabel->clear();
    m_selectedId = -1;
}

void NoteView::setEditorEnabled(bool enabled) {
    m_titleEdit->setEnabled(enabled);
    m_bodyEdit->setEnabled(enabled);
    m_btnSave->setEnabled(enabled);
}

void NoteView::updateStatusBar(int total) {
    m_statusLabel->setText(QString("%1 note%2")
        .arg(total).arg(total == 1 ? "" : "s"));
}

void NoteView::onListItemClicked(QListWidgetItem* item) {
    int index = m_noteList->row(item);
    emit noteSelected(index);
}

void NoteView::onSaveClicked() {
    emit saveNoteRequested(m_titleEdit->text(), m_bodyEdit->toPlainText());
}

void NoteView::onDeleteClicked() {
    if (m_selectedId >= 0)
        emit deleteNoteRequested(m_selectedId);
}
