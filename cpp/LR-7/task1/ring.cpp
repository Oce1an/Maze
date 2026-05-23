#include "ring.h"

Ring::Ring() : count(0) {
    sentinel = new RNode(0);
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
}

Ring::~Ring() {
    clear();
    delete sentinel;
}

void Ring::append(int value) {
    RNode *node = new RNode(value);
    RNode *last = sentinel->prev;
    node->next = sentinel;
    node->prev = last;
    last->next = node;
    sentinel->prev = node;
    ++count;
}

void Ring::clear() {
    RNode *cur = sentinel->next;
    while (cur != sentinel) {
        RNode *tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    count = 0;
}

int Ring::size() const {
    return count;
}

bool Ring::isEmpty() const {
    return count == 0;
}

int* Ring::toArray(int &outSize) const {
    outSize = count;
    if (count == 0) return nullptr;
    int *arr = new int[count];
    RNode *cur = sentinel->next;
    for (int i = 0; i < count; ++i) {
        arr[i] = cur->data;
        cur = cur->next;
    }
    return arr;
}