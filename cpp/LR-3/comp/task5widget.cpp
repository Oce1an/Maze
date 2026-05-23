#include "task5widget.h"

Task5Widget::Task5Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Обход папок и файлов");

    QVBoxLayout *layout = new QVBoxLayout(this);

    selectBtn = new QPushButton("Выбрать главную папку", this);
    layout->addWidget(selectBtn);

    infoLabel = new QLabel("Папок: 0, Файлов: 0", this);
    infoLabel->setStyleSheet("font-weight: bold; margin: 5px;");
    layout->addWidget(infoLabel);

    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabels({"Имя", "Тип"});
    treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(treeWidget);

    connect(selectBtn, &QPushButton::clicked, this, &Task5Widget::selectFolder);
    connect(treeWidget, &QTreeWidget::itemDoubleClicked,
            this, &Task5Widget::openFile);
}

void Task5Widget::traverse(const QString &path, int &folderCount, int &fileCount,
                          QTreeWidgetItem *parentItem)
{
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden | QDir::System);

    for (const QFileInfo &info : entries) {
        if (info.isDir()) {
            folderCount++;

            QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
            item->setText(0, info.fileName());
            item->setText(1, "Папка");
            item->setData(0, Qt::UserRole, info.absoluteFilePath());

            traverse(info.absoluteFilePath(), folderCount, fileCount, item);
        } else {
            fileCount++;

            QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
            item->setText(0, info.fileName());
            item->setText(1, "Файл");
            item->setData(0, Qt::UserRole, info.absoluteFilePath());
        }
    }
}

void Task5Widget::selectFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку");
    if (folderPath.isEmpty())
        return;

    treeWidget->clear();
    int folderCount = 0;
    int fileCount = 0;

    QTreeWidgetItem *root = new QTreeWidgetItem(treeWidget);
    QDir dir(folderPath);
    root->setText(0, dir.dirName());
    root->setText(1, "Корневая папка");
    root->setData(0, Qt::UserRole, folderPath);

    traverse(folderPath, folderCount, fileCount, root);

    root->setExpanded(true);

    infoLabel->setText(QString("Подпапок: %1, Файлов: %2").arg(folderCount).arg(fileCount));
}

void Task5Widget::openFile(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QString filePath = item->data(0, Qt::UserRole).toString();
    if (filePath.isEmpty())
        return;

    QFileInfo info(filePath);
    if (info.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}