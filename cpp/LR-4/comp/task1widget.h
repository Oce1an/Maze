#ifndef TASK1WIDGET_H
#define TASK1WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class Task1Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Task1Widget(QWidget* parent = nullptr);

private slots:
    void onSortAndSearch();

private:
    QLineEdit* sizeEdit;
    QLineEdit* elementsEdit;
    QLineEdit* searchEdit;
    QTextEdit* output;
    QPushButton* runButton;
};

#endif