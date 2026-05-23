#ifndef TASK2WIDGET_H
#define TASK2WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class Task2Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task2Widget(QWidget* parent = nullptr);

private slots:
    void onRun();

private:
    QLineEdit* sizeEdit;
    QLineEdit* elementsEdit;
    QLineEdit* searchEdit;
    QLineEdit* modEdit;
    QTextEdit* output;
    QPushButton* runButton;
};

#endif