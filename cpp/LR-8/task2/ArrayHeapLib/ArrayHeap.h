#ifndef ARRAYHEAP_H
#define ARRAYHEAP_H

#include <vector>
#include <stdexcept>

template<typename T>
class ArrayHeap {
public:
    void push(const T& value);
    T pop();
    const T& top() const;
    bool empty() const;
    size_t size() const;
    std::vector<T> toVector() const;

private:
    std::vector<T> data;
    void siftUp(size_t index);
    void siftDown(size_t index);
};

template<typename T>
void ArrayHeap<T>::push(const T& value) {
    data.push_back(value);
    siftUp(data.size() - 1);
}

template<typename T>
T ArrayHeap<T>::pop() {
    if (data.empty()) throw std::underflow_error("Heap is empty");
    T topVal = data.front();
    data[0] = data.back();
    data.pop_back();
    if (!data.empty()) siftDown(0);
    return topVal;
}

template<typename T>
const T& ArrayHeap<T>::top() const {
    if (data.empty()) throw std::underflow_error("Heap is empty");
    return data.front();
}

template<typename T>
bool ArrayHeap<T>::empty() const { return data.empty(); }
template<typename T>
size_t ArrayHeap<T>::size() const { return data.size(); }

template<typename T>
std::vector<T> ArrayHeap<T>::toVector() const { return data; }

template<typename T>
void ArrayHeap<T>::siftUp(size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (data[index] < data[parent]) {
            std::swap(data[index], data[parent]);
            index = parent;
        } else break;
    }
}

template<typename T>
void ArrayHeap<T>::siftDown(size_t index) {
    size_t sz = data.size();
    while (true) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t smallest = index;
        if (left < sz && data[left] < data[smallest]) smallest = left;
        if (right < sz && data[right] < data[smallest]) smallest = right;
        if (smallest == index) break;
        std::swap(data[index], data[smallest]);
        index = smallest;
    }
}
#endif