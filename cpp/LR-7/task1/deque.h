#ifndef DEQUE_H
#define DEQUE_H

struct DNode {
    int data;
    DNode *next;
    DNode *prev;
    DNode(int d) : data(d), next(nullptr), prev(nullptr) {}
};

class Deque {
private:
    DNode *head;
    DNode *tail;
    int count;

public:
    Deque();
    ~Deque();

    void push_back(int value);
    int pop_front();           
    int front() const;         
    int back() const;       
    bool isEmpty() const;
    int size() const;
    void clear();

    void fromRandom(int n, int minVal = 0, int maxVal = 100);
    int* toArray(int &outSize) const; 

    int findMin() const;
    int findMax() const;

    DNode* getHead() const { return head; }
    DNode* getTail() const { return tail; }
};

#endif