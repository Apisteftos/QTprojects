#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "taskmodel.h"
#include "taskview.h"
#include "taskpresenter.h"

// ============================================================
// MainWindow — the application shell
//
// Just owns the three MVP components and wires them together.
// No business logic lives here.
// ============================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    TaskModel*     m_model;
    TaskView*      m_view;
    TaskPresenter* m_presenter;
};

#endif // MAINWINDOW_H
