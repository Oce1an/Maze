#ifndef TASK3WIDGET_H
#define TASK3WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class Task3Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task3Widget(QWidget* parent = nullptr);

private slots:
    void onCompute();

private:
    QLineEdit* elementsEdit;
    QTextEdit* output;
    QPushButton* runButton;
};

#endif