#ifndef VECTORDEMOWIDGET_H
#define VECTORDEMOWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "vector.h"
#include "pair.h"

using IntVector = Vector<int>;
using PairVector = Vector<Pair<int, double>>;
using MainVector = Vector<Pair<IntVector, PairVector>>;

class VectorDemoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VectorDemoWidget(QWidget* parent = nullptr);

private slots:
    void onPushBackInt();
    void onPushBackPair();
    void onPopBackInt();
    void onErase();
    void onInsert();
    void onEmplace();
    void onResize();
    void onSwap();

private:
    void refreshTables();

    MainVector mainData;
    QTableWidget* tableInt;
    QTableWidget* tablePair;
};

#endif