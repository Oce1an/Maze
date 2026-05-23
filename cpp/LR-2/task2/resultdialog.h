#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class ResultDialog : public QDialog {
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr);
    void setResult(const QString& resultText, const QString& title);

private:
    QTextEdit *textEdit;
    QLabel *labelTitle;
};

#endif