#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <vector>
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Binary Heap Demo - Array vs List");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Общий ввод
    QHBoxLayout *inputLayout = new QHBoxLayout();
    valueInput = new QSpinBox();
    valueInput->setRange(-999, 999);
    valueInput->setValue(42);
    QPushButton *pushArrayBtn = new QPushButton("Push (Array)");
    QPushButton *popArrayBtn = new QPushButton("Pop (Array)");
    QPushButton *pushListBtn = new QPushButton("Push (List)");
    QPushButton *popListBtn = new QPushButton("Pop (List)");
    connect(pushArrayBtn, &QPushButton::clicked, this, &MainWindow::pushArray);
    connect(popArrayBtn, &QPushButton::clicked, this, &MainWindow::popArray);
    connect(pushListBtn, &QPushButton::clicked, this, &MainWindow::pushList);
    connect(popListBtn, &QPushButton::clicked, this, &MainWindow::popList);
    inputLayout->addWidget(valueInput);
    inputLayout->addWidget(pushArrayBtn);
    inputLayout->addWidget(popArrayBtn);
    inputLayout->addWidget(pushListBtn);
    inputLayout->addWidget(popListBtn);
    mainLayout->addLayout(inputLayout);

    // Отображения
    QHBoxLayout *displays = new QHBoxLayout();
    arrayDisplay = new QTextEdit();
    arrayDisplay->setReadOnly(true);
    listDisplay = new QTextEdit();
    listDisplay->setReadOnly(true);
    displays->addWidget(new QLabel("Array Heap:"));
    displays->addWidget(arrayDisplay);
    displays->addWidget(new QLabel("List Heap:"));
    displays->addWidget(listDisplay);
    mainLayout->addLayout(displays);

    updateDisplays();
}

MainWindow::~MainWindow() {}

void MainWindow::pushArray() {
    arrayHeap.push(valueInput->value());
    updateDisplays();
}

void MainWindow::popArray() {
    if (arrayHeap.empty()) {
        QMessageBox::warning(this, "Error", "Array heap is empty");
        return;
    }
    arrayHeap.pop();
    updateDisplays();
}

void MainWindow::pushList() {
    listHeap.push(valueInput->value());
    updateDisplays();
}

void MainWindow::popList() {
    if (listHeap.empty()) {
        QMessageBox::warning(this, "Error", "List heap is empty");
        return;
    }
    listHeap.pop();
    updateDisplays();
}

void MainWindow::updateDisplays() {
    QString arrayStr;
    auto vec = arrayHeap.toVector();
    for (size_t i = 0; i < vec.size(); ++i) {
        arrayStr += QString::number(vec[i]) + " ";
    }
    arrayDisplay->setPlainText(arrayStr);

    QString listStr;
    auto vec2 = listHeap.toVector();
    for (size_t i = 0; i < vec2.size(); ++i) {
        listStr += QString::number(vec2[i]) + " ";
    }
    listDisplay->setPlainText(listStr);
}