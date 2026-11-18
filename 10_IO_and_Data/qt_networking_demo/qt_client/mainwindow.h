#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectionmodel.h"
#include "clientview.h"
#include "clientpresenter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    ConnectionModel*  m_model;
    ClientView*       m_view;
    ClientPresenter*  m_presenter;
};

#endif // MAINWINDOW_H
