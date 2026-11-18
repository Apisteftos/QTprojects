#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sendermodel.h"
#include "senderview.h"
#include "senderpresenter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    SenderModel*     m_model;
    SenderView*      m_view;
    SenderPresenter* m_presenter;
};

#endif
