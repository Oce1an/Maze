#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Deque (блочная реализация)");
    resize(500, 400);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    listWidget = new QListWidget();
    mainLayout->addWidget(listWidget);

    QHBoxLayout *infoLayout = new QHBoxLayout();
    labelSize = new QLabel("Size: 0");
    labelEmpty = new QLabel("Empty: true");
    infoLayout->addWidget(labelSize);
    infoLayout->addWidget(labelEmpty);
    mainLayout->addLayout(infoLayout);

    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    spinValue = new QSpinBox();
    spinValue->setRange(-999, 999);
    spinValue->setValue(42);
    QPushButton *btnPushBack = new QPushButton("Push Back");
    QPushButton *btnPushFront = new QPushButton("Push Front");
    connect(btnPushBack, &QPushButton::clicked, this, &MainWindow::onPushBack);
    connect(btnPushFront, &QPushButton::clicked, this, &MainWindow::onPushFront);
    ctrlLayout->addWidget(spinValue);
    ctrlLayout->addWidget(btnPushBack);
    ctrlLayout->addWidget(btnPushFront);
    mainLayout->addLayout(ctrlLayout);

    QHBoxLayout *ctrlLayout2 = new QHBoxLayout();
    QPushButton *btnPopBack = new QPushButton("Pop Back");
    QPushButton *btnPopFront = new QPushButton("Pop Front");
    QPushButton *btnClear = new QPushButton("Clear");
    connect(btnPopBack, &QPushButton::clicked, this, &MainWindow::onPopBack);
    connect(btnPopFront, &QPushButton::clicked, this, &MainWindow::onPopFront);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClear);
    ctrlLayout2->addWidget(btnPopBack);
    ctrlLayout2->addWidget(btnPopFront);
    ctrlLayout2->addWidget(btnClear);
    mainLayout->addLayout(ctrlLayout2);

    QHBoxLayout *raLayout = new QHBoxLayout();
    spinIndex = new QSpinBox();
    spinIndex->setRange(0, 0);
    QPushButton *btnAccess = new QPushButton("Доступ по индексу");
    connect(btnAccess, &QPushButton::clicked, this, &MainWindow::onRandomAccess);
    raLayout->addWidget(new QLabel("Индекс:"));
    raLayout->addWidget(spinIndex);
    raLayout->addWidget(btnAccess);
    mainLayout->addLayout(raLayout);

    QPushButton *btnIterTest = new QPushButton("Тест: итератор после resize");
    connect(btnIterTest, &QPushButton::clicked, this, &MainWindow::onIteratorTest);
    mainLayout->addWidget(btnIterTest);

    updateDisplay();
}

MainWindow::~MainWindow() {}

void MainWindow::updateDisplay() {
    listWidget->clear();
    for (size_t i = 0; i < deque.size(); ++i) {
        listWidget->addItem(QString::number(deque[i]));
    }
    labelSize->setText(QString("Size: %1").arg(deque.size()));
    labelEmpty->setText(QString("Empty: %1").arg(deque.empty() ? "true" : "false"));
    spinIndex->setMaximum(deque.size() ? deque.size() - 1 : 0);
}

void MainWindow::onPushBack() {
    deque.push_back(spinValue->value());
    updateDisplay();
}

void MainWindow::onPushFront() {
    deque.push_front(spinValue->value());
    updateDisplay();
}

void MainWindow::onPopBack() {
    if (deque.empty()) {
        QMessageBox::warning(this, "Ошибка", "Deque пуст");
        return;
    }
    deque.pop_back();
    updateDisplay();
}

void MainWindow::onPopFront() {
    if (deque.empty()) {
        QMessageBox::warning(this, "Ошибка", "Deque пуст");
        return;
    }
    deque.pop_front();
    updateDisplay();
}

void MainWindow::onClear() {
    deque.clear();
    updateDisplay();
}

void MainWindow::onRandomAccess() {
    if (deque.empty()) {
        QMessageBox::information(this, "Random Access", "Deque пуст");
        return;
    }
    size_t idx = spinIndex->value();
    if (idx >= deque.size()) {
        QMessageBox::warning(this, "Ошибка", "Индекс вне диапазона");
        return;
    }
    QMessageBox::information(this, "Random Access", 
                             QString("Элемент [%1] = %2").arg(idx).arg(deque[idx]));
}

void MainWindow::onIteratorTest() {
    if (deque.empty()) {
        QMessageBox::information(this, "Тест", "Добавьте хотя бы один элемент");
        return;
    }
    Deque<int>::iterator it = deque.begin();
    int firstVal = *it;

    for (int i = 0; i < 200; ++i)
        deque.push_back(1000 + i);

    bool valid = (*it == firstVal);
    updateDisplay();
    QMessageBox::information(this, "Тест итератора",
                             QString("Итератор на первый элемент (значение %1) после добавления 200 элементов %2валиден.")
                             .arg(firstVal).arg(valid ? "" : "НЕ "));
}