#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include "counter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    Counter*  m_counter        = nullptr;
    QLabel*   m_counterDisplay = nullptr;
    QSpinBox* m_spinBox        = nullptr;
};

#endif // MAINWINDOW_H
