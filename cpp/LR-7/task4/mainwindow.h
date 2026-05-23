#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QPushButton>
#include <QTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runResearch();
    void showTable();

private:
    QChartView *chartView;
    QChart *chart;
    QPushButton *btnResearch;
    QPushButton *btnShowTable;
    QTextEdit *textOutput;
};

#endif