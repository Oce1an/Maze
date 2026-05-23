#include "mainwindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Переворот числа");

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    inputEdit = new QLineEdit(this);
    inputEdit->setPlaceholderText("Введите число без нулей");
    layout->addWidget(inputEdit);

    QPushButton *btn = new QPushButton("Перевернуть", this);
    layout->addWidget(btn);

    resultLabel = new QLabel("Результат появится здесь", this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet(
        "font-size: 18px; font-weight: bold; padding: 10px;"
        "background-color: #3d3d3d; border-radius: 5px;");
    layout->addWidget(resultLabel);

    setCentralWidget(central);
    connect(btn, &QPushButton::clicked, this, &MainWindow::reverseNumber);
}

MainWindow::~MainWindow() {}

int MainWindow::reverseInt(int n, int acc)
{
    if (n == 0)
        return acc;
    return reverseInt(n / 10, acc * 10 + n % 10);
}

bool MainWindow::hasNoZero(int n)
{
    if (n == 0)
        return true;
    if (n % 10 == 0)
        return false;
    return hasNoZero(n / 10);
}

void MainWindow::reverseNumber()
{
    bool ok = false;
    int number = inputEdit->text().toInt(&ok);

    if (!ok || number <= 0) {
        resultLabel->setText("Ошибка: введите целое положительное число!");
        return;
    }

    if (!hasNoZero(number)) {
        resultLabel->setText("Ошибка: число не должно содержать нулей!");
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    int reversed = reverseInt(number, 0);
    QApplication::restoreOverrideCursor();

    resultLabel->setText("Результат: " + QString::number(reversed));
}