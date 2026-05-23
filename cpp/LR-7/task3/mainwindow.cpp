#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTime>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Хеш-таблица");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    output = new QTextEdit();
    output->setReadOnly(true);
    layout->addWidget(output);

    QHBoxLayout *sizeLayout = new QHBoxLayout();
    spinSize = new QSpinBox();
    spinSize->setRange(2, 50);
    spinSize->setValue(7);
    sizeLayout->addWidget(new QLabel("Размер таблицы:"));
    sizeLayout->addWidget(spinSize);
    QPushButton *btnFill = new QPushButton("Заполнить случайно");
    connect(btnFill, &QPushButton::clicked, this, &MainWindow::fillRandom);
    sizeLayout->addWidget(btnFill);
    layout->addLayout(sizeLayout);

    QHBoxLayout *opLayout = new QHBoxLayout();
    spinKey = new QSpinBox();
    spinKey->setRange(-9999, 9999);
    spinKey->setValue(42);
    opLayout->addWidget(new QLabel("Ключ:"));
    opLayout->addWidget(spinKey);
    QPushButton *btnInsert = new QPushButton("Вставить");
    connect(btnInsert, &QPushButton::clicked, this, &MainWindow::insertKey);
    QPushButton *btnDelete = new QPushButton("Удалить");
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteKey);
    QPushButton *btnSearch = new QPushButton("Найти");
    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::searchKey);
    opLayout->addWidget(btnInsert);
    opLayout->addWidget(btnDelete);
    opLayout->addWidget(btnSearch);
    layout->addLayout(opLayout);

    QPushButton *btnPrint = new QPushButton("Печать хеш-таблицы");
    connect(btnPrint, &QPushButton::clicked, this, &MainWindow::printTable);
    layout->addWidget(btnPrint);

    QPushButton *btnMinStack = new QPushButton("Номер стека с минимальным ключом");
    connect(btnMinStack, &QPushButton::clicked, this, &MainWindow::findMinStack);
    layout->addWidget(btnMinStack);

    ht = ExtendedHashTable(spinSize->value());
    fillRandom();
}

MainWindow::~MainWindow() {}

void MainWindow::fillRandom() {
    int size = spinSize->value();
    ht = ExtendedHashTable(size);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-100, 99);
    
    for (int i = 0; i < 20; ++i) {
        int key = dis(gen);
        ht.insert(key);
    }
    printTable();
    output->append("Создана случайная хеш-таблица.\n");
}

void MainWindow::insertKey() {
    int key = spinKey->value();
    ht.insert(key);
    output->append(QString("Вставлен ключ: %1\n").arg(key));
    printTable();
}

void MainWindow::deleteKey() {
    int key = spinKey->value();
    if (ht.remove(key)) {
        output->append(QString("Удалён ключ: %1\n").arg(key));
    } else {
        QMessageBox::information(this, "Удаление", "Ключ не найден.");
    }
    printTable();
}

void MainWindow::searchKey() {
    int key = spinKey->value();
    if (ht.search(key)) {
        QMessageBox::information(this, "Поиск", QString("Ключ %1 найден.").arg(key));
    } else {
        QMessageBox::information(this, "Поиск", "Ключ не найден.");
    }
}

void MainWindow::printTable() {
    output->clear();
    output->append(ht.print());
}

void MainWindow::findMinStack() {
    int idx = ht.findStackWithMinKey();
    if (idx == -1) {
        QMessageBox::information(this, "Результат", "Таблица пуста.");
    } else {
        QMessageBox::information(this, "Минимальный ключ",
            QString("Стек с минимальным ключом находится по индексу %1").arg(idx));
        output->append(QString("\n*** Стек %1 содержит минимальный ключ ***").arg(idx));
    }
}