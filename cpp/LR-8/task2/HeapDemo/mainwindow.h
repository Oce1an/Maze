#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include "ArrayHeap.h"
#include "ListHeap.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pushArray();
    void popArray();
    void pushList();
    void popList();
    void updateDisplays();

private:
    QTextEdit *arrayDisplay;
    QTextEdit *listDisplay;
    QSpinBox *valueInput;
    ArrayHeap<int> arrayHeap;
    ListHeap<int> listHeap;
};

#endif