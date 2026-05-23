#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include "bst.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAdd();
    void onRemove();
    void onFind();
    void onTraverse();
    void onInsertSubtree();
    void onRemoveSubtree();
    void onInsertBranch();
    void onRemoveBranch();
    void onInsertAtPosition();
    void onLCA();

private:
    void updateTreeView();
    void addNodeToTree(QTreeWidgetItem* parentItem, BSTNode* node);

    BST tree;
    QTreeWidget* treeWidget;
    QTextEdit* output;

    QSpinBox *spinKey, *spinKey2, *spinParent;
    QCheckBox *checkAsLeft;
};

#endif