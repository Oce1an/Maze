#include "bst.h"
#include <algorithm>

BST::BST() : root(nullptr) {}

BST::~BST() {
    clear();
}

void BST::clear() {
    clearTree(root);
    root = nullptr;
}

void BST::clearTree(TreeNode *node) {
    if (node) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

void BST::insert(int key, const QString &str) {
    insertNode(root, key, str);
}

void BST::insertNode(TreeNode *&node, int key, const QString &str) {
    if (!node) {
        node = new TreeNode(key, str);
    } else if (key < node->key) {
        insertNode(node->left, key, str);
    } else if (key > node->key) {
        insertNode(node->right, key, str);
    } else {
        node->str = str;
    }
}

void BST::buildFromTable(QTableWidget *table) {
    clear();
    for (int row = 0; row < table->rowCount(); ++row) {
        QTableWidgetItem *keyItem = table->item(row, 0);
        QTableWidgetItem *strItem = table->item(row, 1);
        if (keyItem && strItem) {
            int key = keyItem->text().toInt();
            QString str = strItem->text();
            insert(key, str);
        }
    }
}

TreeNode* BST::search(int key) const {
    return searchNode(root, key);
}

TreeNode* BST::searchNode(TreeNode *node, int key) const {
    if (!node || node->key == key)
        return node;
    if (key < node->key)
        return searchNode(node->left, key);
    else
        return searchNode(node->right, key);
}

bool BST::remove(int key) {
    bool deleted = false;
    root = removeNode(root, key, deleted);
    return deleted;
}

TreeNode* BST::removeNode(TreeNode *node, int key, bool &deleted) {
    if (!node) return nullptr;
    if (key < node->key) {
        node->left = removeNode(node->left, key, deleted);
    } else if (key > node->key) {
        node->right = removeNode(node->right, key, deleted);
    } else {
        deleted = true;
        if (!node->left) {
            TreeNode *right = node->right;
            delete node;
            return right;
        } else if (!node->right) {
            TreeNode *left = node->left;
            delete node;
            return left;
        } else {
            TreeNode *minNode = findMin(node->right);
            node->key = minNode->key;
            node->str = minNode->str;
            node->right = removeNode(node->right, minNode->key, deleted);
        }
    }
    return node;
}

TreeNode* BST::findMin(TreeNode *node) const {
    while (node && node->left)
        node = node->left;
    return node;
}

void BST::balance() {
    QList<TreeNode*> nodes;
    inorderArray(root, nodes);
    clearTree(root);
    root = buildBalanced(nodes, 0, nodes.size() - 1);
}

void BST::inorderArray(TreeNode *node, QList<TreeNode*> &list) const {
    if (node) {
        inorderArray(node->left, list);
        list.append(node);
        inorderArray(node->right, list);
    }
}

TreeNode* BST::buildBalanced(QList<TreeNode*> &list, int start, int end) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    TreeNode *node = list[mid];
    node->left = buildBalanced(list, start, mid - 1);
    node->right = buildBalanced(list, mid + 1, end);
    return node;
}

QString BST::preorder() const {
    QString result;
    preorderRec(root, result);
    return result;
}

void BST::preorderRec(TreeNode *node, QString &result) const {
    if (node) {
        result += QString("[%1: %2] ").arg(node->key).arg(node->str);
        preorderRec(node->left, result);
        preorderRec(node->right, result);
    }
}

QString BST::inorder() const {
    QString result;
    inorderRec(root, result);
    return result;
}

void BST::inorderRec(TreeNode *node, QString &result) const {
    if (node) {
        inorderRec(node->left, result);
        result += QString("[%1: %2] ").arg(node->key).arg(node->str);
        inorderRec(node->right, result);
    }
}

QString BST::postorder() const {
    QString result;
    postorderRec(root, result);
    return result;
}

void BST::postorderRec(TreeNode *node, QString &result) const {
    if (node) {
        postorderRec(node->left, result);
        postorderRec(node->right, result);
        result += QString("[%1: %2] ").arg(node->key).arg(node->str);
    }
}

void BST::populateTreeWidget(QTreeWidget *treeWidget) const {
    treeWidget->clear();
    if (root) {
        QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
        rootItem->setText(0, QString("%1: %2").arg(root->key).arg(root->str));
        populateTreeRec(root->left, rootItem);
        populateTreeRec(root->right, rootItem);
    }
}

void BST::populateTreeRec(TreeNode *node, QTreeWidgetItem *parent) const {
    if (node) {
        QTreeWidgetItem *item = new QTreeWidgetItem(parent);
        item->setText(0, QString("%1: %2").arg(node->key).arg(node->str));
        populateTreeRec(node->left, item);
        populateTreeRec(node->right, item);
    }
}

int ExtendedBST::totalStringLength() const {
    int total = 0;
    std::function<void(TreeNode*)> sumLength = [&](TreeNode *node) {
        if (node) {
            total += node->str.length();
            sumLength(node->left);
            sumLength(node->right);
        }
    };
    sumLength(root);
    return total;
}