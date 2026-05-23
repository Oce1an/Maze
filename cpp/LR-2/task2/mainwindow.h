#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTableWidget>
#include <QInputDialog>
#include <vector>
#include <map>
#include <algorithm>
#include "student.h"
#include "resultdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLoadFile_clicked();
    void on_btnSaveFile_clicked();
    void on_btnAddStudent_clicked();
    void on_btnDeleteStudent_clicked();
    void on_btnSearch_clicked();
    void on_btnSortByName_clicked();
    void on_btnWinterDebtors_clicked();
    void on_btnExpelList_clicked();
    void on_btnClearTable_clicked();
    void on_btnEditStudent_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<Student> students;
    QString currentFileName;
    
    void fillTable(const std::vector<Student>& studentList);
    void fillTable();
    std::vector<Student> getWinterDebtors();
    std::vector<Student> getStudentsForExpel();
    void sortStudentsByNameInGroups();
};

#endif