#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include "hashtable.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void fillRandom();
    void insertKey();
    void deleteKey();
    void searchKey();
    void printTable();
    void findMinStack();

private:
    ExtendedHashTable ht;
    QTextEdit *output;
    QSpinBox *spinSize;
    QSpinBox *spinKey;
};

#endif