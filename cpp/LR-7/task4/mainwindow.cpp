#include "mainwindow.h"
#include "hashtable.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QElapsedTimer>
#include <QVector>
#include <QMap>
#include <QtMath>
#include <cstdlib>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Исследование хеш-таблиц с открытой адресацией");
    resize(800, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    chart = new QChart();
    chart->setTitle("Среднее время поиска (универсальное хеширование)");
    chart->legend()->setVisible(true);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnResearch = new QPushButton("Запустить исследование");
    btnShowTable = new QPushButton("Показать таблицу (m=16, загрузка 0.9)");
    btnLayout->addWidget(btnResearch);
    btnLayout->addWidget(btnShowTable);
    layout->addLayout(btnLayout);

    textOutput = new QTextEdit();
    textOutput->setReadOnly(true);
    layout->addWidget(textOutput, 1);

    connect(btnResearch, &QPushButton::clicked, this, &MainWindow::runResearch);
    connect(btnShowTable, &QPushButton::clicked, this, &MainWindow::showTable);
}

MainWindow::~MainWindow() {}

void MainWindow::runResearch() {
    chart->removeAllSeries();
    const auto axes = chart->axes();
    for (QAbstractAxis *axis : axes) {
        chart->removeAxis(axis);
        delete axis;
    }

    const QVector<int> sizes = {16, 64, 128, 2048};
    const QVector<double> loads = {0.5, 0.75, 0.9};
    const int queryCount = 10000;

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Среднее время поиска (мкс)");
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setTitleText("Размер таблицы");
    axisX->setRange(0, sizes.size() - 1);
    for (int i = 0; i < sizes.size(); ++i)
        axisX->append(QString::number(sizes[i]), i);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    QMap<double, QLineSeries*> seriesMap;
    for (double lf : loads) {
        QLineSeries *series = new QLineSeries();
        series->setName(QString("Загрузка %1%").arg(lf * 100));
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        seriesMap[lf] = series;
    }

    textOutput->clear();
    textOutput->append("Идёт исследование...\n");

    for (int sizeIdx = 0; sizeIdx < sizes.size(); ++sizeIdx) {
        int m = sizes[sizeIdx];
        for (double lf : loads) {
            OpenAddrHashTable ht(m);
            int n = qFloor(m * lf);
            int inserted = 0;
            QVector<int> presentKeys; 

            while (inserted < n) {
                int key = rand() % 100000 + 1;
                if (ht.insert(key)) {
                    presentKeys.append(key);
                    inserted++;
                }
            }

            QVector<int> testKeys;
            for (int i = 0; i < queryCount / 2; ++i) {
                int idx = rand() % presentKeys.size();
                testKeys.append(presentKeys[idx]);
            }
            for (int i = 0; i < queryCount / 2; ++i) {
                int key;
                do {
                    key = rand() % 100000 + 1;
                } while (ht.search(key)); 
                testKeys.append(key);
            }

            QElapsedTimer timer;
            timer.start();
            for (int key : testKeys)
                ht.search(key);
            qint64 elapsed = timer.nsecsElapsed();
            double avgMicro = (elapsed / 1000.0) / queryCount;

            textOutput->append(QString("m=%1, загрузка %2: среднее время поиска %3 мкс")
                                   .arg(m)
                                   .arg(lf)
                                   .arg(avgMicro, 0, 'f', 3));

            seriesMap[lf]->append(sizeIdx, avgMicro);
        }
    }

    double maxTime = 0;
    for (QLineSeries *s : seriesMap) {
        for (const QPointF &p : s->points())
            if (p.y() > maxTime) maxTime = p.y();
    }
    axisY->setRange(0, maxTime * 1.1);
    textOutput->append("\nИсследование завершено.");
}

void MainWindow::showTable() {
    OpenAddrHashTable ht(16);
    int n = qFloor(16 * 0.9);
    int inserted = 0;
    while (inserted < n) {
        int key = rand() % 1000 + 1;
        if (ht.insert(key)) inserted++;
    }
    QString out;
    ht.print(out);
    textOutput->clear();
    textOutput->append("Пример таблицы (m=16, загрузка 0.9):\n" + out);
}