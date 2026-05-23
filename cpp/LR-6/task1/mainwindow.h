#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

// 1
struct State {
    int population;
    double area;
    char continentCode;
    bool isUNMember;
    char name[50];
    int gdpGrowth[3];
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showDefaultInit();
    void showSeparateAssignment();
    void showUserInput();
    void showPointerInit();
    void showReferenceInit();

private:
    QString stateToString(const State &s); // форматированный вывод полей
    QTextEdit *output;
};

#endif // MAINWINDOW_H