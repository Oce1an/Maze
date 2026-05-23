#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void executeCommand();

private:
    void showMenu();
    void case1_writeWcharText();
    void case2_readWcharText();
    void case3_writeFloatArray();
    void case4_readFloatArray();
    void case5_appendTextStruct();
    void case6_readTextStructs();
    void case7_appendBinaryStruct();
    void case8_readBinaryStructs();
    void case9_appendLine();
    void case10_readLines();

    QString stateToString(const State &s);
    State inputState();

    QTextEdit *output;
    QLineEdit *inputLine;
};

#endif // MAINWINDOW_H