#include "mainwindow.h"
#include <QStandardPaths>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt Notes — SQLite Demo");
    setMinimumSize(700, 500);

    // Store DB in user's app data folder — persists between runs
    QString dbPath = QStandardPaths::writableLocation(
                         QStandardPaths::AppDataLocation) + "/notes.db";

    m_repo      = new NoteRepository(dbPath, this);
    m_model     = new NoteModel(m_repo, this);
    m_view      = new NoteView(this);
    m_presenter = new NotePresenter(m_model, m_view, this);

    if (!m_repo->open()) {
        QMessageBox::critical(this, "Database Error",
            "Could not open database:\n" + m_repo->lastError());
    }

    setCentralWidget(m_view);
}
