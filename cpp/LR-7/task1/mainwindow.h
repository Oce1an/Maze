#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include "deque.h"
#include "ring.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createRandomDeque();
    void pushBack();
    void popFront();
    void showFront();
    void showBack();
    void showSize();
    void clearDeque();
    void splitToRings();

private:
    void updateDequeDisplay();
    void updateRingDisplays();
    void displayRing(QListWidget *list, Ring &ring);

    Deque m_deque;
    Ring m_ring1;
    Ring m_ring2;

    QListWidget *dequeList;
    QListWidget *ring1List;
    QListWidget *ring2List;

    QSpinBox *sizeSpin;
    QSpinBox *valueSpin;
};

#endif