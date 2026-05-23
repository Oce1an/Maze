#include "task2widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>
#include <string>
#include <sstream>

static long long binpow(long long digit, long long powder, long long mod) {
    long long res = 1 % mod;
    digit %= mod;
    while (powder > 0) {
        if (powder & 1) res = (res * digit) % mod;
        digit = (digit * digit) % mod;
        powder >>= 1;
    }
    return res;
}

static int binsearch(const int* arr, int n, int digit) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == digit) return mid;
        else if (arr[mid] < digit) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

static std::string interpolationSortWithSteps(int* arr, int n) {
    std::ostringstream out;
    if (n <= 1) {
        out << "Array after step 1 (no changes): ";
        for (int i = 0; i < n; ++i) out << arr[i] << " ";
        out << "\n";
        return out.str();
    }

    int minVal = arr[0], maxVal = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    out << "Initial array: ";
    for (int i = 0; i < n; ++i) out << arr[i] << " ";
    out << "\n\n";

    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int lo = 0, hi = i - 1;
        int pos = i;

        if (maxVal != minVal && arr[hi] != arr[lo]) {
            long long numerator = (long long)(key - arr[lo]) * (hi - lo);
            long long denominator = (long long)(arr[hi] - arr[lo]);
            pos = lo + (int)(numerator / denominator);
            if (pos < 0) pos = 0;
            if (pos > i) pos = i;
        } else {
            pos = i;
        }

        while (pos < i && arr[pos] <= key) ++pos;
        while (pos > 0 && arr[pos - 1] > key) --pos;

        for (int j = i; j > pos; --j) arr[j] = arr[j - 1];
        arr[pos] = key;

        out << "Step " << i << ": ";
        for (int j = 0; j < n; ++j) out << arr[j] << " ";
        out << "\n";
    }
    return out.str();
}

Task2Widget::Task2Widget(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);

    auto* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(new QLabel("Array size:"));
    sizeEdit = new QLineEdit;
    sizeEdit->setPlaceholderText("e.g. 5");
    inputLayout->addWidget(sizeEdit);

    inputLayout->addWidget(new QLabel("Elements:"));
    elementsEdit = new QLineEdit;
    elementsEdit->setPlaceholderText("e.g. 4 2 5 1 3");
    inputLayout->addWidget(elementsEdit);
    mainLayout->addLayout(inputLayout);

    auto* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("Search value:"));
    searchEdit = new QLineEdit;
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(new QLabel("Modulus for binpow:"));
    modEdit = new QLineEdit;
    modEdit->setPlaceholderText("e.g. 1000");
    searchLayout->addWidget(modEdit);
    runButton = new QPushButton("Run Interpolation Sort & Search");
    searchLayout->addWidget(runButton);
    mainLayout->addLayout(searchLayout);

    output = new QTextEdit;
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    connect(runButton, &QPushButton::clicked, this, &Task2Widget::onRun);
}

void Task2Widget::onRun()
{
    output->clear();
    bool ok;
    int n = sizeEdit->text().toInt(&ok);
    if (!ok || n <= 0) {
        output->setText("Invalid array size.");
        return;
    }

    QStringList parts = elementsEdit->text().split(' ', Qt::SkipEmptyParts);
    if (parts.size() != n) {
        output->setText(QString("Enter exactly %1 numbers.").arg(n));
        return;
    }

    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = parts[i].toInt(&ok);
        if (!ok) {
            output->setText("Invalid number in array.");
            delete[] arr;
            return;
        }
    }

    std::string steps = interpolationSortWithSteps(arr, n);
    QString report = QString::fromStdString(steps);

    report += "\nFinal sorted array: ";
    for (int i = 0; i < n; ++i) report += QString::number(arr[i]) + " ";

    int searchDigit = searchEdit->text().toInt(&ok);
    if (!ok) {
        report += "\n\nInvalid search value.";
        delete[] arr;
        output->setText(report);
        return;
    }
    int idx = binsearch(arr, n, searchDigit);
    if (idx != -1) {
        int mod = modEdit->text().toInt(&ok);
        if (ok && mod > 0) {
            long long result = binpow(idx, n, mod);
            report += QString("\n\nElement %1 found at index %2.\n(%2 ^ %3) % %4 = %5")
                          .arg(searchDigit).arg(idx).arg(n).arg(mod).arg(result);
        } else {
            report += QString("\n\nElement %1 found at index %2. Invalid modulus – no binpow result.")
                          .arg(searchDigit).arg(idx);
        }
    } else {
        report += QString("\n\nElement %1 not found.").arg(searchDigit);
    }

    output->setText(report);
    delete[] arr;
}