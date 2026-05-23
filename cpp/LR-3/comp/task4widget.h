#ifndef TASK4WIDGET_H
#define TASK4WIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QStringList>

class Task4Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task4Widget(QWidget *parent = nullptr);

private slots:
    void solveHanoi();

private:
    QSpinBox *nSpinBox;
    QTextEdit *outputText;

    void hanoiOptimized(int n, char from, char to, char mid, QStringList &stepsList);
    void hanoiStringBuilder(int n, char from, char to, char mid, QStringList &stepsList);
};

#endif