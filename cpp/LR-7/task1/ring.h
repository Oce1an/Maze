#ifndef RING_H
#define RING_H

struct RNode {
    int data;
    RNode *next;
    RNode *prev;
    RNode(int d) : data(d), next(nullptr), prev(nullptr) {}
};

class Ring {
private:
    RNode *sentinel;
    int count;

public:
    Ring();
    ~Ring();

    void append(int value);
    void clear();
    int size() const;
    bool isEmpty() const;
    int* toArray(int &outSize) const;
};

#endif