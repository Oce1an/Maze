#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int medianOfThree(int a, int b, int c) {
    if (a > b) std::swap(a, b);
    if (a > c) std::swap(a, c);
    if (b > c) std::swap(b, c);
    return b;
}

double average(const std::vector<int>& group) {
    if (group.empty()) return 0.0;
    double sum = 0.0;
    for (int x : group) sum += x;
    return sum / group.size();
}

int main() {
    int n;
    std::cout << "Enter odd number of elements: ";
    std::cin >> n;

    std::vector<int> a(n);
    std::cout << "Enter " << n << " integer elements:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    std::vector<double> b;

    for (size_t i = 0; i < a.size(); i += 3) {
        size_t remain = std::min<size_t>(3, a.size() - i);
        std::vector<int> group(a.begin() + i, a.begin() + i + remain);

        if (group.size() == 3) {
            int med = medianOfThree(group[0], group[1], group[2]);
            b.push_back(med);
        } else {
            b.push_back(average(group));
        }
    }

    std::cout << "b = {";
    for (size_t i = 0; i < b.size(); ++i) {
        std::cout << b[i];
        if (i != b.size() - 1) std::cout << ", ";
    }
    std::cout << "}\n";

    return 0;
}