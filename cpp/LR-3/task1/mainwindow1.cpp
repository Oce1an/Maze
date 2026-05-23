#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <cmath>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Recursive Number Conversion");

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    inputEdit = new QLineEdit(this);
    inputEdit->setPlaceholderText("Введите вещественное число: ");
    inputEdit->setValidator(new QDoubleValidator(this));
    layout->addWidget(inputEdit);

    QPushButton *convertBtn = new QPushButton("Преобразовать", this);
    layout->addWidget(convertBtn);

    resultLabel = new QLabel("Результат: ", this);
    resultLabel->setWordWrap(true);
    layout->addWidget(resultLabel);

    setCentralWidget(central);
    connect(convertBtn, &QPushButton::clicked, this, &MainWindow::convert);
}

MainWindow::~MainWindow() {}

QString MainWindow::intToBin(long long n)
{
    if (n == 0) return "0";
    if (n == 1) return "1";
    return intToBin(n / 2) + QString::number(n % 2);
}

void MainWindow::fracToBin(double frac, int maxDigits, QString &out)
{
    if (maxDigits <= 0 || frac == 0.0) return;
    fracToBinHelper(frac, maxDigits, out);
}

void MainWindow::fracToBinHelper(double frac, int maxDigits, QString &out)
{
    frac *= 2.0;
    if (frac >= 1.0) {
        out.append('1');
        frac -= 1.0;
    } else {
        out.append('0');
    }
    fracToBin(frac, maxDigits - 1, out);
}

void MainWindow::convert()
{
    bool ok = false;
    double number = inputEdit->text().toDouble(&ok);
    if (!ok) {
        resultLabel->setText("Ошибка: введите корректное вещественное число.");
        return;
    }

    if (!std::isfinite(number)) {
        resultLabel->setText("Ошибка: число слишком большое или недопустимое.");
        return;
    }

    QString sign = (number < 0) ? "-" : "";
    double absNumber = std::fabs(number);

    const double maxLL = static_cast<double>(std::numeric_limits<long long>::max());
    if (absNumber > maxLL) {
        resultLabel->setText("Ошибка: целая часть слишком велика для long long.\n"
                             "Максимальное значение: 9 223 372 036 854 775 807");
        return;
    }

    long long integerPart = static_cast<long long>(absNumber);
    double fractionalPart = absNumber - integerPart;

    QString binInteger = (integerPart == 0) ? "0" : intToBin(integerPart);

    QString binFraction;
    const int MAX_FRAC_DIGITS = 10;
    if (fractionalPart > 1e-12) {
        fracToBin(fractionalPart, MAX_FRAC_DIGITS, binFraction);
    }

    QString result = sign + binInteger;
    if (!binFraction.isEmpty())
        result += "." + binFraction;

    resultLabel->setText("Результат: " + result);
}