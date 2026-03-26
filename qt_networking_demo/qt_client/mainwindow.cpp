#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_model     = new ConnectionModel(this);
    m_view      = new ClientView(this);
    m_presenter = new ClientPresenter(m_model, m_view, this);

    setCentralWidget(m_view);
    setMinimumSize(500, 600);
}
