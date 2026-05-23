#include "BST.h"
#include <stdexcept>

BST::BST() : root(nullptr) {}

BST::~BST() { clear(); }

void BST::clear() {
    clearTree(root);
    root = nullptr;
}

void BST::clearTree(BSTNode* node) {
    if (node) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

bool BST::add(int key) {
    BSTNode* newNode = new BSTNode(key);
    if (!root) {
        root = newNode;
        return true;
    }
    BSTNode* cur = root;
    BSTNode* parent = nullptr;
    while (cur) {
        parent = cur;
        if (key < cur->key)
            cur = cur->left;
        else if (key > cur->key)
            cur = cur->right;
        else {
            delete newNode;
            return false;
        }
    }
    newNode->parent = parent;
    if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;
    return true;
}

BSTNode* BST::find(int key) const {
    return findNode(root, key);
}

BSTNode* BST::findNode(BSTNode* node, int key) const {
    while (node && node->key != key) {
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

BSTNode* BST::findMin(BSTNode* node) const {
    while (node && node->left)
        node = node->left;
    return node;
}

void BST::transplant(BSTNode* u, BSTNode* v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

bool BST::remove(int key) {
    BSTNode* z = find(key);
    if (!z) return false;

    if (!z->left)
        transplant(z, z->right);
    else if (!z->right)
        transplant(z, z->left);
    else {
        BSTNode* y = findMin(z->right);
        if (y->parent != z) {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    delete z;
    return true;
}

void BST::traverse(std::function<void(BSTNode*)> visit) const {
    std::function<void(BSTNode*)> inorder = [&](BSTNode* n) {
        if (n) {
            inorder(n->left);
            visit(n);
            inorder(n->right);
        }
    };
    inorder(root);
}

BSTNode* BST::cloneTree(BSTNode* node, BSTNode* parent) {
    if (!node) return nullptr;
    BSTNode* n = new BSTNode(node->key);
    n->parent = parent;
    n->left = cloneTree(node->left, n);
    n->right = cloneTree(node->right, n);
    return n;
}

bool BST::insertSubtree(BSTNode* subtree) {
    if (!subtree) return false;
    BSTNode* clone = cloneTree(subtree, nullptr);
    if (!root) {
        root = clone;
        return true;
    }
    BSTNode* cur = root;
    BSTNode* parent = nullptr;
    while (cur) {
        parent = cur;
        if (clone->key < cur->key)
            cur = cur->left;
        else if (clone->key > cur->key)
            cur = cur->right;
        else {
            clearTree(clone);
            return false;
        }
    }
    clone->parent = parent;
    if (clone->key < parent->key)
        parent->left = clone;
    else
        parent->right = clone;
    return true;
}

BSTNode* BST::removeSubtree(int key) {
    BSTNode* node = find(key);
    if (!node) return nullptr;
    if (node->parent) {
        if (node == node->parent->left)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
    } else {
        root = nullptr;
    }
    node->parent = nullptr;
    return node;
}

bool BST::insertBranch(int parentKey, int key, bool asLeft) {
    BSTNode* parent = find(parentKey);
    if (!parent) return false;
    BSTNode** target = asLeft ? &parent->left : &parent->right;
    if (*target) return false;
    BSTNode* node = new BSTNode(key);
    node->parent = parent;
    *target = node;
    return true;
}

BSTNode* BST::removeBranch(int parentKey, bool leftBranch) {
    BSTNode* parent = find(parentKey);
    if (!parent) return nullptr;
    BSTNode** target = leftBranch ? &parent->left : &parent->right;
    BSTNode* branch = *target;
    if (!branch) return nullptr;
    *target = nullptr;
    branch->parent = nullptr;
    return branch;
}

bool BST::insertAtPosition(int parentKey, int key, bool asLeft) {
    return insertBranch(parentKey, key, asLeft);
}

int BST::lowestCommonAncestor(int key1, int key2) const {
    BSTNode* n1 = find(key1);
    BSTNode* n2 = find(key2);
    if (!n1 || !n2) throw std::runtime_error("Один или оба ключа не найдены");

    QList<BSTNode*> path;
    BSTNode* cur = n1;
    while (cur) {
        path.prepend(cur);
        cur = cur->parent;
    }

    cur = n2;
    while (cur) {
        if (path.contains(cur))
            return cur->key;
        cur = cur->parent;
    }
    return root->key; 
}