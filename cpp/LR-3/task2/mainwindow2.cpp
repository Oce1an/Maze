#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Функция Аккермана");

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QFormLayout *formLayout = new QFormLayout();
    
    mSpinBox = new QSpinBox(this);
    mSpinBox->setRange(0, 4);
    mSpinBox->setValue(2);
    mSpinBox->setPrefix("m = ");
    formLayout->addRow("m:", mSpinBox);

    nSpinBox = new QSpinBox(this);
    nSpinBox->setRange(0, 2147483647);
    nSpinBox->setValue(2);
    nSpinBox->setPrefix("n = ");
    formLayout->addRow("n:", nSpinBox);

    mainLayout->addLayout(formLayout);

    QPushButton *calcBtn = new QPushButton("Вычислить", this);
    mainLayout->addWidget(calcBtn);

    resultLabel = new QLabel("Результат:", this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet(
        "font-size: 16px; font-weight: bold; "
        "background-color: #3d3d3d; padding: 15px; border-radius: 5px;");
    mainLayout->addWidget(resultLabel);

    setCentralWidget(central);
    connect(calcBtn, &QPushButton::clicked, this, &MainWindow::calculateAckermann);
}

MainWindow::~MainWindow() {}

long long MainWindow::ackermann(long long m, long long n)
{
    if (m == 0) {
        return n + 1;
    }
    else if (m == 1){
        return n + 2;
    }
    else if (m == 2){
        return (m * n) + 3;
    }
    else if (m == 3 && n > 60){
        return 0;
    }
    else if (m == 4 && n > 2){
        return 0;
    }
    else if (m > 0 && n == 0) {
        return ackermann(m - 1, 1);
    }
    else if (m > 0 && n > 0) {
        return ackermann(m - 1, ackermann(m, n - 1));
    }
    return 0;
}

void MainWindow::calculateAckermann()
{
    long long m = mSpinBox->value();
    long long n = nSpinBox->value();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    
    long long result = ackermann(m, n);
    
    QApplication::restoreOverrideCursor();

    resultLabel->setText(
        "A(" + QString::number(m) + ", " + QString::number(n) + ") = " + 
        QString::number(result));
}