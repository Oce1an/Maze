#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void reverseNumber();

private:
    QLineEdit *inputEdit;
    QLabel *resultLabel;

    int reverseInt(int n, int acc);
    bool hasNoZero(int n);
};

#endif