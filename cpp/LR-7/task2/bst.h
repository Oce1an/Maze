#ifndef BST_H
#define BST_H

#include <QString>
#include <QList>
#include <QTableWidget>
#include <QTreeWidget>

struct TreeNode {
    int key;
    QString str;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int k, const QString &s) : key(k), str(s), left(nullptr), right(nullptr) {}
};

class BST {
public:
    BST();
    virtual ~BST();

    void insert(int key, const QString &str);
    void buildFromTable(QTableWidget *table);
    void balance();
    TreeNode* search(int key) const;
    bool remove(int key);
    void clear();

    QString preorder() const;
    QString inorder() const;
    QString postorder() const;

    void populateTreeWidget(QTreeWidget *treeWidget) const;

protected:
    TreeNode *root;

    void insertNode(TreeNode *&node, int key, const QString &str);
    TreeNode* searchNode(TreeNode *node, int key) const;
    TreeNode* removeNode(TreeNode *node, int key, bool &deleted);
    TreeNode* findMin(TreeNode *node) const;
    void clearTree(TreeNode *node);
    void inorderArray(TreeNode *node, QList<TreeNode*> &list) const;
    TreeNode* buildBalanced(QList<TreeNode*> &list, int start, int end);

    void preorderRec(TreeNode *node, QString &result) const;
    void inorderRec(TreeNode *node, QString &result) const;
    void postorderRec(TreeNode *node, QString &result) const;
    void populateTreeRec(TreeNode *node, QTreeWidgetItem *parent) const;
};

class ExtendedBST : public BST {
public:
    ExtendedBST() : BST() {}

    int totalStringLength() const;
};

#endif