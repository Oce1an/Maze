#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>

class InputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr);

    int population() const;
    double area() const;
    char continentCode() const;
    bool isUNMember() const;
    QString name() const;
    int gdpGrowth(int index) const;

private:
    QSpinBox *popSpin;
    QDoubleSpinBox *areaSpin;
    QLineEdit *continentEdit;
    QCheckBox *unCheck;
    QLineEdit *nameEdit;
    QSpinBox *gdpSpin[3];
};

#endif