#include "deque.h"
#include <cstdlib>
#include <ctime>
#include <cassert>

Deque::Deque() : head(nullptr), tail(nullptr), count(0) {}

Deque::~Deque() {
    clear();
}

void Deque::push_back(int value) {
    DNode *node = new DNode(value);
    if (isEmpty()) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    ++count;
}

int Deque::pop_front() {
    assert(!isEmpty());
    DNode *node = head;
    int value = node->data;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete node;
    --count;
    return value;
}

int Deque::front() const {
    assert(!isEmpty());
    return head->data;
}

int Deque::back() const {
    assert(!isEmpty());
    return tail->data;
}

bool Deque::isEmpty() const {
    return count == 0;
}

int Deque::size() const {
    return count;
}

void Deque::clear() {
    while (!isEmpty()) {
        pop_front();
    }
}

void Deque::fromRandom(int n, int minVal, int maxVal) {
    clear();
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < n; ++i) {
        int val = minVal + rand() % (maxVal - minVal + 1);
        push_back(val);
    }
}

int* Deque::toArray(int &outSize) const {
    outSize = count;
    if (count == 0) return nullptr;
    int *arr = new int[count];
    DNode *cur = head;
    for (int i = 0; i < count; ++i) {
        arr[i] = cur->data;
        cur = cur->next;
    }
    return arr;
}

int Deque::findMin() const {
    assert(!isEmpty());
    int minVal = head->data;
    DNode *cur = head->next;
    while (cur) {
        if (cur->data < minVal) minVal = cur->data;
        cur = cur->next;
    }
    return minVal;
}

int Deque::findMax() const {
    assert(!isEmpty());
    int maxVal = head->data;
    DNode *cur = head->next;
    while (cur) {
        if (cur->data > maxVal) maxVal = cur->data;
        cur = cur->next;
    }
    return maxVal;
}