#include <iostream>
#include <chrono>

void heapSort(int* arr, int n);
void quickSort(int* arr, int low, int high);
void mergeSort(int* arr, int l, int r);

void heapify(int* arr, int n, int i);
int partition(int* arr, int low, int high);
void merge(int* arr, int l, int m, int r);

int binsearch(int* arr, int n, int digit);

int main() {
    int n;
    std::cout << "Enter the size of the array: ";
    std::cin >> n;

    int* original = new int[n];
    int* arrHeap  = new int[n];
    int* arrQuick = new int[n];
    int* arrMerge = new int[n];

    std::cout << "Enter " << n << " integer elements:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> original[i];
    }

    std::copy(original, original + n, arrHeap);
    std::copy(original, original + n, arrQuick);
    std::copy(original, original + n, arrMerge);

    auto start = std::chrono::high_resolution_clock::now();
    heapSort(arrHeap, n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> heapTime = end - start;
    std::cout << "Heap Sort time: " << heapTime.count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    quickSort(arrQuick, 0, n - 1);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> quickTime = end - start;
    std::cout << "Quick Sort time: " << quickTime.count() << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    mergeSort(arrMerge, 0, n - 1);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> mergeTime = end - start;
    std::cout << "Merge Sort time: " << mergeTime.count() << " ms\n";

    std::cout << "\nSorted array (after Merge Sort):\n";
    for (int i = 0; i < n; ++i) {
        std::cout << arrMerge[i] << " ";
    }
    std::cout << "\n";

    int digit;
    std::cout << "\nEnter a number to search: ";
    std::cin >> digit;
    int index = binsearch(arrMerge, n, digit);
    if (index != -1)
        std::cout << "Element found at index: " << index << "\n";
    else
        std::cout << "Element not found.\n";

    delete[] original;
    delete[] arrHeap;
    delete[] arrQuick;
    delete[] arrMerge;

    return 0;
}

void heapify(int* arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(int* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int binsearch(int* arr, int n, int digit) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == digit)
            return mid;
        else if (arr[mid] < digit)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}