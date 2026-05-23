#ifndef TASK3WIDGET_H
#define TASK3WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class Task3Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task3Widget(QWidget *parent = nullptr);

private slots:
    void reverseNumber();

private:
    QLineEdit *inputEdit;
    QLabel *resultLabel;

    int reverseInt(int n, int acc);
    bool hasNoZero(int n);
};

#endif