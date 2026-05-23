#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mybitset.h"
#include <memory>

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
    void on_createButton_clicked();
    void on_setButton_clicked();
    void on_resetButton_clicked();
    void on_flipButton_clicked();
    void on_setAllButton_clicked();
    void on_resetAllButton_clicked();
    void on_flipAllButton_clicked();
    void on_andButton_clicked();
    void on_orButton_clicked();
    void on_notButton_clicked();
    void on_statsButton_clicked();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<MyBitSet> m_bsA;
    std::unique_ptr<MyBitSet> m_bsB;

    void refreshDisplay();
    void appendLog(const QString &msg);
    size_t getPosA();
};

#endif // MAINWINDOW_H
