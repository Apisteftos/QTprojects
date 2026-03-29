#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include "calculator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onCalculate();
    void onClear();
    void onResultReady(double result, const QString& expression);
    void onErrorOccurred(const QString& error);
    void onHistoryChanged();

private:
    Calculator*  m_calc;
    QLineEdit*   m_input;
    QLabel*      m_result;
    QListWidget* m_history;
    QPushButton* m_btnCalc;
    QPushButton* m_btnClear;
};

#endif // MAINWINDOW_H
