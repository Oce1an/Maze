#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Дерево поиска");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *lblTable = new QLabel("Исходный массив (ключ, строка):");
    mainLayout->addWidget(lblTable);
    table = new QTableWidget(0, 2);
    table->setHorizontalHeaderLabels({"Ключ", "Строка"});
    table->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(table);

    QHBoxLayout *tableBtns = new QHBoxLayout();
    QPushButton *btnAddRow = new QPushButton("Добавить строку");
    QPushButton *btnDelRow = new QPushButton("Удалить строку");
    QPushButton *btnLoad = new QPushButton("Загрузить в дерево");
    connect(btnAddRow, &QPushButton::clicked, [this](){
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(""));
        table->setItem(row, 1, new QTableWidgetItem(""));
    });
    connect(btnDelRow, &QPushButton::clicked, [this](){
        int row = table->currentRow();
        if (row >= 0) table->removeRow(row);
    });
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadFromTable);
    tableBtns->addWidget(btnAddRow);
    tableBtns->addWidget(btnDelRow);
    tableBtns->addWidget(btnLoad);
    mainLayout->addLayout(tableBtns);

    QLabel *lblTree = new QLabel("Дерево (TreeView):");
    mainLayout->addWidget(lblTree);
    treeView = new QTreeWidget();
    treeView->setHeaderHidden(true);
    mainLayout->addWidget(treeView);

    QPushButton *btnBalance = new QPushButton("Сбалансировать дерево");
    connect(btnBalance, &QPushButton::clicked, this, &MainWindow::balanceTree);
    mainLayout->addWidget(btnBalance);

    QHBoxLayout *addLayout = new QHBoxLayout();
    addKeyInput = new QSpinBox();
    addKeyInput->setRange(-999999, 999999);
    addKeyInput->setPrefix("Ключ: ");
    addStrInput = new QLineEdit();
    addStrInput->setPlaceholderText("Строка");
    QPushButton *btnAdd = new QPushButton("Добавить");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addRecord);
    addLayout->addWidget(addKeyInput);
    addLayout->addWidget(addStrInput);
    addLayout->addWidget(btnAdd);
    mainLayout->addLayout(addLayout);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchKeyInput = new QSpinBox();
    searchKeyInput->setRange(-999999, 999999);
    searchKeyInput->setPrefix("Поиск по ключу: ");
    QPushButton *btnSearch = new QPushButton("Найти");
    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::searchByKey);
    searchLayout->addWidget(searchKeyInput);
    searchLayout->addWidget(btnSearch);
    mainLayout->addLayout(searchLayout);

    QHBoxLayout *deleteLayout = new QHBoxLayout();
    deleteKeyInput = new QSpinBox();
    deleteKeyInput->setRange(-999999, 999999);
    deleteKeyInput->setPrefix("Удалить по ключу: ");
    QPushButton *btnDelete = new QPushButton("Удалить");
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteByKey);
    deleteLayout->addWidget(deleteKeyInput);
    deleteLayout->addWidget(btnDelete);
    mainLayout->addLayout(deleteLayout);

    QHBoxLayout *travLayout = new QHBoxLayout();
    QPushButton *btnPre = new QPushButton("Прямой обход");
    connect(btnPre, &QPushButton::clicked, this, &MainWindow::showPreorder);
    QPushButton *btnIn = new QPushButton("По возрастанию ключа");
    connect(btnIn, &QPushButton::clicked, this, &MainWindow::showInorder);
    QPushButton *btnPost = new QPushButton("Обратный обход");
    connect(btnPost, &QPushButton::clicked, this, &MainWindow::showPostorder);
    travLayout->addWidget(btnPre);
    travLayout->addWidget(btnIn);
    travLayout->addWidget(btnPost);
    mainLayout->addLayout(travLayout);

    QPushButton *btnTotalStr = new QPushButton("Суммарная длина строк (производный класс)");
    connect(btnTotalStr, &QPushButton::clicked, this, &MainWindow::showTotalStringLength);
    mainLayout->addWidget(btnTotalStr);

    output = new QTextEdit();
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    table->setRowCount(5);
    QStringList keys = {"100", "50", "150", "30", "70"};
    QStringList strs = {"Иванов И.И.", "Петров П.П.", "Сидоров С.С.", "Козлов К.К.", "Смирнов А.А."};
    for (int i = 0; i < 5; ++i) {
        table->setItem(i, 0, new QTableWidgetItem(keys[i]));
        table->setItem(i, 1, new QTableWidgetItem(strs[i]));
    }
    loadFromTable();
}

MainWindow::~MainWindow() {}

void MainWindow::log(const QString &text) {
    output->append(text);
}

void MainWindow::refreshTreeWidget() {
    bst.populateTreeWidget(treeView);
}

void MainWindow::loadFromTable() {
    bst.buildFromTable(table);
    refreshTreeWidget();
    log("Дерево построено из таблицы.");
}

void MainWindow::balanceTree() {
    bst.balance();
    refreshTreeWidget();
    log("Дерево сбалансировано.");
}

void MainWindow::addRecord() {
    int key = addKeyInput->value();
    QString str = addStrInput->text().trimmed();
    if (str.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите строку.");
        return;
    }
    bst.insert(key, str);
    refreshTreeWidget();
    log(QString("Добавлена запись: [%1: %2]").arg(key).arg(str));
}

void MainWindow::searchByKey() {
    int key = searchKeyInput->value();
    TreeNode *found = bst.search(key);
    if (found) {
        QMessageBox::information(this, "Найдено", QString("Ключ: %1\nСтрока: %2").arg(found->key).arg(found->str));
        log(QString("Найден узел: [%1: %2]").arg(found->key).arg(found->str));
    } else {
        QMessageBox::information(this, "Не найдено", "Запись с таким ключом отсутствует.");
        log("Поиск: ключ не найден.");
    }
}

void MainWindow::deleteByKey() {
    int key = deleteKeyInput->value();
    bool deleted = bst.remove(key);
    if (deleted) {
        refreshTreeWidget();
        log(QString("Удалён узел с ключом %1").arg(key));
    } else {
        QMessageBox::information(this, "Ошибка", "Ключ не найден.");
        log("Удаление: ключ не найден.");
    }
}

void MainWindow::showPreorder() {
    QString res = bst.preorder();
    log("Прямой обход: " + res);
}

void MainWindow::showInorder() {
    QString res = bst.inorder();
    log("По возрастанию ключа: " + res);
}

void MainWindow::showPostorder() {
    QString res = bst.postorder();
    log("Обратный обход: " + res);
}

void MainWindow::showTotalStringLength() {
    int total = bst.totalStringLength();
    log(QString("Суммарная длина строк во всех узлах дерева: %1 символов").arg(total));
}