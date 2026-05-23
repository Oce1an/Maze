#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Ханойская башня");

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QFormLayout *formLayout = new QFormLayout();
    nSpinBox = new QSpinBox(this);
    nSpinBox->setRange(1, 20);
    nSpinBox->setValue(3);
    nSpinBox->setPrefix("n = ");
    formLayout->addRow("Количество колец:", nSpinBox);
    mainLayout->addLayout(formLayout);

    QPushButton *solveBtn = new QPushButton("Решить", this);
    mainLayout->addWidget(solveBtn);

    outputText = new QTextEdit(this);
    outputText->setReadOnly(true);
    outputText->setStyleSheet("font-family: monospace; font-size: 13px;");
    mainLayout->addWidget(outputText);

    setCentralWidget(central);
    connect(solveBtn, &QPushButton::clicked, this, &MainWindow::solveHanoi);
}

MainWindow::~MainWindow() {}

void MainWindow::hanoiOptimized(int n, char from, char to, char mid, QStringList &stepsList)
{
    if (n == 1) {
        static const QString format1("\nПереместить кольцо 1 с %1 на %2");
        stepsList.append(format1.arg(from).arg(to));
        return;
    }
    
    hanoiOptimized(n - 1, from, mid, to, stepsList);
    
    static const QString formatN("\nПереместить кольцо %1 с %2 на %3");
    stepsList.append(formatN.arg(n).arg(from).arg(to));
    
    hanoiOptimized(n - 1, mid, to, from, stepsList);
}

void MainWindow::hanoiStringBuilder(int n, char from, char to, char mid, QStringList &stepsList)
{
    if (n == 1) {
        stepsList.append(QStringLiteral("\nПереместить кольцо 1 с ") + 
                        QChar(from) + QStringLiteral(" на ") + QChar(to));
        return;
    }
    
    hanoiStringBuilder(n - 1, from, mid, to, stepsList);
    
    stepsList.append(QStringLiteral("\nПереместить кольцо ") + 
                    QString::number(n) + 
                    QStringLiteral(" с ") + QChar(from) + 
                    QStringLiteral(" на ") + QChar(to));
    
    hanoiStringBuilder(n - 1, mid, to, from, stepsList);
}

void MainWindow::solveHanoi()
{
    int n = nSpinBox->value();
    
    outputText->setUpdatesEnabled(false);
    outputText->clear();
    
    outputText->setPlainText(QString("Вычисление решения для %1 колец...\n").arg(n));
    QApplication::processEvents();
    
    int totalSteps = (1 << n) - 1;
    QStringList stepsList;
    stepsList.reserve(totalSteps + 1);
    
    stepsList.append(QString("Решение для %1 колец:\n").arg(n));
    
    hanoiOptimized(n, 'A', 'C', 'B', stepsList);
    
    QString result = stepsList.join(QString());
    outputText->setPlainText(result);
    
    outputText->setUpdatesEnabled(true);
}