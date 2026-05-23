#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Двухсвязная очередь и кольца");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *labelDeque = new QLabel("Очередь (Deque):");
    mainLayout->addWidget(labelDeque);
    dequeList = new QListWidget();
    mainLayout->addWidget(dequeList);

    QHBoxLayout *ctrlLayout1 = new QHBoxLayout();
    sizeSpin = new QSpinBox();
    sizeSpin->setRange(1, 50);
    sizeSpin->setValue(10);
    ctrlLayout1->addWidget(new QLabel("Размер:"));
    ctrlLayout1->addWidget(sizeSpin);
    QPushButton *btnRandom = new QPushButton("Создать случайную очередь");
    connect(btnRandom, &QPushButton::clicked, this, &MainWindow::createRandomDeque);
    ctrlLayout1->addWidget(btnRandom);
    mainLayout->addLayout(ctrlLayout1);

    QHBoxLayout *ctrlLayout2 = new QHBoxLayout();
    valueSpin = new QSpinBox();
    valueSpin->setRange(-999, 999);
    valueSpin->setValue(50);
    ctrlLayout2->addWidget(new QLabel("Значение:"));
    ctrlLayout2->addWidget(valueSpin);
    QPushButton *btnPush = new QPushButton("Добавить в конец");
    connect(btnPush, &QPushButton::clicked, this, &MainWindow::pushBack);
    ctrlLayout2->addWidget(btnPush);
    QPushButton *btnPop = new QPushButton("Удалить первый");
    connect(btnPop, &QPushButton::clicked, this, &MainWindow::popFront);
    ctrlLayout2->addWidget(btnPop);
    mainLayout->addLayout(ctrlLayout2);

    QHBoxLayout *ctrlLayout3 = new QHBoxLayout();
    QPushButton *btnFront = new QPushButton("Первый");
    connect(btnFront, &QPushButton::clicked, this, &MainWindow::showFront);
    QPushButton *btnBack = new QPushButton("Последний");
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::showBack);
    QPushButton *btnSize = new QPushButton("Размер");
    connect(btnSize, &QPushButton::clicked, this, &MainWindow::showSize);
    QPushButton *btnClear = new QPushButton("Очистить");
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearDeque);
    ctrlLayout3->addWidget(btnFront);
    ctrlLayout3->addWidget(btnBack);
    ctrlLayout3->addWidget(btnSize);
    ctrlLayout3->addWidget(btnClear);
    mainLayout->addLayout(ctrlLayout3);

    QPushButton *btnSplit = new QPushButton("Разделить на кольца (вариант 7)");
    connect(btnSplit, &QPushButton::clicked, this, &MainWindow::splitToRings);
    mainLayout->addWidget(btnSplit);

    QLabel *labelRing1 = new QLabel("Первое кольцо (между мин и макс):");
    mainLayout->addWidget(labelRing1);
    ring1List = new QListWidget();
    mainLayout->addWidget(ring1List);

    QLabel *labelRing2 = new QLabel("Второе кольцо (остальные):");
    mainLayout->addWidget(labelRing2);
    ring2List = new QListWidget();
    mainLayout->addWidget(ring2List);

    createRandomDeque();
}

MainWindow::~MainWindow() {}

void MainWindow::createRandomDeque() {
    int size = sizeSpin->value();
    m_deque.fromRandom(size, 0, 99);
    updateDequeDisplay();
    m_ring1.clear();
    m_ring2.clear();
    ring1List->clear();
    ring2List->clear();
}

void MainWindow::pushBack() {
    m_deque.push_back(valueSpin->value());
    updateDequeDisplay();
}

void MainWindow::popFront() {
    if (m_deque.isEmpty()) {
        QMessageBox::warning(this, "Очередь пуста", "Нечего удалять.");
        return;
    }
    m_deque.pop_front();
    updateDequeDisplay();
}

void MainWindow::showFront() {
    if (m_deque.isEmpty()) {
        QMessageBox::information(this, "Первый элемент", "Очередь пуста.");
    } else {
        QMessageBox::information(this, "Первый элемент", QString::number(m_deque.front()));
    }
}

void MainWindow::showBack() {
    if (m_deque.isEmpty()) {
        QMessageBox::information(this, "Последний элемент", "Очередь пуста.");
    } else {
        QMessageBox::information(this, "Последний элемент", QString::number(m_deque.back()));
    }
}

void MainWindow::showSize() {
    QMessageBox::information(this, "Размер", QString::number(m_deque.size()));
}

void MainWindow::clearDeque() {
    m_deque.clear();
    updateDequeDisplay();
}

void MainWindow::updateDequeDisplay() {
    dequeList->clear();
    if (m_deque.isEmpty()) return;
    int size;
    int *arr = m_deque.toArray(size);
    for (int i = 0; i < size; ++i) {
        dequeList->addItem(QString::number(arr[i]));
    }
    delete[] arr;
}

void MainWindow::splitToRings() {
    if (m_deque.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Очередь пуста, невозможно разделить.");
        return;
    }

    m_ring1.clear();
    m_ring2.clear();

    int size;
    int *arr = m_deque.toArray(size);
    if (size == 1) {
        m_ring1.append(arr[0]);
        delete[] arr;
        updateRingDisplays();
        return;
    }

    int minVal = arr[0], maxVal = arr[0];
    int minIdx = 0, maxIdx = 0;
    for (int i = 1; i < size; ++i) {
        if (arr[i] < minVal) {
            minVal = arr[i];
            minIdx = i;
        }
        if (arr[i] > maxVal) {
            maxVal = arr[i];
            maxIdx = i;
        }
    }

    int start = minIdx;
    int end = maxIdx;
    if (start > end) {
        start = maxIdx;
        end = minIdx;
    }

    for (int i = start; i <= end; ++i) {
        m_ring1.append(arr[i]);
    }

    for (int i = 0; i < start; ++i) {
        m_ring2.append(arr[i]);
    }
    for (int i = end + 1; i < size; ++i) {
        m_ring2.append(arr[i]);
    }

    delete[] arr;
    updateRingDisplays();
}

void MainWindow::updateRingDisplays() {
    ring1List->clear();
    ring2List->clear();

    displayRing(ring1List, m_ring1);
    displayRing(ring2List, m_ring2);
}

void MainWindow::displayRing(QListWidget *list, Ring &ring) {
    if (ring.isEmpty()) return;
    int size;
    int *arr = ring.toArray(size);
    for (int i = 0; i < size; ++i) {
        list->addItem(QString::number(arr[i]));
    }
    delete[] arr;
}