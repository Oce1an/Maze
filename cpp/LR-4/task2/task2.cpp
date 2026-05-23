#include <iostream>
#include <algorithm>

long long binpow(long long digit, long long powder, long long mod) {
    long long res = 1 % mod;
    digit %= mod;
    while (powder > 0) {
        if (powder & 1)
            res = (res * digit) % mod;
        digit = (digit * digit) % mod;
        powder >>= 1;
    }
    return res;
}

int binsearch(const int* arr, int n, int digit) {
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

void interpolationSort(int* arr, int n) {
    if (n <= 1) {
        std::cout << "Array after step 1 (no changes): ";
        for (int i = 0; i < n; ++i) std::cout << arr[i] << " ";
        std::cout << "\n";
        return;
    }

    int minVal = arr[0], maxVal = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    std::cout << "Initial array: ";
    for (int i = 0; i < n; ++i) std::cout << arr[i] << " ";
    std::cout << "\n\n";

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

        while (pos < i && arr[pos] <= key)
            ++pos;
        while (pos > 0 && arr[pos - 1] > key)
            --pos;

        for (int j = i; j > pos; --j)
            arr[j] = arr[j - 1];
        arr[pos] = key;

        std::cout << "Step " << i << ": ";
        for (int j = 0; j < n; ++j)
            std::cout << arr[j] << " ";
        std::cout << "\n";
    }
}

int main() {
    int n;
    std::cout << "Enter the size of the array: ";
    std::cin >> n;

    int* arr = new int[n];
    std::cout << "Enter " << n << " integer elements:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    interpolationSort(arr, n);
    std::cout << "\nFinal sorted array: ";
    for (int i = 0; i < n; ++i) std::cout << arr[i] << " ";
    std::cout << "\n";

    int searchDigit;
    std::cout << "\nEnter a number to search: ";
    std::cin >> searchDigit;
    int index = binsearch(arr, n, searchDigit);

    if (index != -1) {
        int mod;
        std::cout << "Enter modulus for exponentiation: ";
        std::cin >> mod;
        long long result = binpow(index, n, mod);
        std::cout << "Result: (" << index << " ^ " << n << ") % " << mod << " = " << result << "\n";
    } else {
        std::cout << "Element not found. Search result: -1\n";
    }

    delete[] arr;
    return 0;
}