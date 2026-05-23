#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Задание 2 — Динамическая библиотека String");

    const QStringList funcs = {
        "strlen",  "strcpy",  "strncpy", "strcat",  "strncat",
        "strcmp",  "strncmp", "strcoll", "strxfrm", "strtok",
        "memcpy",  "memmove", "memcmp",  "memset",  "strerror"
    };
    for (const QString &f : funcs)
        ui->funcCombo->addItem(f);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendResult(const QString &line)
{
    ui->resultBrowser->append(line);
}

void MainWindow::on_runButton_clicked()
{
    ui->resultBrowser->clear();

    QString func = ui->funcCombo->currentText();
    QByteArray ba1 = ui->s1Edit->text().toUtf8();
    QByteArray ba2 = ui->s2Edit->text().toUtf8();
    QString    nStr = ui->nEdit->text().trimmed();

    bool   nOk = false;
    size_t n   = nStr.toULongLong(&nOk);

    appendResult(QString("Функция: <b>%1</b>").arg(func));
    appendResult(QString("s1 = \"%1\"").arg(ui->s1Edit->text()));
    appendResult(QString("s2 = \"%1\"").arg(ui->s2Edit->text()));
    appendResult("---");

    char buf1[512] = {0};
    char buf2[512] = {0};
    std::strncpy(buf1, ba1.constData(), 511);
    std::strncpy(buf2, ba2.constData(), 511);

    if (func == "strlen") {
        size_t r = MyString::s_strlen(buf1);
        appendResult(QString("strlen(s1) = %1").arg(r));

    } else if (func == "strcpy") {
        char dst[512] = {0};
        MyString::s_strcpy(dst, buf1);
        appendResult(QString("strcpy(dst, s1) → dst = \"%1\"").arg(dst));

    } else if (func == "strncpy") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n."); return; }
        char dst[512] = {0};
        MyString::s_strncpy(dst, buf1, n);
        appendResult(QString("strncpy(dst, s1, %1) → dst = \"%2\"").arg(n).arg(dst));

    } else if (func == "strcat") {
        char dst[512] = {0};
        MyString::s_strcpy(dst, buf1);
        MyString::s_strcat(dst, buf2);
        appendResult(QString("strcat(s1, s2) → \"%1\"").arg(dst));

    } else if (func == "strncat") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n."); return; }
        char dst[512] = {0};
        MyString::s_strcpy(dst, buf1);
        MyString::s_strncat(dst, buf2, n);
        appendResult(QString("strncat(s1, s2, %1) → \"%2\"").arg(n).arg(dst));

    } else if (func == "strcmp") {
        int r = MyString::s_strcmp(buf1, buf2);
        appendResult(QString("strcmp(s1, s2) = %1  (%2)").arg(r).arg(
            r < 0 ? "s1 < s2" : r > 0 ? "s1 > s2" : "s1 == s2"));

    } else if (func == "strncmp") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n."); return; }
        int r = MyString::s_strncmp(buf1, buf2, n);
        appendResult(QString("strncmp(s1, s2, %1) = %2").arg(n).arg(r));

    } else if (func == "strcoll") {
        int r = MyString::s_strcoll(buf1, buf2);
        appendResult(QString("strcoll(s1, s2) = %1").arg(r));

    } else if (func == "strxfrm") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n."); return; }
        char dst[512] = {0};
        size_t r = MyString::s_strxfrm(dst, buf1, n);
        appendResult(QString("strxfrm(dst, s1, %1) → r=%2, dst=\"%3\"").arg(n).arg(r).arg(dst));

    } else if (func == "strtok") {
        char tmp[512];
        MyString::s_strcpy(tmp, buf1);
        QString out;
        char* tok = MyString::s_strtok(tmp, buf2);
        while (tok) {
            if (!out.isEmpty()) out += ", ";
            out += QString("\"%1\"").arg(tok);
            tok = MyString::s_strtok(nullptr, buf2);
        }
        appendResult(QString("strtok(s1, s2) → токены: [%1]").arg(out));

    } else if (func == "memcpy") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n."); return; }
        char dst[512] = {0};
        MyString::s_memcpy(dst, buf1, n);
        appendResult(QString("memcpy(dst, s1, %1) → dst = \"%2\"").arg(n).arg(dst));

    } else if (func == "memmove") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n."); return; }
        char src[512] = {0};
        MyString::s_strcpy(src, buf1);
        if (n + 2 < 512 && n <= std::strlen(src)) {
            MyString::s_memmove(src + 2, src, n);
            appendResult(QString("memmove(src+2, src, %1) → src = \"%2\"").arg(n).arg(src));
        } else {
            char dst[512] = {0};
            MyString::s_memmove(dst, src, n);
            appendResult(QString("memmove(dst, s1, %1) → dst = \"%2\"").arg(n).arg(dst));
        }

    } else if (func == "memcmp") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n."); return; }
        int r = MyString::s_memcmp(buf1, buf2, n);
        appendResult(QString("memcmp(s1, s2, %1) = %2").arg(n).arg(r));

    } else if (func == "memset") {
        if (!nOk) { QMessageBox::warning(this, "Ошибка", "Введите n (байты)."); return; }
        char c  = buf2[0] ? buf2[0] : '0';
        char dst[512] = {0};
        MyString::s_strcpy(dst, buf1);
        MyString::s_memset(dst, static_cast<unsigned char>(c), n);
        appendResult(QString("memset(s1, '%1', %2) → \"%3\"").arg(c).arg(n).arg(dst));

    } else if (func == "strerror") {
        bool eOk = false;
        int  eno = nStr.toInt(&eOk);
        if (!eOk) { QMessageBox::warning(this, "Ошибка", "Введите errno в поле n."); return; }
        char* msg = MyString::s_strerror(eno);
        appendResult(QString("strerror(%1) = \"%2\"").arg(eno).arg(msg));
    }
}
