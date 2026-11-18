#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt MVP Demo — Task Manager");
    setMinimumSize(400, 500);

    // Create the three MVP layers
    m_model     = new TaskModel(this);
    m_view      = new TaskView(this);
    m_presenter = new TaskPresenter(m_model, m_view, this);

    setCentralWidget(m_view);
}
