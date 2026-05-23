#include "vectordemowidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

VectorDemoWidget::VectorDemoWidget(QWidget* parent)
    : QWidget(parent)
{
    mainData.push_back(Pair<IntVector, PairVector>());
    auto& firstVec = mainData[0].first;
    firstVec.push_back(10);
    firstVec.push_back(20);
    firstVec.push_back(30);

    auto& secondVec = mainData[0].second;
    secondVec.push_back(Pair<int, double>(1, 1.1));
    secondVec.push_back(Pair<int, double>(2, 2.2));
    secondVec.push_back(Pair<int, double>(3, 3.3));

    auto* mainLayout = new QHBoxLayout(this);

    auto* buttonLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);

    auto* btnPushBackInt = new QPushButton("Push Back (int vector)");
    auto* btnPushBackPair = new QPushButton("Push Back (pair vector)");
    auto* btnPopBackInt = new QPushButton("Pop Back (int)");
    auto* btnErase = new QPushButton("Erase (int by index)");
    auto* btnInsert = new QPushButton("Insert (int)");
    auto* btnEmplace = new QPushButton("Emplace (pair)");
    auto* btnResize = new QPushButton("Resize (int to 5, val=0)");
    auto* btnSwap = new QPushButton("Swap temp vector with int");

    buttonLayout->addWidget(btnPushBackInt);
    buttonLayout->addWidget(btnPushBackPair);
    buttonLayout->addWidget(btnPopBackInt);
    buttonLayout->addWidget(btnErase);
    buttonLayout->addWidget(btnInsert);
    buttonLayout->addWidget(btnEmplace);
    buttonLayout->addWidget(btnResize);
    buttonLayout->addWidget(btnSwap);
    buttonLayout->addStretch();

    auto* tableLayout = new QVBoxLayout;
    mainLayout->addLayout(tableLayout);

    auto* labelInt = new QLabel("First matrix (vector<int>)");
    tableLayout->addWidget(labelInt);
    tableInt = new QTableWidget;
    tableInt->setColumnCount(1);
    tableInt->setHorizontalHeaderLabels(QStringList() << "Value");
    tableInt->horizontalHeader()->setStretchLastSection(true);
    tableLayout->addWidget(tableInt);

    auto* labelPair = new QLabel("Second matrix (vector<pair<int,double>>)");
    tableLayout->addWidget(labelPair);
    tablePair = new QTableWidget;
    tablePair->setColumnCount(2);
    tablePair->setHorizontalHeaderLabels(QStringList() << "Int" << "Double");
    tablePair->horizontalHeader()->setStretchLastSection(true);
    tableLayout->addWidget(tablePair);

    refreshTables();

    connect(btnPushBackInt, &QPushButton::clicked, this, &VectorDemoWidget::onPushBackInt);
    connect(btnPushBackPair, &QPushButton::clicked, this, &VectorDemoWidget::onPushBackPair);
    connect(btnPopBackInt, &QPushButton::clicked, this, &VectorDemoWidget::onPopBackInt);
    connect(btnErase, &QPushButton::clicked, this, &VectorDemoWidget::onErase);
    connect(btnInsert, &QPushButton::clicked, this, &VectorDemoWidget::onInsert);
    connect(btnEmplace, &QPushButton::clicked, this, &VectorDemoWidget::onEmplace);
    connect(btnResize, &QPushButton::clicked, this, &VectorDemoWidget::onResize);
    connect(btnSwap, &QPushButton::clicked, this, &VectorDemoWidget::onSwap);
}

void VectorDemoWidget::refreshTables() {
    const auto& firstVec = mainData[0].first;
    tableInt->setRowCount(static_cast<int>(firstVec.size()));
    for (size_t i = 0; i < firstVec.size(); ++i) {
        tableInt->setItem(static_cast<int>(i), 0,
            new QTableWidgetItem(QString::number(firstVec[i])));
    }

    const auto& secondVec = mainData[0].second;
    tablePair->setRowCount(static_cast<int>(secondVec.size()));
    for (size_t i = 0; i < secondVec.size(); ++i) {
        tablePair->setItem(static_cast<int>(i), 0,
            new QTableWidgetItem(QString::number(secondVec[i].first)));
        tablePair->setItem(static_cast<int>(i), 1,
            new QTableWidgetItem(QString::number(secondVec[i].second)));
    }
}

void VectorDemoWidget::onPushBackInt() {
    bool ok;
    int val = QInputDialog::getInt(this, "Push Back", "Enter integer:", 0, -10000000, 10000000, 1, &ok);
    if (ok) {
        mainData[0].first.push_back(val);
        refreshTables();
    }
}

void VectorDemoWidget::onPushBackPair() {
    bool ok1, ok2;
    int i = QInputDialog::getInt(this, "Push Back Pair", "Enter int:", 0, -10000000, 10000000, 1, &ok1);
    if (!ok1) return;
    double d = QInputDialog::getDouble(this, "Push Back Pair", "Enter double:", 0.0, -10000000, 10000000, 2, &ok2);
    if (ok2) {
        mainData[0].second.push_back(Pair<int, double>(i, d));
        refreshTables();
    }
}

void VectorDemoWidget::onPopBackInt() {
    if (!mainData[0].first.empty()) {
        mainData[0].first.pop_back();
        refreshTables();
    } else {
        QMessageBox::information(this, "Pop Back", "Vector is empty.");
    }
}

void VectorDemoWidget::onErase() {
    auto& vec = mainData[0].first;
    if (vec.empty()) {
        QMessageBox::information(this, "Erase", "Vector is empty.");
        return;
    }
    bool ok;
    int idx = QInputDialog::getInt(this, "Erase", "Enter index to erase:", 0, 0, static_cast<int>(vec.size())-1, 1, &ok);
    if (ok) {
        vec.erase(vec.cbegin() + idx);
        refreshTables();
    }
}

void VectorDemoWidget::onInsert() {
    auto& vec = mainData[0].first;
    bool ok1, ok2;
    int pos = QInputDialog::getInt(this, "Insert", "Enter position:", 0, 0, static_cast<int>(vec.size()), 1, &ok1);
    if (!ok1) return;
    int val = QInputDialog::getInt(this, "Insert", "Enter value:", 0, -10000000, 10000000, 1, &ok2);
    if (ok2) {
        vec.insert(vec.cbegin() + pos, val);
        refreshTables();
    }
}

void VectorDemoWidget::onEmplace() {
    auto& vec = mainData[0].second;
    bool ok1, ok2;
    int pos = QInputDialog::getInt(this, "Emplace", "Enter position:", 0, 0, static_cast<int>(vec.size()), 1, &ok1);
    if (!ok1) return;
    int i = QInputDialog::getInt(this, "Emplace Pair", "Enter int:", 0, -10000000, 10000000, 1, &ok2);
    if (!ok2) return;
    bool ok3;
    double d = QInputDialog::getDouble(this, "Emplace Pair", "Enter double:", 0.0, -10000000, 10000000, 2, &ok3);
    if (ok3) {
        vec.emplace(vec.cbegin() + pos, i, d);
        refreshTables();
    }
}

void VectorDemoWidget::onResize() {
    mainData[0].first.resize(5, 0);
    refreshTables();
}

void VectorDemoWidget::onSwap() {
    IntVector temp;
    temp.push_back(100);
    temp.push_back(200);
    mainData[0].first.swap(temp);
    refreshTables();
}