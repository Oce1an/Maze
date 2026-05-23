#include <QApplication>
#include <QTabWidget>
#include "task1widget.h"
#include "task2widget.h"
#include "task3widget.h"
#include "vectordemowidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTabWidget tabs;
    tabs.setWindowTitle("Laboratory work 4 – All tasks");
    tabs.resize(1000, 700);

    tabs.addTab(new Task1Widget, "Task 1 (Heap/Quick/Merge)");
    tabs.addTab(new Task2Widget, "Task 2 (Interpolation + binpow)");
    tabs.addTab(new Task3Widget, "Task 3 (Medians)");
    tabs.addTab(new VectorDemoWidget, "Task 4/5 (Vector & Pair)");

    tabs.show();
    return app.exec();
}