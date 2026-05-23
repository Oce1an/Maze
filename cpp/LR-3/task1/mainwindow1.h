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
    void convert();

private:
    QLineEdit *inputEdit;
    QLabel *resultLabel;

    QString intToBin(long long n);
    void fracToBin(double frac, int maxDigits, QString &out);
    void fracToBinHelper(double frac, int maxDigits, QString &out);
};

#endif