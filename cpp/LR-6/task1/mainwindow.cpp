#include "mainwindow.h"
#include "inputdialog.h"
#include <QMessageBox>
#include <cstring>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Государство");
    resize(600, 500);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);
    output = new QTextEdit(this);
    output->setReadOnly(true);
    layout->addWidget(output);

    QPushButton *btnDefault  = new QPushButton("1. Инициализация через { }", this);
    QPushButton *btnAssign   = new QPushButton("2. Присваивание полей отдельно", this);
    QPushButton *btnUser     = new QPushButton("3. Ввод пользователем", this);
    QPushButton *btnPointer  = new QPushButton("4. Указатель на структуру", this);
    QPushButton *btnRef      = new QPushButton("5. Ссылка на структуру", this);

    layout->addWidget(btnDefault);
    layout->addWidget(btnAssign);
    layout->addWidget(btnUser);
    layout->addWidget(btnPointer);
    layout->addWidget(btnRef);

    connect(btnDefault, &QPushButton::clicked, this, &MainWindow::showDefaultInit);
    connect(btnAssign,  &QPushButton::clicked, this, &MainWindow::showSeparateAssignment);
    connect(btnUser,    &QPushButton::clicked, this, &MainWindow::showUserInput);
    connect(btnPointer, &QPushButton::clicked, this, &MainWindow::showPointerInit);
    connect(btnRef,     &QPushButton::clicked, this, &MainWindow::showReferenceInit);

    // Автоматический показ первого пункта
    showDefaultInit();
}

MainWindow::~MainWindow() {}

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

void MainWindow::showDefaultInit()
{
    // 2
    State s = { 146, 17100.0, 'E', true, "Россия", {2, -3, 1} };
    output->append("=== 1. Инициализация через { } ===\n" + stateToString(s) + "\n");
}

void MainWindow::showSeparateAssignment()
{
    // 3
    State s;
    s.population = 83;
    s.area = 357.0;
    s.continentCode = 'E';
    s.isUNMember = true;

    std::strncpy(s.name, "Германия", sizeof(s.name) - 1);
    s.name[sizeof(s.name)-1] = '\0';
    s.gdpGrowth[0] = 1; s.gdpGrowth[1] = 2; s.gdpGrowth[2] = 0;

    output->append("=== 2. Присваивание полей отдельно ===\n" + stateToString(s) + "\n");
}

void MainWindow::showUserInput()
{
    InputDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        State s;
        s.population    = dlg.population();
        s.area          = dlg.area();
        s.continentCode = dlg.continentCode();
        s.isUNMember    = dlg.isUNMember();
        QByteArray nameUtf8 = dlg.name().toUtf8();
        std::strncpy(s.name, nameUtf8.constData(), sizeof(s.name)-1);
        s.name[sizeof(s.name)-1] = '\0';
        for (int i = 0; i < 3; ++i)
            s.gdpGrowth[i] = dlg.gdpGrowth(i);

        output->append("=== 3. Ввод пользователем ===\n" + stateToString(s) + "\n");
    }
}

void MainWindow::showPointerInit()
{
    // 5
    State s;
    State *ptr = &s;
    ptr->population = 38;
    ptr->area = 312.7;
    ptr->continentCode = 'E';
    ptr->isUNMember = true;
    std::strncpy(ptr->name, "Польша", sizeof(ptr->name)-1);
    ptr->name[sizeof(ptr->name)-1] = '\0';
    ptr->gdpGrowth[0] = 4; ptr->gdpGrowth[1] = 5; ptr->gdpGrowth[2] = 4;

    output->append("=== 4. Обращение через указатель ===\n" + stateToString(*ptr) + "\n");
}

void MainWindow::showReferenceInit()
{
    // 6
    State s;
    State &ref = s;
    ref.population = 10;
    ref.area = 93.0;
    ref.continentCode = 'E';
    ref.isUNMember = true;
    std::strncpy(ref.name, "Венгрия", sizeof(ref.name)-1);
    ref.name[sizeof(ref.name)-1] = '\0';
    ref.gdpGrowth[0] = 3; ref.gdpGrowth[1] = 4; ref.gdpGrowth[2] = 2;

    output->append("=== 5. Обращение через ссылку ===\n" + stateToString(ref) + "\n");
}