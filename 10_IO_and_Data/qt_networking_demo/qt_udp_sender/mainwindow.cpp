#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_model     = new SenderModel(this);
    m_view      = new SenderView(this);
    m_presenter = new SenderPresenter(m_model, m_view, this);
    setCentralWidget(m_view);
}
