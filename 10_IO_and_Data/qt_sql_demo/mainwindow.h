#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "noterepository.h"
#include "notemodel.h"
#include "noteview.h"
#include "notepresenter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    NoteRepository* m_repo;
    NoteModel*      m_model;
    NoteView*       m_view;
    NotePresenter*  m_presenter;
};

#endif // MAINWINDOW_H
