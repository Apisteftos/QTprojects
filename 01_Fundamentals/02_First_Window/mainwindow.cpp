#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // The window's own properties
    setWindowTitle("My First Window");
    resize(640, 400);
    setMinimumSize(320, 200);

    // The status bar comes from the form; show a message for 5 seconds
    statusBar()->showMessage("Ready", 5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
