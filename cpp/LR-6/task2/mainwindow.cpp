#include "mainwindow.h"
#include "inputdialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <fstream>
#include <cstring>
#include <locale>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Файлы");
    resize(700, 500);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    output = new QTextEdit(this);
    output->setReadOnly(true);
    layout->addWidget(output);

    inputLine = new QLineEdit(this);
    inputLine->setPlaceholderText("Введите номер пункта меню и нажмите Enter");
    layout->addWidget(inputLine);

    QPushButton *btn = new QPushButton("Выполнить", this);
    layout->addWidget(btn);

    connect(inputLine, &QLineEdit::returnPressed, this, &MainWindow::executeCommand);
    connect(btn, &QPushButton::clicked, this, &MainWindow::executeCommand);

    showMenu();
}

MainWindow::~MainWindow() {}

void MainWindow::showMenu()
{
    output->append("Меню:\n"
                   "1. Ввести абзац текста и сохранить посимвольно (wchar_t) в ФамилияЛаб20.txt\n"
                   "2. Считать посимвольно из ФамилияЛаб20.txt и вывести\n"
                   "3. Ввести массив float и сохранить в ФамилияЛаб20_0.txt (разделитель '$')\n"
                   "4. Считать массив float из ФамилияЛаб20_0.txt и вывести\n"
                   "5. Ввести структуру Государство и дописать в текстовый ФамилияЛаб20_1.txt\n"
                   "6. Прочитать все структуры из ФамилияЛаб20_1.txt (вывод через '|')\n"
                   "7. Ввести структуру Государство и дописать в бинарный ФамилияЛаб20_2.bin\n"
                   "8. Прочитать все структуры из ФамилияЛаб20_2.bin (вывод через ' ; ')\n"
                   "9. Ввести предложение и дописать в ФамилияЛаб20_3.txt\n"
                   "10. Построчно считать ФамилияЛаб20_3.txt и вывести\n"
                   "0. Выход\n");
}

void MainWindow::executeCommand()
{
    bool ok;
    int cmd = inputLine->text().toInt(&ok);
    if (!ok) {
        output->append("Неверный ввод. Введите число.");
        return;
    }
    output->append(QString("Выполняется пункт %1").arg(cmd));

    switch (cmd) {
    case 1: case1_writeWcharText(); break;
    case 2: case2_readWcharText(); break;
    case 3: case3_writeFloatArray(); break;
    case 4: case4_readFloatArray(); break;
    case 5: case5_appendTextStruct(); break;
    case 6: case6_readTextStructs(); break;
    case 7: case7_appendBinaryStruct(); break;
    case 8: case8_readBinaryStructs(); break;
    case 9: case9_appendLine(); break;
    case 10: case10_readLines(); break;
    case 0: QApplication::quit(); break;
    default: output->append("Неизвестный пункт меню.");
    }
    inputLine->clear();
}

QString MainWindow::stateToString(const State &s)
{
    QString gdpStr;
    for (int i = 0; i < 3; ++i)
        gdpStr += QString::number(s.gdpGrowth[i]) + (i < 2 ? ", " : "");
    return QString("%1 | %2 | %3 | %4 | %5 | [%6]")
        .arg(s.population)
        .arg(s.area, 0, 'f', 2)
        .arg(s.continentCode)
        .arg(s.isUNMember ? "true" : "false")
        .arg(s.name)
        .arg(gdpStr);
}

State MainWindow::inputState()
{
    InputDialog dlg(this);
    State s;
    if (dlg.exec() == QDialog::Accepted) {
        s.population    = dlg.population();
        s.area          = dlg.area();
        s.continentCode = dlg.continentCode();
        s.isUNMember    = dlg.isUNMember();
        QByteArray nameUtf8 = dlg.name().toUtf8();
        std::strncpy(s.name, nameUtf8.constData(), sizeof(s.name)-1);
        s.name[sizeof(s.name)-1] = '\0';
        for (int i = 0; i < 3; ++i)
            s.gdpGrowth[i] = dlg.gdpGrowth(i);
    }
    return s;
}

void MainWindow::case1_writeWcharText()
{
    QString text = QInputDialog::getText(this, "Ввод текста", "Введите абзац текста:");
    if (text.isEmpty()) return;

    std::wofstream file("ФамилияЛаб20.txt", std::ios::out | std::ios::trunc);
    if (!file) {
        output->append("Ошибка открытия файла для записи.");
        return;
    }
    file.imbue(std::locale("en_US.UTF-8"));
    for (const QChar &qc : text) {
        wchar_t wc = qc.unicode();
        file << wc;
    }
    file.close();
    output->append("Текст сохранён посимвольно (wchar_t) в ФамилияЛаб20.txt");
}

void MainWindow::case2_readWcharText()
{
    std::wifstream file("ФамилияЛаб20.txt");
    if (!file) {
        output->append("Файл ФамилияЛаб20.txt не найден.");
        return;
    }
    file.imbue(std::locale("en_US.UTF-8"));
    QString result;
    wchar_t wc;
    while (file.get(wc)) {
        result += QChar(wc);
    }
    file.close();
    output->append("Содержимое ФамилияЛаб20.txt:\n" + result);
}

void MainWindow::case3_writeFloatArray()
{
    bool ok;
    int size = QInputDialog::getInt(this, "Размер", "Введите размер массива:", 5, 1, 100, 1, &ok);
    if (!ok) return;

    QVector<float> arr;
    for (int i = 0; i < size; ++i) {
        float val = QInputDialog::getDouble(this, "Элемент", QString("Введите элемент %1:").arg(i+1), 0, -1e6, 1e6, 2, &ok);
        if (!ok) return;
        arr.append(val);
    }

    QFile file("ФамилияЛаб20_0.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        output->append("Ошибка открытия файла.");
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < arr.size(); ++i) {
        out << arr[i];
        if (i != arr.size()-1) out << "$";
    }
    file.close();
    output->append("Массив записан в ФамилияЛаб20_0.txt");
}

void MainWindow::case4_readFloatArray()
{
    QFile file("ФамилияЛаб20_0.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        output->append("Файл ФамилияЛаб20_0.txt не найден.");
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QStringList tokens = content.split("$", Qt::SkipEmptyParts);
    QString result;
    for (const QString &t : tokens) {
        bool ok;
        float val = t.toFloat(&ok);
        if (ok) result += QString::number(val) + " ";
    }
    output->append("Массив из файла (разделены пробелом):\n" + result.trimmed());
}

void MainWindow::case5_appendTextStruct()
{
    State s = inputState();
    QFile file("ФамилияЛаб20_1.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        output->append("Ошибка открытия файла.");
        return;
    }
    QTextStream out(&file);
    out << s.population << "\n";
    out << s.area << "\n";
    out << s.continentCode << "\n";
    out << (s.isUNMember ? "1" : "0") << "\n";
    out << s.name << "\n";
    for (int i = 0; i < 3; ++i)
        out << s.gdpGrowth[i] << (i < 2 ? " " : "");
    out << "\n";
    file.close();
    output->append("Структура добавлена в текстовый файл.");
}

void MainWindow::case6_readTextStructs()
{
    QFile file("ФамилияЛаб20_1.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        output->append("Файл ФамилияЛаб20_1.txt не найден.");
        return;
    }
    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
    file.close();

    QString result = "Структуры из текстового файла:\n";
    int i = 0;
    while (i + 5 < lines.size()) {
        State s;
        s.population = lines[i++].toInt();
        s.area = lines[i++].toDouble();
        s.continentCode = lines[i++].at(0).toLatin1();
        s.isUNMember = (lines[i++].toInt() != 0);
        QByteArray nameBytes = lines[i++].toUtf8();
        std::strncpy(s.name, nameBytes.constData(), sizeof(s.name)-1);
        s.name[sizeof(s.name)-1] = '\0';
        QStringList gdpParts = lines[i++].split(' ', Qt::SkipEmptyParts);
        for (int j = 0; j < 3 && j < gdpParts.size(); ++j)
            s.gdpGrowth[j] = gdpParts[j].toInt();
        result += stateToString(s) + "\n";
    }
    output->append(result);
}

void MainWindow::case7_appendBinaryStruct()
{
    State s = inputState();
    QFile file("ФамилияЛаб20_2.bin");
    if (!file.open(QIODevice::Append)) {
        output->append("Ошибка открытия бинарного файла.");
        return;
    }
    file.write(reinterpret_cast<const char*>(&s), sizeof(State));
    file.close();
    output->append("Структура добавлена в бинарный файл.");
}

void MainWindow::case8_readBinaryStructs()
{
    QFile file("ФамилияЛаб20_2.bin");
    if (!file.open(QIODevice::ReadOnly)) {
        output->append("Файл ФамилияЛаб20_2.bin не найден.");
        return;
    }
    QString result = "Структуры из бинарного файла (разделитель ' ; '):\n";
    State s;
    while (file.read(reinterpret_cast<char*>(&s), sizeof(State)) == sizeof(State)) {
        QString gdpStr;
        for (int j = 0; j < 3; ++j)
            gdpStr += QString::number(s.gdpGrowth[j]) + (j < 2 ? ", " : "");
        result += QString("%1 ; %2 ; %3 ; %4 ; %5 ; [%6]\n")
                      .arg(s.population)
                      .arg(s.area, 0, 'f', 2)
                      .arg(s.continentCode)
                      .arg(s.isUNMember ? "true" : "false")
                      .arg(s.name)
                      .arg(gdpStr);
    }
    file.close();
    output->append(result);
}

void MainWindow::case9_appendLine()
{
    QString sentence = QInputDialog::getText(this, "Предложение", "Введите предложение:");
    if (sentence.isEmpty()) return;

    QFile file("ФамилияЛаб20_3.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        output->append("Ошибка открытия файла.");
        return;
    }
    QTextStream out(&file);
    out << sentence << "\n";
    file.close();
    output->append("Предложение добавлено в ФамилияЛаб20_3.txt");
}

void MainWindow::case10_readLines()
{
    QFile file("ФамилияЛаб20_3.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        output->append("Файл ФамилияЛаб20_3.txt не найден.");
        return;
    }
    QTextStream in(&file);
    QString result = "Строки из файла:\n";
    while (!in.atEnd()) {
        result += in.readLine() + "\n";
    }
    file.close();
    output->append(result);
}