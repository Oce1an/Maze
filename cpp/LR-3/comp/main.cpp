#include <QApplication>
#include <QTabWidget>
#include "task1widget.h"
#include "task2widget.h"
#include "task3widget.h"
#include "task4widget.h"
#include "task5widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTabWidget tabs;
    tabs.setWindowTitle("Лабораторная работа №3 – Рекурсия");
    tabs.resize(900, 600);

    tabs.addTab(new Task1Widget, "Задание 1");
    tabs.addTab(new Task2Widget, "Задание 2 (Аккерман)");
    tabs.addTab(new Task3Widget, "Задание 3 (Переворот)");
    tabs.addTab(new Task4Widget, "Задание 4 (Ханой)");
    tabs.addTab(new Task5Widget, "Задание 5 (Папки)");

    tabs.show();
    return a.exec();
}