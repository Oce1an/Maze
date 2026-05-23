#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include "deque.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPushBack();
    void onPushFront();
    void onPopBack();
    void onPopFront();
    void onClear();
    void onRandomAccess();
    void onIteratorTest();

private:
    void updateDisplay();

    Deque<int> deque;
    QListWidget *listWidget;
    QSpinBox *spinValue;
    QSpinBox *spinIndex;
    QLabel *labelSize;
    QLabel *labelEmpty;
};

#endif