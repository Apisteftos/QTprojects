#include "noterepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

NoteRepository::NoteRepository(const QString& dbPath, QObject* parent)
    : QObject(parent), m_dbPath(dbPath)
{}

bool NoteRepository::open() {
    // ── Add SQLite connection ──────────────────────────────
    // Qt supports multiple DB connections — each needs a name.
    // "QSQLITE" is the driver name for SQLite.
    m_db = QSqlDatabase::addDatabase("QSQLITE", "notes_connection");
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }

    return createSchema();
}

bool NoteRepository::isOpen() const {
    return m_db.isOpen();
}

QString NoteRepository::lastError() const {
    return m_lastError;
}

bool NoteRepository::createSchema() {
    QSqlQuery query(m_db);

    // CREATE TABLE IF NOT EXISTS — safe to call every startup
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS notes ("
        "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  title      TEXT    NOT NULL,"
        "  body       TEXT    NOT NULL DEFAULT '',"
        "  created_at TEXT    NOT NULL,"
        "  updated_at TEXT    NOT NULL"
        ")"
    );

    if (!ok) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }

    return true;
}

// ── CREATE ─────────────────────────────────────────────────
bool NoteRepository::createNote(const QString& title, const QString& body) {
    if (title.trimmed().isEmpty()) {
        m_lastError = "Title cannot be empty";
        emit errorOccurred(m_lastError);
        return false;
    }

    QSqlQuery query(m_db);

    // Prepared statements — ALWAYS use these, never string concat
    // Protects against SQL injection
    query.prepare(
        "INSERT INTO notes (title, body, created_at, updated_at) "
        "VALUES (:title, :body, :created_at, :updated_at)"
    );

    QString now = QDateTime::currentDateTime().toString(Qt::ISODate);
    query.bindValue(":title",      title.trimmed());
    query.bindValue(":body",       body);
    query.bindValue(":created_at", now);
    query.bindValue(":updated_at", now);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }

    // Fetch the full note to emit it
    Note note = getNoteById(query.lastInsertId().toInt());
    emit noteCreated(note);
    return true;
}

// ── READ ALL ───────────────────────────────────────────────
QList<Note> NoteRepository::getAllNotes() {
    QList<Note> notes;
    QSqlQuery query("SELECT id, title, body, created_at, updated_at "
                    "FROM notes ORDER BY updated_at DESC", m_db);

    while (query.next()) {
        Note n;
        n.id        = query.value("id").toInt();
        n.title     = query.value("title").toString();
        n.body      = query.value("body").toString();
        n.createdAt = QDateTime::fromString(
                          query.value("created_at").toString(), Qt::ISODate);
        n.updatedAt = QDateTime::fromString(
                          query.value("updated_at").toString(), Qt::ISODate);
        notes.append(n);
    }

    return notes;
}

// ── READ ONE ───────────────────────────────────────────────
Note NoteRepository::getNoteById(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, title, body, created_at, updated_at "
                  "FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    Note n{};
    if (query.next()) {
        n.id        = query.value("id").toInt();
        n.title     = query.value("title").toString();
        n.body      = query.value("body").toString();
        n.createdAt = QDateTime::fromString(
                          query.value("created_at").toString(), Qt::ISODate);
        n.updatedAt = QDateTime::fromString(
                          query.value("updated_at").toString(), Qt::ISODate);
    }
    return n;
}

// ── UPDATE ─────────────────────────────────────────────────
bool NoteRepository::updateNote(int id, const QString& title, const QString& body) {
    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE notes SET title = :title, body = :body, updated_at = :updated_at "
        "WHERE id = :id"
    );
    query.bindValue(":title",      title.trimmed());
    query.bindValue(":body",       body);
    query.bindValue(":updated_at", QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(":id",         id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }

    Note note = getNoteById(id);
    emit noteUpdated(note);
    return true;
}

// ── DELETE ─────────────────────────────────────────────────
bool NoteRepository::deleteNote(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }

    emit noteDeleted(id);
    return true;
}

// ── COUNT ──────────────────────────────────────────────────
int NoteRepository::noteCount() {
    QSqlQuery query("SELECT COUNT(*) FROM notes", m_db);
    if (query.next()) return query.value(0).toInt();
    return 0;
}
