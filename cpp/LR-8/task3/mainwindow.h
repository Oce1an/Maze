#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include "map.h"
#include "set.h"
#include "unordered_map.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void mapInsert();
    void mapErase();
    void mapFind();
    void mapBuildList();
    void mapDisplayListIter();

    void setInsert();
    void setErase();
    void setFind();
    void setBuildList();
    void setDisplayListIter();

    void hashInsert();
    void hashErase();
    void hashContains();
    void hashRehash();
    void hashDisplay();

private:
    void updateMapTree();
    void updateSetTree();
    void updateHashTable();

    Map<int, QString> map;
    Set<int> set;
    UnorderedMap<int, QString> hashMap;

    QTabWidget *tabs;

    QSpinBox *mapKey;
    QLineEdit *mapValue;
    QTreeWidget *mapTree;
    QTreeWidget *mapListTree;

    QSpinBox *setKey;
    QTreeWidget *setTree;
    QTreeWidget *setListTree;

    QSpinBox *hashKey;
    QLineEdit *hashValue;
    QTableWidget *hashTable;
    QSpinBox *hashBuckets;
};

#endif