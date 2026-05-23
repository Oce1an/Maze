#ifndef BST_H
#define BST_H

#include <memory>
#include <utility>
#include <functional>
#include <stdexcept>
#include <QString>

template<typename KeyType, typename ValueType>
class BST {
public:
    struct Node {
        std::pair<const KeyType, ValueType> data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node* parent = nullptr;
        Node* prev = nullptr;
        Node* next = nullptr;

        template<typename... Args>
        Node(Args&&... args) : data(std::forward<Args>(args)...) {}
    };

private:
    std::unique_ptr<Node> root;
    size_t sz = 0;
    Node* listHead = nullptr;
    Node* listTail = nullptr;

public:
    bool insert(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    Node* find(const KeyType& key) const;
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }

    class iterator {
    public:
        iterator(Node* ptr = nullptr, Node* root = nullptr)
            : current(ptr), treeRoot(root) {}
        
        std::pair<const KeyType, ValueType>& operator*() const { return current->data; }
        std::pair<const KeyType, ValueType>* operator->() const { return &current->data; }
        
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    
    private:
        Node* current;
        Node* treeRoot;
        Node* successor(Node* node) const;
        friend class BST;
    };
    
    iterator tree_begin();
    iterator tree_end();

    class list_iterator {
    public:
        list_iterator(Node* ptr = nullptr) : current(ptr) {}
        
        std::pair<const KeyType, ValueType>& operator*() const { return current->data; }
        std::pair<const KeyType, ValueType>* operator->() const { return &current->data; }
        
        list_iterator& operator++() { current = current->next; return *this; }
        list_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
        list_iterator& operator--() { current = current->prev; return *this; }
        list_iterator operator--(int) { auto tmp = *this; --(*this); return tmp; }
        
        bool operator==(const list_iterator& other) const { return current == other.current; }
        bool operator!=(const list_iterator& other) const { return current != other.current; }
    
    private:
        Node* current;
        friend class BST;
    };
    
    list_iterator list_begin() { return list_iterator(listHead); }
    list_iterator list_end() { return list_iterator(nullptr); }

    void buildList();
    void inorder(std::function<void(const KeyType&, const ValueType&)> visit) const;

private:
    void inorder(Node* node, std::function<void(const KeyType&, const ValueType&)> visit) const;
    Node* insertNode(std::unique_ptr<Node>& node, Node* parent, const KeyType& key, const ValueType& value);
    Node* eraseNode(std::unique_ptr<Node>& node, const KeyType& key, bool& deleted);
    Node* findNode(Node* node, const KeyType& key) const;
    Node* findMin(Node* node) const;
};


template<typename K, typename V>
typename BST<K,V>::iterator& BST<K,V>::iterator::operator++() {
    current = successor(current);
    return *this;
}

template<typename K, typename V>
typename BST<K,V>::iterator BST<K,V>::iterator::operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::iterator::successor(Node* node) const {
    if (!node) return nullptr;
    if (node->right) {
        Node* cur = node->right.get();
        while (cur->left) cur = cur->left.get();
        return cur;
    }
    Node* par = node->parent;
    while (par && node == par->right.get()) {
        node = par;
        par = par->parent;
    }
    return par;
}

template<typename K, typename V>
typename BST<K,V>::iterator BST<K,V>::tree_begin() {
    if (!root) return iterator(nullptr, nullptr);
    Node* cur = root.get();
    while (cur->left) cur = cur->left.get();
    return iterator(cur, root.get());
}

template<typename K, typename V>
typename BST<K,V>::iterator BST<K,V>::tree_end() {
    return iterator(nullptr, root.get());
}

template<typename K, typename V>
bool BST<K,V>::insert(const K& key, const V& value) {
    Node* ins = insertNode(root, nullptr, key, value);
    if (ins) {
        ++sz;
        return true;
    }
    return false;
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::insertNode(std::unique_ptr<Node>& node, Node* parent, const K& key, const V& value) {
    if (!node) {
        node = std::make_unique<Node>(key, value);
        node->parent = parent;
        return node.get();
    }
    if (key < node->data.first)
        return insertNode(node->left, node.get(), key, value);
    else if (key > node->data.first)
        return insertNode(node->right, node.get(), key, value);
    else
        return nullptr;
}

template<typename K, typename V>
bool BST<K,V>::erase(const K& key) {
    bool deleted = false;
    root.reset(eraseNode(root, key, deleted));
    if (deleted) --sz;
    return deleted;
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::eraseNode(std::unique_ptr<Node>& node, const K& key, bool& deleted) {
    if (!node) return nullptr;
    
    if (key < node->data.first) {
        node->left.reset(eraseNode(node->left, key, deleted));
        if (node->left) node->left->parent = node.get();
        return node.release();
    }
    else if (key > node->data.first) {
        node->right.reset(eraseNode(node->right, key, deleted));
        if (node->right) node->right->parent = node.get();
        return node.release();
    }
    else {
        deleted = true;
        if (!node->left) {
            Node* ret = node->right.release();
            if (ret) ret->parent = node->parent;
            return ret;
        }
        if (!node->right) {
            Node* ret = node->left.release();
            if (ret) ret->parent = node->parent;
            return ret;
        }
        Node* min = findMin(node->right.get());
        const_cast<K&>(node->data.first) = min->data.first;
        node->data.second = min->data.second;
        bool dummy = false;
        node->right.reset(eraseNode(node->right, min->data.first, dummy));
        if (node->right) node->right->parent = node.get();
        return node.release();
    }
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::findNode(Node* node, const K& key) const {
    while (node) {
        if (key < node->data.first) node = node->left.get();
        else if (key > node->data.first) node = node->right.get();
        else return node;
    }
    return nullptr;
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::find(const K& key) const {
    return findNode(root.get(), key);
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::findMin(Node* node) const {
    while (node && node->left) node = node->left.get();
    return node;
}

template<typename K, typename V>
void BST<K,V>::buildList() {
    listHead = listTail = nullptr;
    Node* prev = nullptr;
    std::function<void(Node*)> link = [&](Node* node) {
        if (!node) return;
        link(node->left.get());
        node->prev = prev;
        if (prev) prev->next = node;
        else listHead = node;
        prev = node;
        link(node->right.get());
    };
    link(root.get());
    listTail = prev;
    if (listTail) listTail->next = nullptr;
}

template<typename K, typename V>
void BST<K,V>::inorder(std::function<void(const K&, const V&)> visit) const {
    inorder(root.get(), visit);
}

template<typename K, typename V>
void BST<K,V>::inorder(Node* node, std::function<void(const K&, const V&)> visit) const {
    if (!node) return;
    inorder(node->left.get(), visit);
    visit(node->data.first, node->data.second);
    inorder(node->right.get(), visit);
}

#endif