#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void solveHanoi();

private:
    QSpinBox *nSpinBox;
    QTextEdit *outputText;

    // Оригинальный метод (оставлен для совместимости)
    void hanoi(int n, char from, char to, char aux, QString &steps);
    
    // Оптимизированные методы
    void hanoiOptimized(int n, char from, char to, char mid, QStringList &stepsList);
    void hanoiStringBuilder(int n, char from, char to, char mid, QStringList &stepsList);
};

#endif