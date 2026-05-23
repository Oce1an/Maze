// inputdialog.cpp
#include "inputdialog.h"
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>

InputDialog::InputDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Введите данные о государстве");
    QFormLayout *form = new QFormLayout(this);

    popSpin = new QSpinBox(this);
    popSpin->setRange(0, 10000);
    popSpin->setValue(100);
    form->addRow("Население (млн):", popSpin);

    areaSpin = new QDoubleSpinBox(this);
    areaSpin->setRange(0.0, 100000.0);
    areaSpin->setDecimals(2);
    areaSpin->setValue(500.0);
    form->addRow("Площадь (тыс. км²):", areaSpin);

    continentEdit = new QLineEdit(this);
    continentEdit->setMaxLength(1);
    continentEdit->setText("E");
    form->addRow("Код континента (буква):", continentEdit);

    unCheck = new QCheckBox("Член ООН", this);
    unCheck->setChecked(true);
    form->addRow(unCheck);

    nameEdit = new QLineEdit(this);
    nameEdit->setMaxLength(49);
    nameEdit->setText("Россия");
    form->addRow("Название:", nameEdit);

    for (int i = 0; i < 3; ++i) {
        gdpSpin[i] = new QSpinBox(this);
        gdpSpin[i]->setRange(-100, 100);
        gdpSpin[i]->setValue(i);
        form->addRow(QString("Рост ВВП год %1 (%):").arg(i+1), gdpSpin[i]);
    }

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this](){
        if (continentEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите код континента");
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    form->addRow(buttons);
}

int InputDialog::population() const { return popSpin->value(); }
double InputDialog::area() const { return areaSpin->value(); }
char InputDialog::continentCode() const { return continentEdit->text().at(0).toLatin1(); }
bool InputDialog::isUNMember() const { return unCheck->isChecked(); }
QString InputDialog::name() const { return nameEdit->text(); }
int InputDialog::gdpGrowth(int index) const {
    if (index >= 0 && index < 3) return gdpSpin[index]->value();
    return 0;
}