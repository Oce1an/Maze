#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QRegularExpression>
#include <QDate>
#include <QKeyEvent>
#include <vector>
#include "date.h"

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
    void on_btnOpenFile_clicked();
    void on_btnAddDate_clicked();
    void on_btnModifyDate_clicked();
    void on_btnShowPreviousDay_clicked();
    void on_btnCalculateBirthday_clicked();
    void on_btnCalculateDuration_clicked();
    void on_btnShowCurrentDate_clicked();
    void on_btnSaveAs_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<Date> dates;
    QString currentFileName;
    
    void saveDatesToFile(const QString& fileName);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif