#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../lib/mystring.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_runButton_clicked();

private:
    Ui::MainWindow *ui;

    void runDemo(const QString &funcName, const QString &s1, const QString &s2, const QString &nStr);
    void appendResult(const QString &line);
};

#endif
