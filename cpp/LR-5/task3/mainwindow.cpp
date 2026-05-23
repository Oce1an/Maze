#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Задание 3 — Класс BitSet");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
void MainWindow::appendLog(const QString &msg)
{
    ui->logBrowser->append(msg);
}

void MainWindow::refreshDisplay()
{
    if (!m_bsA) { ui->displayA->setText("(не создан)"); return; }
    ui->displayA->setText(QString::fromStdString(m_bsA->to_string()));

    if (m_bsB)
        ui->displayB->setText(QString::fromStdString(m_bsB->to_string()));
    else
        ui->displayB->setText("(не создан)");
}

size_t MainWindow::getPosA()
{
    bool ok;
    size_t pos = static_cast<size_t>(ui->posSpinA->value());
    (void)ok;
    return pos;
}

// ---------------------------------------------------------------------------
// Slots
// ---------------------------------------------------------------------------
void MainWindow::on_createButton_clicked()
{
    bool okA, okB;
    int nA = ui->sizeEditA->text().toInt(&okA);
    int nB = ui->sizeEditB->text().toInt(&okB);

    if (!okA || nA <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный размер A (> 0).");
        return;
    }

    QString initA = ui->initEditA->text().trimmed();
    if (!initA.isEmpty()) {
        bool numOk;
        unsigned long long val = initA.toULongLong(&numOk);
        m_bsA = std::make_unique<MyBitSet>(static_cast<size_t>(nA), val);
    } else {
        m_bsA = std::make_unique<MyBitSet>(static_cast<size_t>(nA));
    }
    appendLog(QString("Создан BitSet A[%1] = %2").arg(nA)
              .arg(QString::fromStdString(m_bsA->to_string())));

    if (okB && nB > 0) {
        QString initB = ui->initEditB->text().trimmed();
        if (!initB.isEmpty()) {
            bool numOk;
            unsigned long long val = initB.toULongLong(&numOk);
            m_bsB = std::make_unique<MyBitSet>(static_cast<size_t>(nB), val);
        } else {
            m_bsB = std::make_unique<MyBitSet>(static_cast<size_t>(nB));
        }
        appendLog(QString("Создан BitSet B[%1] = %2").arg(nB)
                  .arg(QString::fromStdString(m_bsB->to_string())));
    }

    ui->posSpinA->setMaximum(nA - 1);
    refreshDisplay();
}

void MainWindow::on_setButton_clicked()
{
    if (!m_bsA) return;
    try {
        size_t pos = getPosA();
        m_bsA->set(pos);
        appendLog(QString("set(%1) → %2").arg(pos)
                  .arg(QString::fromStdString(m_bsA->to_string())));
        refreshDisplay();
    } catch (std::out_of_range& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::on_resetButton_clicked()
{
    if (!m_bsA) return;
    try {
        size_t pos = getPosA();
        m_bsA->reset(pos);
        appendLog(QString("reset(%1) → %2").arg(pos)
                  .arg(QString::fromStdString(m_bsA->to_string())));
        refreshDisplay();
    } catch (std::out_of_range& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::on_flipButton_clicked()
{
    if (!m_bsA) return;
    try {
        size_t pos = getPosA();
        m_bsA->flip(pos);
        appendLog(QString("flip(%1) → %2").arg(pos)
                  .arg(QString::fromStdString(m_bsA->to_string())));
        refreshDisplay();
    } catch (std::out_of_range& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::on_setAllButton_clicked()
{
    if (!m_bsA) return;
    m_bsA->set();
    appendLog("set() все биты → " + QString::fromStdString(m_bsA->to_string()));
    refreshDisplay();
}

void MainWindow::on_resetAllButton_clicked()
{
    if (!m_bsA) return;
    m_bsA->reset();
    appendLog("reset() все биты → " + QString::fromStdString(m_bsA->to_string()));
    refreshDisplay();
}

void MainWindow::on_flipAllButton_clicked()
{
    if (!m_bsA) return;
    m_bsA->flip();
    appendLog("flip() все биты → " + QString::fromStdString(m_bsA->to_string()));
    refreshDisplay();
}

void MainWindow::on_andButton_clicked()
{
    if (!m_bsA || !m_bsB) {
        QMessageBox::warning(this, "Ошибка", "Нужны оба BitSet A и B.");
        return;
    }
    MyBitSet res = *m_bsA & *m_bsB;
    appendLog("A & B = " + QString::fromStdString(res.to_string()));
}

void MainWindow::on_orButton_clicked()
{
    if (!m_bsA || !m_bsB) {
        QMessageBox::warning(this, "Ошибка", "Нужны оба BitSet A и B.");
        return;
    }
    MyBitSet res = *m_bsA | *m_bsB;
    appendLog("A | B = " + QString::fromStdString(res.to_string()));
}

void MainWindow::on_notButton_clicked()
{
    if (!m_bsA) return;
    MyBitSet res = ~(*m_bsA);
    appendLog("~A = " + QString::fromStdString(res.to_string()));
}

void MainWindow::on_statsButton_clicked()
{
    if (!m_bsA) return;
    appendLog("--- Статистика BitSet A ---");
    appendLog(QString("size()  = %1").arg(m_bsA->size()));
    appendLog(QString("count() = %1").arg(m_bsA->count()));
    appendLog(QString("all()   = %1").arg(m_bsA->all()  ? "true" : "false"));
    appendLog(QString("any()   = %1").arg(m_bsA->any()  ? "true" : "false"));
    appendLog(QString("none()  = %1").arg(m_bsA->none() ? "true" : "false"));
    appendLog(QString("to_ulong()  = %1").arg(m_bsA->to_ulong()));
    appendLog(QString("to_ullong() = %1").arg(m_bsA->to_ullong()));
    appendLog(QString("to_string() = %1").arg(QString::fromStdString(m_bsA->to_string())));
}
