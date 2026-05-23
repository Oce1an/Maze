#include "task3widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>
#include <algorithm>

static int medianOfThree(int a, int b, int c) {
    if (a > b) std::swap(a, b);
    if (a > c) std::swap(a, c);
    if (b > c) std::swap(b, c);
    return b;
}

static double average(const std::vector<int>& group) {
    if (group.empty()) return 0.0;
    double sum = 0.0;
    for (int x : group) sum += x;
    return sum / group.size();
}

Task3Widget::Task3Widget(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);

    auto* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(new QLabel("Array elements (space separated):"));
    elementsEdit = new QLineEdit;
    elementsEdit->setPlaceholderText("e.g. 1 5 1 4 5 6 2 1 3 4 4 4 5 7");
    inputLayout->addWidget(elementsEdit);
    runButton = new QPushButton("Compute b");
    inputLayout->addWidget(runButton);
    mainLayout->addLayout(inputLayout);

    output = new QTextEdit;
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    connect(runButton, &QPushButton::clicked, this, &Task3Widget::onCompute);
}

void Task3Widget::onCompute()
{
    output->clear();
    QStringList parts = elementsEdit->text().split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty()) {
        output->setText("Enter at least one integer.");
        return;
    }

    std::vector<int> a;
    bool ok;
    for (const QString& s : parts) {
        a.push_back(s.toInt(&ok));
        if (!ok) {
            output->setText(QString("Invalid integer: '%1'").arg(s));
            return;
        }
    }

    std::vector<double> b;
    for (size_t i = 0; i < a.size(); i += 3) {
        size_t remain = std::min<size_t>(3, a.size() - i);
        std::vector<int> group(a.begin() + i, a.begin() + i + remain);
        if (group.size() == 3)
            b.push_back(medianOfThree(group[0], group[1], group[2]));
        else
            b.push_back(average(group));
    }

    QString result;
    result += "a = { ";
    for (size_t i = 0; i < a.size(); ++i) {
        result += QString::number(a[i]);
        if (i != a.size() - 1) result += ", ";
    }
    result += " }\n\nb = { ";
    for (size_t i = 0; i < b.size(); ++i) {
        result += QString::number(b[i]);
        if (i != b.size() - 1) result += ", ";
    }
    result += " }";
    output->setText(result);
}