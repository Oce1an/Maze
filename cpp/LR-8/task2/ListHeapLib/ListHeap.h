#ifndef LISTHEAP_H
#define LISTHEAP_H

#include <memory>
#include <vector>
#include <stdexcept>

// Определение API экспорта/импорта
#if defined(_WIN32) || defined(_WIN64)
    #ifdef LISTHEAP_LIBRARY
        #define LISTHEAP_API __declspec(dllexport)
    #else
        #define LISTHEAP_API __declspec(dllimport)
    #endif
#else
    #ifdef LISTHEAP_LIBRARY
        #define LISTHEAP_API __attribute__((visibility("default")))
    #else
        #define LISTHEAP_API
    #endif
#endif

template<typename T>
class LISTHEAP_API ListHeap {
private:
    struct Node {
        T value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;
        Node(T val) : value(val), left(nullptr), right(nullptr) {}
    };

    std::shared_ptr<Node> root;
    size_t nodeCount = 0;

    std::shared_ptr<Node> findInsertionParent() const {
        if (!root) return nullptr;
        size_t next = nodeCount + 1;
        std::vector<bool> path;
        while (next > 1) {
            path.push_back(next % 2 == 0);
            next /= 2;
        }
        auto cur = root;
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            if (*it) {
                if (!cur->left) break;
                cur = cur->left;
            } else {
                if (!cur->right) break;
                cur = cur->right;
            }
        }
        return cur;
    }

    void siftUp(std::shared_ptr<Node> node) {
        while (auto parent = node->parent.lock()) {
            if (node->value < parent->value) {
                std::swap(node->value, parent->value);
                node = parent;
            } else break;
        }
    }

    void siftDown(std::shared_ptr<Node> node) {
        while (node) {
            std::shared_ptr<Node> smallest = node;
            if (node->left && node->left->value < smallest->value)
                smallest = node->left;
            if (node->right && node->right->value < smallest->value)
                smallest = node->right;
            if (smallest == node) break;
            std::swap(node->value, smallest->value);
            node = smallest;
        }
    }

    void clearTree(std::shared_ptr<Node> node) {
        if (!node) return;
        clearTree(node->left);
        clearTree(node->right);
        node.reset();
    }

public:
    ListHeap() = default;
    ~ListHeap() {
        clearTree(root);
    }

    void push(const T& value) {
        auto newNode = std::make_shared<Node>(value);
        ++nodeCount;

        if (!root) {
            root = newNode;
            return;
        }

        auto parent = findInsertionParent();
        newNode->parent = parent;
        if (nodeCount % 2 == 0) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        siftUp(newNode);
    }

    T pop() {
        if (!root) throw std::underflow_error("Heap is empty");
        T minVal = root->value;

        if (nodeCount == 1) {
            root.reset();
            nodeCount = 0;
            return minVal;
        }

        auto parent = findInsertionParent();
        std::shared_ptr<Node> lastNode;
        if (parent->right) {
            lastNode = parent->right;
            parent->right.reset();
        } else {
            lastNode = parent->left;
            parent->left.reset();
        }

        root->value = lastNode->value;
        lastNode.reset();
        --nodeCount;
        siftDown(root);
        return minVal;
    }

    const T& top() const {
        if (!root) throw std::underflow_error("Heap is empty");
        return root->value;
    }

    bool empty() const { return nodeCount == 0; }
    size_t size() const { return nodeCount; }

    std::vector<T> toVector() const {
        std::vector<T> result;
        if (!root) return result;
        std::vector<std::shared_ptr<Node>> queue;
        queue.push_back(root);
        size_t idx = 0;
        while (idx < queue.size()) {
            auto cur = queue[idx++];
            result.push_back(cur->value);
            if (cur->left) queue.push_back(cur->left);
            if (cur->right) queue.push_back(cur->right);
        }
        return result;
    }
};

// Экспортируем явную инстанциацию для int
#ifdef LISTHEAP_LIBRARY
    extern template class LISTHEAP_API ListHeap<int>;
#endif

#endif // LISTHEAP_H