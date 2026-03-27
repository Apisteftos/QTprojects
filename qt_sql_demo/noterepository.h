#ifndef NOTEREPOSITORY_H
#define NOTEREPOSITORY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QSqlDatabase>

// ============================================================
// Note — the data structure
// ============================================================
struct Note {
    int      id;
    QString  title;
    QString  body;
    QDateTime createdAt;
    QDateTime updatedAt;
};

// ============================================================
// NoteRepository — DATABASE layer (Repository pattern)
//
// Responsibilities:
//   - Opens and manages the SQLite database
//   - Creates the schema if it doesn't exist
//   - Provides CRUD operations (Create Read Update Delete)
//   - Returns plain data structs — no Qt widgets
//
// This is the DATA layer beneath the Model.
// In larger apps the Model uses the Repository.
// The Repository knows SQL. The Model knows business rules.
//
// SQLite in Qt:
//   - No server needed — the database is a single .db file
//   - QSqlDatabase — connection management
//   - QSqlQuery   — execute SQL statements
//   - QSqlError   — error reporting
// ============================================================
class NoteRepository : public QObject
{
    Q_OBJECT

public:
    explicit NoteRepository(const QString& dbPath, QObject* parent = nullptr);

    bool open();
    bool isOpen() const;
    QString lastError() const;

    // CRUD
    bool        createNote(const QString& title, const QString& body);
    QList<Note> getAllNotes();
    Note        getNoteById(int id);
    bool        updateNote(int id, const QString& title, const QString& body);
    bool        deleteNote(int id);
    int         noteCount();

signals:
    void noteCreated(const Note& note);
    void noteUpdated(const Note& note);
    void noteDeleted(int id);
    void errorOccurred(const QString& error);

private:
    bool createSchema();

    QSqlDatabase m_db;
    QString      m_dbPath;
    QString      m_lastError;
};

#endif // NOTEREPOSITORY_H
