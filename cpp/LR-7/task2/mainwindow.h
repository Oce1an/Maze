#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include "bst.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadFromTable();
    void balanceTree();
    void addRecord();
    void searchByKey();
    void deleteByKey();
    void showPreorder();
    void showInorder();
    void showPostorder();
    void showTotalStringLength();

private:
    void refreshTreeWidget();
    void log(const QString &text);

    QTableWidget *table;
    QTreeWidget *treeView;
    QTextEdit *output;

    QSpinBox *addKeyInput;
    QLineEdit *addStrInput;
    QSpinBox *searchKeyInput;
    QSpinBox *deleteKeyInput;

    ExtendedBST bst;
};

#endif