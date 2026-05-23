#include "task1widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <algorithm>
#include <chrono>
#include <vector>

static void heapify(int* arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

static void heapSort(int* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

static int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j)
        if (arr[j] <= pivot) { ++i; std::swap(arr[i], arr[j]); }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

static void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

static void merge(int* arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

static void mergeSort(int* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
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

Task1Widget::Task1Widget(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);

    auto* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(new QLabel("Array size:"));
    sizeEdit = new QLineEdit;
    sizeEdit->setPlaceholderText("e.g. 10");
    inputLayout->addWidget(sizeEdit);

    inputLayout->addWidget(new QLabel("Elements (space separated):"));
    elementsEdit = new QLineEdit;
    elementsEdit->setPlaceholderText("e.g. 5 3 8 1 ...");
    inputLayout->addWidget(elementsEdit);
    mainLayout->addLayout(inputLayout);

    auto* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("Search value:"));
    searchEdit = new QLineEdit;
    searchLayout->addWidget(searchEdit);
    runButton = new QPushButton("Run sorts & Search");
    searchLayout->addWidget(runButton);
    mainLayout->addLayout(searchLayout);

    output = new QTextEdit;
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    connect(runButton, &QPushButton::clicked, this, &Task1Widget::onSortAndSearch);
}

void Task1Widget::onSortAndSearch()
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

    int* original = new int[n];
    int* arrHeap  = new int[n];
    int* arrQuick = new int[n];
    int* arrMerge = new int[n];

    for (int i = 0; i < n; ++i) {
        bool okNum;
        original[i] = parts[i].toInt(&okNum);
        if (!okNum) {
            output->setText("Invalid number in array.");
            delete[] original; delete[] arrHeap; delete[] arrQuick; delete[] arrMerge;
            return;
        }
        arrHeap[i] = arrQuick[i] = arrMerge[i] = original[i];
    }

    QString report;

    auto t1 = std::chrono::high_resolution_clock::now();
    heapSort(arrHeap, n);
    auto t2 = std::chrono::high_resolution_clock::now();
    double heapMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    report += QString("Heap sort time: %1 ms\n").arg(heapMs, 0, 'f', 6);

    t1 = std::chrono::high_resolution_clock::now();
    quickSort(arrQuick, 0, n - 1);
    t2 = std::chrono::high_resolution_clock::now();
    double quickMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    report += QString("Quick sort time: %1 ms\n").arg(quickMs, 0, 'f', 6);

    t1 = std::chrono::high_resolution_clock::now();
    mergeSort(arrMerge, 0, n - 1);
    t2 = std::chrono::high_resolution_clock::now();
    double mergeMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    report += QString("Merge sort time: %1 ms\n").arg(mergeMs, 0, 'f', 6);

    report += "\nSorted array (merge sort):\n";
    for (int i = 0; i < n; ++i)
        report += QString::number(arrMerge[i]) + " ";

    bool searchOk;
    int digit = searchEdit->text().toInt(&searchOk);
    if (searchOk) {
        int idx = binsearch(arrMerge, n, digit);
        if (idx != -1)
            report += QString("\n\nElement %1 found at index: %2").arg(digit).arg(idx);
        else
            report += QString("\n\nElement %1 not found (-1).").arg(digit);
    } else {
        report += "\n\nNo search performed (invalid input).";
    }

    output->setText(report);

    delete[] original;
    delete[] arrHeap;
    delete[] arrQuick;
    delete[] arrMerge;
}