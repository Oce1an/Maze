#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Лаб.8 – Map, Set, HashTable");
    resize(800, 600);

    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    QWidget *mapTab = new QWidget();
    QVBoxLayout *mapLayout = new QVBoxLayout(mapTab);
    QHBoxLayout *mapCtrl = new QHBoxLayout();
    mapKey = new QSpinBox(); mapKey->setRange(-999,999); mapKey->setValue(5);
    mapValue = new QLineEdit(); mapValue->setText("Hello");
    QPushButton *mapIns = new QPushButton("Insert");
    QPushButton *mapEr = new QPushButton("Erase");
    QPushButton *mapFind = new QPushButton("Find");
    QPushButton *mapBuild = new QPushButton("Build List");
    QPushButton *mapListIt = new QPushButton("Show list iter");
    mapCtrl->addWidget(new QLabel("Key:"));
    mapCtrl->addWidget(mapKey);
    mapCtrl->addWidget(new QLabel("Value:"));
    mapCtrl->addWidget(mapValue);
    mapCtrl->addWidget(mapIns);
    mapCtrl->addWidget(mapEr);
    mapCtrl->addWidget(mapFind);
    mapCtrl->addWidget(mapBuild);
    mapCtrl->addWidget(mapListIt);
    mapLayout->addLayout(mapCtrl);
    QHBoxLayout *mapTrees = new QHBoxLayout();
    mapTree = new QTreeWidget(); mapTree->setHeaderLabel("Map (inorder)");
    mapListTree = new QTreeWidget(); mapListTree->setHeaderLabel("List order");
    mapTrees->addWidget(mapTree);
    mapTrees->addWidget(mapListTree);
    mapLayout->addLayout(mapTrees);
    tabs->addTab(mapTab, "Map<int, QString>");
    connect(mapIns, &QPushButton::clicked, this, &MainWindow::mapInsert);
    connect(mapEr, &QPushButton::clicked, this, &MainWindow::mapErase);
    connect(mapFind, &QPushButton::clicked, this, &MainWindow::mapFind);
    connect(mapBuild, &QPushButton::clicked, this, &MainWindow::mapBuildList);
    connect(mapListIt, &QPushButton::clicked, this, &MainWindow::mapDisplayListIter);

    QWidget *setTab = new QWidget();
    QVBoxLayout *setLayout = new QVBoxLayout(setTab);
    QHBoxLayout *setCtrl = new QHBoxLayout();
    setKey = new QSpinBox(); setKey->setRange(-999,999); setKey->setValue(10);
    QPushButton *setIns = new QPushButton("Insert");
    QPushButton *setEr = new QPushButton("Erase");
    QPushButton *setFind = new QPushButton("Find");
    QPushButton *setBuild = new QPushButton("Build List");
    QPushButton *setListIt = new QPushButton("Show list iter");
    setCtrl->addWidget(new QLabel("Key:"));
    setCtrl->addWidget(setKey);
    setCtrl->addWidget(setIns);
    setCtrl->addWidget(setEr);
    setCtrl->addWidget(setFind);
    setCtrl->addWidget(setBuild);
    setCtrl->addWidget(setListIt);
    setLayout->addLayout(setCtrl);
    QHBoxLayout *setTrees = new QHBoxLayout();
    setTree = new QTreeWidget(); setTree->setHeaderLabel("Set (inorder)");
    setListTree = new QTreeWidget(); setListTree->setHeaderLabel("List order");
    setTrees->addWidget(setTree);
    setTrees->addWidget(setListTree);
    setLayout->addLayout(setTrees);
    tabs->addTab(setTab, "Set<int>");
    connect(setIns, &QPushButton::clicked, this, &MainWindow::setInsert);
    connect(setEr, &QPushButton::clicked, this, &MainWindow::setErase);
    connect(setFind, &QPushButton::clicked, this, &MainWindow::setFind);
    connect(setBuild, &QPushButton::clicked, this, &MainWindow::setBuildList);
    connect(setListIt, &QPushButton::clicked, this, &MainWindow::setDisplayListIter);

    QWidget *hashTab = new QWidget();
    QVBoxLayout *hashLayout = new QVBoxLayout(hashTab);
    QHBoxLayout *hashCtrl = new QHBoxLayout();
    hashKey = new QSpinBox(); hashKey->setRange(-999,999); hashKey->setValue(99);
    hashValue = new QLineEdit(); hashValue->setText("world");
    QPushButton *hashIns = new QPushButton("Insert");
    QPushButton *hashEr = new QPushButton("Erase");
    QPushButton *hashCont = new QPushButton("Contains");
    QPushButton *hashRehashBtn = new QPushButton("Rehash");
    hashBuckets = new QSpinBox(); hashBuckets->setRange(1,100); hashBuckets->setValue(8);
    hashCtrl->addWidget(new QLabel("Key:"));
    hashCtrl->addWidget(hashKey);
    hashCtrl->addWidget(new QLabel("Value:"));
    hashCtrl->addWidget(hashValue);
    hashCtrl->addWidget(hashIns);
    hashCtrl->addWidget(hashEr);
    hashCtrl->addWidget(hashCont);
    hashCtrl->addWidget(new QLabel("Buckets:"));
    hashCtrl->addWidget(hashBuckets);
    hashCtrl->addWidget(hashRehashBtn);
    hashLayout->addLayout(hashCtrl);
    hashTable = new QTableWidget();
    hashTable->setColumnCount(2);
    hashTable->setHorizontalHeaderLabels({"Key", "Value"});
    hashTable->horizontalHeader()->setStretchLastSection(true);
    hashLayout->addWidget(hashTable);
    tabs->addTab(hashTab, "UnorderedMap<int,QString>");
    connect(hashIns, &QPushButton::clicked, this, &MainWindow::hashInsert);
    connect(hashEr, &QPushButton::clicked, this, &MainWindow::hashErase);
    connect(hashCont, &QPushButton::clicked, this, &MainWindow::hashContains);
    connect(hashRehashBtn, &QPushButton::clicked, this, &MainWindow::hashRehash);
    connect(hashRehashBtn, &QPushButton::clicked, this, &MainWindow::hashDisplay);

    updateMapTree();
    updateSetTree();
    updateHashTable();
}

MainWindow::~MainWindow() {}

void MainWindow::mapInsert() {
    map.insert(mapKey->value(), mapValue->text());
    updateMapTree();
}

void MainWindow::mapErase() {
    map.erase(mapKey->value());
    updateMapTree();
}

void MainWindow::mapFind() {
    auto node = map.find(mapKey->value());
    QMessageBox::information(this, "Find", node ? "Found" : "Not found");
}

void MainWindow::mapBuildList() {
    map.buildList();
    updateMapTree();
}

void MainWindow::mapDisplayListIter() {
    map.buildList();
    mapListTree->clear();
    for (auto it = map.list_begin(); it != map.list_end(); ++it) {
        auto& pair = *it;
        mapListTree->addTopLevelItem(
            new QTreeWidgetItem({QString("%1 -> %2").arg(pair.first).arg(pair.second)}));
    }
}

void MainWindow::updateMapTree() {
    mapTree->clear();
    map.inorder([this](const int& k, const QString& v) {
        mapTree->addTopLevelItem(new QTreeWidgetItem({QString("%1 : %2").arg(k).arg(v)}));
    });
    mapDisplayListIter();
}

void MainWindow::setInsert() {
    set.insert(setKey->value());
    updateSetTree();
}

void MainWindow::setErase() {
    set.erase(setKey->value());
    updateSetTree();
}

void MainWindow::setFind() {
    auto node = set.find(setKey->value());
    QMessageBox::information(this, "Find", node ? "Found" : "Not found");
}

void MainWindow::setBuildList() {
    set.buildList();
    updateSetTree();
}

void MainWindow::setDisplayListIter() {
    set.buildList();
    setListTree->clear();
    for (auto it = set.list_begin(); it != set.list_end(); ++it) {
        auto& pair = *it;
        setListTree->addTopLevelItem(
            new QTreeWidgetItem({QString::number(pair.first)}));
    }
}

void MainWindow::updateSetTree() {
    setTree->clear();
    set.inorder([this](const int& k, const char&) {
        setTree->addTopLevelItem(new QTreeWidgetItem({QString::number(k)}));
    });
    setDisplayListIter();
}

void MainWindow::hashInsert() {
    hashMap.insert(hashKey->value(), hashValue->text());
    updateHashTable();
}

void MainWindow::hashErase() {
    hashMap.erase(hashKey->value());
    updateHashTable();
}

void MainWindow::hashContains() {
    bool c = hashMap.contains(hashKey->value());
    QMessageBox::information(this, "Contains", c ? "Yes" : "No");
}

void MainWindow::hashRehash() {
    hashMap.rehash(hashBuckets->value());
    updateHashTable();
}

void MainWindow::updateHashTable() {
    auto items = hashMap.items();
    hashTable->setRowCount(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        hashTable->setItem(i, 0, new QTableWidgetItem(QString::number(items[i].first)));
        hashTable->setItem(i, 1, new QTableWidgetItem(items[i].second));
    }
}

void MainWindow::hashDisplay() {
    updateHashTable();
}