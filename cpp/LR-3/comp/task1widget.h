#ifndef TASK1WIDGET_H
#define TASK1WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class Task1Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task1Widget(QWidget *parent = nullptr);

private slots:
    void convert();

private:
    QLineEdit *inputEdit;
    QLabel *resultLabel;

    QString intToBin(long long n);
    void fracToBin(double frac, int maxDigits, QString &out);
    void fracToBinHelper(double frac, int maxDigits, QString &out);
};

#endif