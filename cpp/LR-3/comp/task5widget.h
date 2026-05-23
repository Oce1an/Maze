#ifndef TASK5WIDGET_H
#define TASK5WIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QHeaderView>
#include <QVBoxLayout>

class Task5Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task5Widget(QWidget *parent = nullptr);

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