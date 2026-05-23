#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QLabel>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Бинарное дерево поиска — Лаб. 8");
    resize(700, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    treeWidget = new QTreeWidget();
    treeWidget->setHeaderLabel("Ключ");
    mainLayout->addWidget(treeWidget, 1);

    output = new QTextEdit();
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    QHBoxLayout* keyLayout = new QHBoxLayout();
    spinKey = new QSpinBox();  spinKey->setRange(-999, 999); spinKey->setValue(50);
    spinKey2 = new QSpinBox(); spinKey2->setRange(-999, 999); spinKey2->setValue(75);
    spinParent = new QSpinBox(); spinParent->setRange(-999, 999); spinParent->setValue(50);
    checkAsLeft = new QCheckBox("Левый");
    checkAsLeft->setChecked(true);
    keyLayout->addWidget(new QLabel("Ключ1:"));
    keyLayout->addWidget(spinKey);
    keyLayout->addWidget(new QLabel("Ключ2:"));
    keyLayout->addWidget(spinKey2);
    keyLayout->addWidget(new QLabel("Родитель:"));
    keyLayout->addWidget(spinParent);
    keyLayout->addWidget(checkAsLeft);
    mainLayout->addLayout(keyLayout);

    auto addButton = [&](const QString& text, auto slot) {
        QPushButton* btn = new QPushButton(text);
        connect(btn, &QPushButton::clicked, this, slot);
        return btn;
    };

    QHBoxLayout* row1 = new QHBoxLayout();
    row1->addWidget(addButton("Add", &MainWindow::onAdd));
    row1->addWidget(addButton("Remove", &MainWindow::onRemove));
    row1->addWidget(addButton("Find", &MainWindow::onFind));
    row1->addWidget(addButton("Traverse", &MainWindow::onTraverse));
    mainLayout->addLayout(row1);

    QHBoxLayout* row2 = new QHBoxLayout();
    row2->addWidget(addButton("Insert Subtree (key1 как корень)", &MainWindow::onInsertSubtree));
    row2->addWidget(addButton("Remove Subtree (key1)", &MainWindow::onRemoveSubtree));
    mainLayout->addLayout(row2);

    QHBoxLayout* row3 = new QHBoxLayout();
    row3->addWidget(addButton("Insert Branch (родитель, key1, лев/прав)", &MainWindow::onInsertBranch));
    row3->addWidget(addButton("Remove Branch (родитель, лев/прав)", &MainWindow::onRemoveBranch));
    mainLayout->addLayout(row3);

    QHBoxLayout* row4 = new QHBoxLayout();
    row4->addWidget(addButton("Insert At Position (родитель, key1, лев/прав)", &MainWindow::onInsertAtPosition));
    row4->addWidget(addButton("LCA (key1, key2)", &MainWindow::onLCA));
    mainLayout->addLayout(row4);

    for (int k : {50, 30, 70, 20, 40, 60, 80})
        tree.add(k);
    updateTreeView();
    output->append("Начальное дерево: 50, 30, 70, 20, 40, 60, 80");
}

MainWindow::~MainWindow() {}

void MainWindow::updateTreeView() {
    treeWidget->clear();
    if (tree.getRoot())
        addNodeToTree(nullptr, tree.getRoot());
    treeWidget->expandAll();
}

void MainWindow::addNodeToTree(QTreeWidgetItem* parent, BSTNode* node) {
    QTreeWidgetItem* item = parent ? new QTreeWidgetItem(parent) : new QTreeWidgetItem(treeWidget);
    item->setText(0, QString::number(node->key));
    if (node->left)  addNodeToTree(item, node->left);
    if (node->right) addNodeToTree(item, node->right);
}

void MainWindow::onAdd() {
    bool ok = tree.add(spinKey->value());
    output->append(ok ? QString("Добавлен %1").arg(spinKey->value())
                      : QString("Ключ %1 уже существует").arg(spinKey->value()));
    updateTreeView();
}

void MainWindow::onRemove() {
    bool ok = tree.remove(spinKey->value());
    output->append(ok ? QString("Удалён %1").arg(spinKey->value())
                      : QString("Ключ %1 не найден").arg(spinKey->value()));
    updateTreeView();
}

void MainWindow::onFind() {
    BSTNode* node = tree.find(spinKey->value());
    QMessageBox::information(this, "Поиск",
        node ? QString("Найден: %1").arg(node->key) : "Не найден");
}

void MainWindow::onTraverse() {
    QString s;
    tree.traverse([&](BSTNode* n) { s += QString::number(n->key) + " "; });
    output->append("Обход inorder: " + s);
}

void MainWindow::onInsertSubtree() {
    BST subtree;
    subtree.add(spinKey->value());
    subtree.add(spinKey->value() - 10);
    subtree.add(spinKey->value() + 10);
    bool ok = tree.insertSubtree(subtree.getRoot());
    output->append(ok ? QString("Поддерево с корнем %1 вставлено").arg(spinKey->value())
                      : "Ошибка вставки поддерева");
    updateTreeView();
}

void MainWindow::onRemoveSubtree() {
    BSTNode* removed = tree.removeSubtree(spinKey->value());
    if (removed) {
        output->append(QString("Поддерево с корнем %1 удалено").arg(spinKey->value()));
        std::function<void(BSTNode*)> del = [&](BSTNode* n) {
            if (n) { del(n->left); del(n->right); delete n; }
        };
        del(removed);
    } else {
        output->append("Ключ не найден");
    }
    updateTreeView();
}

void MainWindow::onInsertBranch() {
    bool ok = tree.insertBranch(spinParent->value(), spinKey->value(), checkAsLeft->isChecked());
    output->append(ok ? QString("Ветвь %1 добавлена как %2 потомок %3")
                            .arg(spinKey->value())
                            .arg(checkAsLeft->isChecked() ? "левый" : "правый")
                            .arg(spinParent->value())
                      : "Ошибка (родитель не найден или позиция занята)");
    updateTreeView();
}

void MainWindow::onRemoveBranch() {
    BSTNode* branch = tree.removeBranch(spinParent->value(), checkAsLeft->isChecked());
    if (branch) {
        output->append(QString("Ветвь удалена, корень: %1").arg(branch->key));
        std::function<void(BSTNode*)> del = [&](BSTNode* n) {
            if (n) { del(n->left); del(n->right); delete n; }
        };
        del(branch);
    } else {
        output->append("Ветвь не найдена");
    }
    updateTreeView();
}

void MainWindow::onInsertAtPosition() {
    onInsertBranch();
}

void MainWindow::onLCA() {
    try {
        int lca = tree.lowestCommonAncestor(spinKey->value(), spinKey2->value());
        output->append(QString("LCA(%1, %2) = %3").arg(spinKey->value()).arg(spinKey2->value()).arg(lca));
    } catch (std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}