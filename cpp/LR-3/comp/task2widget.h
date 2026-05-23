#ifndef TASK2WIDGET_H
#define TASK2WIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

class Task2Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task2Widget(QWidget *parent = nullptr);

private slots:
    void calculateAckermann();

private:
    QSpinBox *mSpinBox;
    QSpinBox *nSpinBox;
    QLabel *resultLabel;

    long long ackermann(long long m, long long n);
};

#endif