#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "date.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTableWidget>
#include <QDate>
#include <QLocale>
#include <QKeyEvent>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({
        "Original Date",
        "Next Day",
        "Difference",
        "Week Number"
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveDatesToFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to create file for writing");
        return;
    }
    
    QTextStream out(&file);
    for (size_t i = 0; i < dates.size(); ++i) {
        out << QString::fromStdString(dates[i].toString());
        if (i < dates.size() - 1) {
            out << " ";
        }
    }
    
    file.close();
}

void MainWindow::on_btnSaveAs_clicked()
{
    if (dates.empty()) {
        QMessageBox::warning(this, "Warning", "No dates to save");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Dates File",
        QDir::homePath(),
        "Text Files (*.txt);;All Files (*)"
        );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    if (!fileName.endsWith(".txt", Qt::CaseInsensitive)) {
        fileName += ".txt";
    }
    
    saveDatesToFile(fileName);
    currentFileName = fileName;
    
    QMessageBox::information(this, "Success", 
                            QString("Saved %1 dates to file").arg(dates.size()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier) {
        if (!dates.empty()) {
            if (currentFileName.isEmpty()) {
                on_btnSaveAs_clicked();
            } else {
                saveDatesToFile(currentFileName);
                QMessageBox::information(this, "Success", "File saved");
            }
        }
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::on_btnOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Dates File",
        QDir::homePath(),
        "Text Files (*.txt);;All Files (*)"
        );

    if (fileName.isEmpty()) return;
    currentFileName = fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open file");
        return;
    }

    dates.clear();
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QStringList dateStrings = content.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    ui->tableWidget->setRowCount(0);

    for (const QString& str : dateStrings) {
        try {
            Date d(str.toStdString());
            dates.push_back(d);

            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0,
                                     new QTableWidgetItem(QString::fromStdString(d.toString())));

            Date next = d.NextDay();
            ui->tableWidget->setItem(row, 1,
                                     new QTableWidgetItem(QString::fromStdString(next.toString())));

            short weekNum = d.WeekNumber();
            ui->tableWidget->setItem(row, 3,
                                     new QTableWidgetItem(QString::number(weekNum)));

        } catch (const InvalidDateException& e) {
            QMessageBox::warning(this, "Format Error",
                                 QString("Invalid date in file: %1").arg(str));
        }
    }

    for (size_t i = 0; i < dates.size(); ++i) {
        if (i < dates.size() - 1) {
            int diff = dates[i].Duration(dates[i+1]);
            ui->tableWidget->setItem(i, 2,
                                     new QTableWidgetItem(QString::number(diff)));
        } else {
            ui->tableWidget->setItem(i, 2,
                                     new QTableWidgetItem("-"));
        }
    }

    QMessageBox::information(this, "Success",
                             QString("Loaded %1 dates").arg(dates.size()));
}

void MainWindow::on_btnAddDate_clicked()
{
    QString input = ui->lineEditNewDate->text();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Enter date in DD.MM.YYYY format");
        return;
    }

    try {
        Date newDate(input.toStdString());

        if (currentFileName.isEmpty()) {
            currentFileName = QFileDialog::getSaveFileName(
                this, "Save Dates File",
                QDir::homePath(), "Text Files (*.txt)"
                );
            if (currentFileName.isEmpty()) return;
        }

        QFile file(currentFileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Failed to open file for writing");
            return;
        }

        QTextStream out(&file);
        out << " " << QString::fromStdString(newDate.toString());
        file.close();

        dates.push_back(newDate);
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0,
                                 new QTableWidgetItem(QString::fromStdString(newDate.toString())));
        ui->tableWidget->setItem(row, 1,
                                 new QTableWidgetItem(QString::fromStdString(newDate.NextDay().toString())));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem("-"));
        ui->tableWidget->setItem(row, 3,
                                 new QTableWidgetItem(QString::number(newDate.WeekNumber())));

        QMessageBox::information(this, "Success", "Date added to file");

    } catch (...) {
        QMessageBox::critical(this, "Error",
                              "Invalid date format. Use DD.MM.YYYY");
    }
}

void MainWindow::on_btnModifyDate_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(dates.size())) {
        QMessageBox::warning(this, "Warning", "Select a row to modify");
        return;
    }

    QString newInput = ui->lineEditNewDate->text();
    if (newInput.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Enter new date");
        return;
    }

    try {
        Date newDate(newInput.toStdString());

        dates[currentRow] = newDate;

        ui->tableWidget->setItem(currentRow, 0,
                                 new QTableWidgetItem(QString::fromStdString(newDate.toString())));

        Date nextDay = newDate.NextDay();
        ui->tableWidget->setItem(currentRow, 1,
                                 new QTableWidgetItem(QString::fromStdString(nextDay.toString())));

        short weekNum = newDate.WeekNumber();
        ui->tableWidget->setItem(currentRow, 3,
                                 new QTableWidgetItem(QString::number(weekNum)));

        if (currentRow > 0) {
            int diff = dates[currentRow - 1].Duration(dates[currentRow]);
            ui->tableWidget->setItem(currentRow - 1, 2,
                                     new QTableWidgetItem(QString::number(diff)));
        }
        if (currentRow < static_cast<int>(dates.size()) - 1) {
            int diff = dates[currentRow].Duration(dates[currentRow + 1]);
            ui->tableWidget->setItem(currentRow, 2,
                                     new QTableWidgetItem(QString::number(diff)));
        } else {
            ui->tableWidget->setItem(currentRow, 2,
                                     new QTableWidgetItem("-"));
        }

        if (!currentFileName.isEmpty()) {
            saveDatesToFile(currentFileName);
        }

        QMessageBox::information(this, "Success", "Date modified");

    } catch (...) {
        QMessageBox::critical(this, "Error", "Invalid date format");
    }
}

void MainWindow::on_btnShowPreviousDay_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(dates.size())) {
        QMessageBox::warning(this, "Warning", "Select a row in the table");
        return;
    }

    try {
        Date prev = dates[currentRow].PreviousDay();

        QMessageBox::information(this, "Previous Day",
                                 QString("For date %1\nPrevious day: %2")
                                     .arg(QString::fromStdString(dates[currentRow].toString()))
                                     .arg(QString::fromStdString(prev.toString())));

    } catch (...) {
        QMessageBox::critical(this, "Error", "Calculation error");
    }
}

void MainWindow::on_btnCalculateDuration_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(dates.size())) {
        QMessageBox::warning(this, "Warning", "Select a date in the table");
        return;
    }

    try {
        Date today;
        Date selectedDate = dates[currentRow];
        int daysDiff = today.Duration(selectedDate);

        QMessageBox::information(this, "Date Difference",
                                 QString("Current date: %1\n"
                                         "Date from file: %2\n"
                                         "Days between: %3")
                                     .arg(QString::fromStdString(today.toString()))
                                     .arg(QString::fromStdString(selectedDate.toString()))
                                     .arg(daysDiff));

    } catch (...) {
        QMessageBox::critical(this, "Error", "Calculation error");
    }
}

void MainWindow::on_btnShowCurrentDate_clicked()
{
    Date today;
    QMessageBox::information(this, "Current Date",
                             QString("Today: %1\nWeek number: %2")
                                 .arg(QString::fromStdString(today.toString()))
                                 .arg(today.WeekNumber()));
}

void MainWindow::on_btnCalculateBirthday_clicked()
{
    QString birthdayInput = ui->lineEditBirthday->text();
    if (birthdayInput.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Enter birthday (DD.MM.YYYY)");
        return;
    }

    try {
        Date birthday(birthdayInput.toStdString());
        Date today;
        int daysLeft = today.DaysTillYourBithday(birthday);

        QMessageBox::information(this, "Birthday",
                                 QString("Days until your birthday: %1")
                                     .arg(daysLeft));

    } catch (...) {
        QMessageBox::critical(this, "Error", "Invalid birthday format");
    }
}