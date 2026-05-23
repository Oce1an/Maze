#include "resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Результаты");
    setMinimumSize(700, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    labelTitle = new QLabel("Результаты:", this);
    labelTitle->setFont(QFont("Arial", 12, QFont::Bold));
    layout->addWidget(labelTitle);
    
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Courier New", 10));
    textEdit->setLineWrapMode(QTextEdit::NoWrap);
    layout->addWidget(textEdit);
    
    QPushButton *closeBtn = new QPushButton("Закрыть", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}

void ResultDialog::setResult(const QString& resultText, const QString& title) {
    labelTitle->setText(title);
    textEdit->setPlainText(resultText);
    textEdit->moveCursor(QTextCursor::Start);
}