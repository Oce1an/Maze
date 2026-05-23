#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QStringList>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Деканат - Управление студентами");
    
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels({
        "Группа", "ФИО", "Зимние оценки", "Ср. балл (зима)", 
        "Летние оценки", "Ср. балл (лето)", "Статус"
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLoadFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Открыть файл со студентами", QDir::homePath(),
        "Text Files (*.txt);;All Files (*)"
    );
    
    if (fileName.isEmpty()) return;
    currentFileName = fileName;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }
    
    students.clear();
    QTextStream in(&file);
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            students.push_back(Student::fromString(line.toStdString()));
        }
    }
    file.close();
    
    fillTable();
    QMessageBox::information(this, "Успех", 
                            QString("Загружено %1 студентов").arg(students.size()));
}

void MainWindow::on_btnSaveFile_clicked()
{
    if (students.empty()) {
        QMessageBox::warning(this, "Предупреждение", "Нет данных для сохранения");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(
        this, "Сохранить файл со студентами", QDir::homePath(),
        "Text Files (*.txt)"
    );
    
    if (fileName.isEmpty()) return;
    currentFileName = fileName;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }
    
    QTextStream out(&file);
    for (const auto& student : students) {
        out << QString::fromStdString(student.toString()) << "\n";
    }
    file.close();
    
    QMessageBox::information(this, "Успех", "Файл успешно сохранен");
}

void MainWindow::on_btnAddStudent_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить студента");
    dialog.setMinimumWidth(400);
    
    QFormLayout form(&dialog);
    
    QLineEdit *groupEdit = new QLineEdit(&dialog);
    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QLineEdit *winterEdit = new QLineEdit(&dialog);
    winterEdit->setPlaceholderText("Например: 8,7,9,6,8");
    QLineEdit *summerEdit = new QLineEdit(&dialog);
    summerEdit->setPlaceholderText("Например: 9,8,7,10,9");
    
    form.addRow("Номер группы:", groupEdit);
    form.addRow("ФИО студента:", nameEdit);
    form.addRow("Оценки (зима):", winterEdit);
    form.addRow("Оценки (лето):", summerEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString group = groupEdit->text();
        QString name = nameEdit->text();
        QString winterStr = winterEdit->text();
        QString summerStr = summerEdit->text();
        
        if (group.isEmpty() || name.isEmpty() || winterStr.isEmpty() || summerStr.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
            return;
        }
        
        std::vector<int> winterGrades;
        QStringList winterList = winterStr.split(',');
        for (const QString& grade : winterList) {
            winterGrades.push_back(grade.trimmed().toInt());
        }
        
        std::vector<int> summerGrades;
        QStringList summerList = summerStr.split(',');
        for (const QString& grade : summerList) {
            summerGrades.push_back(grade.trimmed().toInt());
        }
        
        Student student(group.toStdString(), name.toStdString(), 
                       winterGrades, summerGrades);
        students.push_back(student);
        fillTable();
        
        if (!currentFileName.isEmpty()) {
            QFile file(currentFileName);
            if (file.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&file);
                out << QString::fromStdString(student.toString()) << "\n";
                file.close();
            }
        }
        
        QMessageBox::information(this, "Успех", "Студент добавлен");
    }
}

void MainWindow::on_btnEditStudent_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(students.size())) {
        QMessageBox::warning(this, "Предупреждение", "Выберите студента для редактирования");
        return;
    }
    
    Student& student = students[currentRow];
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать студента");
    dialog.setMinimumWidth(400);
    
    QFormLayout form(&dialog);
    
    QLineEdit *groupEdit = new QLineEdit(QString::fromStdString(student.getGroupNumber()), &dialog);
    QLineEdit *nameEdit = new QLineEdit(QString::fromStdString(student.getFullName()), &dialog);
    
    QString winterStr;
    auto winterGrades = student.getWinterGrades();
    for (size_t i = 0; i < winterGrades.size(); ++i) {
        winterStr += QString::number(winterGrades[i]);
        if (i < winterGrades.size() - 1) winterStr += ",";
    }
    
    QString summerStr;
    auto summerGrades = student.getSummerGrades();
    for (size_t i = 0; i < summerGrades.size(); ++i) {
        summerStr += QString::number(summerGrades[i]);
        if (i < summerGrades.size() - 1) summerStr += ",";
    }
    
    QLineEdit *winterEdit = new QLineEdit(winterStr, &dialog);
    QLineEdit *summerEdit = new QLineEdit(summerStr, &dialog);
    
    form.addRow("Номер группы:", groupEdit);
    form.addRow("ФИО студента:", nameEdit);
    form.addRow("Оценки (зима):", winterEdit);
    form.addRow("Оценки (лето):", summerEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        student.setGroupNumber(groupEdit->text().toStdString());
        student.setFullName(nameEdit->text().toStdString());
        
        std::vector<int> newWinter;
        QStringList winterList = winterEdit->text().split(',');
        for (const QString& grade : winterList) {
            newWinter.push_back(grade.trimmed().toInt());
        }
        student.setWinterGrades(newWinter);
        
        std::vector<int> newSummer;
        QStringList summerList = summerEdit->text().split(',');
        for (const QString& grade : summerList) {
            newSummer.push_back(grade.trimmed().toInt());
        }
        student.setSummerGrades(newSummer);
        
        fillTable();
        QMessageBox::information(this, "Успех", "Данные студента обновлены");
    }
}

void MainWindow::on_btnDeleteStudent_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(students.size())) {
        QMessageBox::warning(this, "Предупреждение", "Выберите студента для удаления");
        return;
    }
    
    auto btn = QMessageBox::question(this, "Подтверждение удаления",
                                    QString("Удалить студента: %1?")
                                        .arg(QString::fromStdString(students[currentRow].getFullName())),
                                    QMessageBox::Yes | QMessageBox::No);
    
    if (btn == QMessageBox::Yes) {
        students.erase(students.begin() + currentRow);
        fillTable();
        QMessageBox::information(this, "Успех", "Студент удален");
    }
}

void MainWindow::on_btnSearch_clicked()
{
    QString searchText = ui->lineEditSearch->text();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите ФИО для поиска");
        return;
    }
    
    std::vector<Student> found;
    for (const auto& student : students) {
        QString name = QString::fromStdString(student.getFullName());
        if (name.contains(searchText, Qt::CaseInsensitive)) {
            found.push_back(student);
        }
    }
    
    if (found.empty()) {
        QMessageBox::information(this, "Результат поиска", "Студенты не найдены");
        return;
    }
    
    fillTable(found);
    QMessageBox::information(this, "Результат поиска", 
                            QString("Найдено %1 студентов").arg(found.size()));
}

void MainWindow::on_btnSortByName_clicked()
{
    if (students.empty()) {
        QMessageBox::warning(this, "Предупреждение", "Нет данных для сортировки");
        return;
    }
    
    sortStudentsByNameInGroups();
    fillTable();
    QMessageBox::information(this, "Успех", "Студенты отсортированы по ФИО внутри групп");
}

void MainWindow::on_btnWinterDebtors_clicked()
{
    std::vector<Student> debtors = getWinterDebtors();

    if (debtors.empty()) {
        QMessageBox::information(this, "Задолжники",
            "Задолжников по зимней сессии нет");
        return;
    }

    QString result = "Список задолжников по зимней сессии:\n\n";

    result += QString("%1 %2 %3 %4\n")
        .arg(QString("Группа").leftJustified(10, ' '))
        .arg(QString("ФИО").leftJustified(30, ' '))
        .arg(QString("Оценки (зима)").leftJustified(20, ' '))
        .arg(QString("Долгов").leftJustified(10, ' '));

    result += QString(75, '=') + "\n";

    for (const auto& student : debtors) {
        QString grades;
        auto winterGrades = student.getWinterGrades();
        for (size_t i = 0; i < winterGrades.size(); ++i) {
            grades += QString::number(winterGrades[i]);
            if (i < winterGrades.size() - 1) grades += ",";
        }

        result += QString("%1 %2 %3 %4\n")
            .arg(QString::fromStdString(student.getGroupNumber()).leftJustified(10, ' '))
            .arg(QString::fromStdString(student.getFullName()).leftJustified(30, ' '))
            .arg(grades.leftJustified(20, ' '))
            .arg(QString::number(student.getWinterDebtCount()).leftJustified(10, ' '));
    }

    ResultDialog* dialog = new ResultDialog(this);
    dialog->setResult(result, QString("Найдено %1 задолжников").arg(debtors.size()));
    dialog->exec();
    delete dialog;
}

void MainWindow::on_btnExpelList_clicked()
{
    std::vector<Student> forExpel = getStudentsForExpel();

    if (forExpel.empty()) {
        QMessageBox::information(this, "Список на отчисление",
            "Студентов на отчисление нет");
        return;
    }

    QString result = "Список студентов на отчисление:\n\n";
    result += "Критерии: 2+ неудовлетворительные оценки за летнюю сессию\n";
    result += "и наличие задолженностей за зимнюю сессию\n\n";

    result += QString("%1 %2 %3 %4\n")
        .arg(QString("Группа").leftJustified(10, ' '))
        .arg(QString("ФИО").leftJustified(30, ' '))
        .arg(QString("Долгов (зима)").leftJustified(15, ' '))
        .arg(QString("Долгов (лето)").leftJustified(15, ' '));

    result += QString(75, '=') + "\n";

    for (const auto& student : forExpel) {
        result += QString("%1 %2 %3 %4\n")
            .arg(QString::fromStdString(student.getGroupNumber()).leftJustified(10, ' '))
            .arg(QString::fromStdString(student.getFullName()).leftJustified(30, ' '))
            .arg(QString::number(student.getWinterDebtCount()).leftJustified(15, ' '))
            .arg(QString::number(student.getSummerDebtCount()).leftJustified(15, ' '));
    }

    ResultDialog* dialog = new ResultDialog(this);
    dialog->setResult(result, QString("Найдено %1 студентов на отчисление").arg(forExpel.size()));
    dialog->exec();
    delete dialog;
}

void MainWindow::on_btnClearTable_clicked()
{
    students.clear();
    ui->tableWidget->setRowCount(0);
    currentFileName.clear();
    QMessageBox::information(this, "Успех", "Таблица очищена");
}

void MainWindow::fillTable(const std::vector<Student>& studentList)
{
    ui->tableWidget->setRowCount(0);
    
    for (const auto& student : studentList) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(student.getGroupNumber())));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.getFullName())));
        
        QString winterGrades;
        auto wGrades = student.getWinterGrades();
        for (size_t i = 0; i < wGrades.size(); ++i) {
            winterGrades += QString::number(wGrades[i]);
            if (i < wGrades.size() - 1) winterGrades += ", ";
        }
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(winterGrades));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(student.getWinterAverage(), 'f', 2)));
        
        QString summerGrades;
        auto sGrades = student.getSummerGrades();
        for (size_t i = 0; i < sGrades.size(); ++i) {
            summerGrades += QString::number(sGrades[i]);
            if (i < sGrades.size() - 1) summerGrades += ", ";
        }
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(summerGrades));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(student.getSummerAverage(), 'f', 2)));
        
        QString status;
        if (student.shouldBeExpelled()) {
            status = "На отчисление";
        } else if (student.hasWinterDebts()) {
            status = "Задолжник (зима)";
        } else {
            status = "Успевает";
        }
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(status));
    }
}

void MainWindow::fillTable()
{
    fillTable(students);
}

std::vector<Student> MainWindow::getWinterDebtors()
{
    std::vector<Student> debtors;
    for (const auto& student : students) {
        if (student.hasWinterDebts()) {
            debtors.push_back(student);
        }
    }
    return debtors;
}

std::vector<Student> MainWindow::getStudentsForExpel()
{
    std::vector<Student> forExpel;
    for (const auto& student : students) {
        if (student.shouldBeExpelled()) {
            forExpel.push_back(student);
        }
    }
    return forExpel;
}

void MainWindow::sortStudentsByNameInGroups()
{
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  if (a.getGroupNumber() != b.getGroupNumber()) {
                      return a.getGroupNumber() < b.getGroupNumber();
                  }
                  return a.getFullName() < b.getFullName();
              });
}