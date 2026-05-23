#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QHeaderView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectFolder();
    void openFile(QTreeWidgetItem *item, int column);

private:
    QTreeWidget *treeWidget;
    QPushButton *selectBtn;
    QLabel *infoLabel;

    void traverse(const QString &path, int &folderCount, int &fileCount,
                  QTreeWidgetItem *parentItem);
};

#endif