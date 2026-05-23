#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_lang(Language::German)
    , m_errorTimer(nullptr)
    , m_started(false)
{
    qDebug() << "Constructor start";
    ui->setupUi(this);
    qDebug() << "setupUi done";
    
    setWindowTitle("Клавиатурный тренажёр");

    if (!ui->keyboardWidget) {
        qDebug() << "ERROR: keyboardWidget is null after setupUi!";
        return;
    }

    ui->langCombo->addItem(Trainer::languageName(Language::German));
    ui->langCombo->addItem(Trainer::languageName(Language::French));
    ui->langCombo->addItem(Trainer::languageName(Language::Arabic));
    ui->langCombo->addItem(Trainer::languageName(Language::Chinese));
    ui->langCombo->addItem(Trainer::languageName(Language::Belarusian));
    ui->langCombo->addItem(Trainer::languageName(Language::Hebrew));
    qDebug() << "Combo items added";

    connect(&m_timer, &QTimer::timeout, this, &MainWindow::onTimerTick);
    m_timer.setInterval(100);
    qDebug() << "Timer setup done";

    m_errorTimer = new QTimer(this);
    m_errorTimer->setSingleShot(true);
    m_errorTimer->setInterval(300);
    connect(m_errorTimer, &QTimer::timeout, this, [this]() {
        m_trainer.clearLastError();
        refreshTextDisplay();
    });
    qDebug() << "Error timer setup done";

    setFocusPolicy(Qt::StrongFocus);
    
    qDebug() << "Calling applyLanguage";
    applyLanguage(m_lang);
    qDebug() << "Constructor end";
}

MainWindow::~MainWindow()
{
    qDebug() << "Destructor";
    delete ui;
}

void MainWindow::applyLanguage(Language lang)
{
    qDebug() << "applyLanguage start" << (int)lang;
    m_lang = lang;
    
    m_keyMap = Trainer::keyMapping(lang);
    qDebug() << "Key mapping loaded, size:" << m_keyMap.size();
    
    QStringList samples = Trainer::sampleText(lang);
    QString text = samples.isEmpty() ? "Test" : samples.join(" ");
    qDebug() << "Sample text:" << text;

    bool rtl = (lang == Language::Arabic || lang == Language::Hebrew);
    ui->textEdit->setLayoutDirection(rtl ? Qt::RightToLeft : Qt::LeftToRight);
    qDebug() << "Text direction set";

    m_trainer.setText(text);
    qDebug() << "Trainer text set";
    
    resetState();
    qDebug() << "State reset";
    
    refreshTextDisplay();
    qDebug() << "Text display refreshed";
    
    buildKeyboard();
    qDebug() << "Keyboard built";
    
    setFocus();
    qDebug() << "applyLanguage end";
}

void MainWindow::resetState()
{
    m_started = false;
    m_timer.stop();
    if (m_errorTimer) m_errorTimer->stop();
    ui->timerLabel->setText("00:00.0");
    ui->wpmLabel->setText("0");
    ui->accuracyLabel->setText("100.0%");
}

void MainWindow::refreshTextDisplay()
{
    qDebug() << "refreshTextDisplay start";
    
    if (!ui->textEdit) {
        qDebug() << "textEdit is null!";
        return;
    }
    
    const QString &text = m_trainer.text();
    int pos = m_trainer.position();
    const auto &results = m_trainer.results();
    bool hasError = m_trainer.hasLastError();

    qDebug() << "Text length:" << text.length() << "Position:" << pos;

    ui->textEdit->clear();
    QTextCursor cursor(ui->textEdit->document());

    for (int i = 0; i < text.length(); ++i) {
        QTextCharFormat fmt;

        if (i < pos) {
            bool correct = (i < results.size()) ? results[i].correct : true;
            fmt.setForeground(correct ? QColor(0, 140, 0) : QColor(200, 0, 0));
            fmt.setBackground(Qt::transparent);
        } else if (i == pos) {
            if (hasError) {
                fmt.setBackground(QColor(255, 180, 180));
                fmt.setForeground(QColor(200, 0, 0));
            } else {
                fmt.setBackground(QColor(180, 200, 255));
                fmt.setForeground(Qt::black);
            }
        } else {
            fmt.setForeground(QColor(80, 80, 80));
            fmt.setBackground(Qt::transparent);
        }

        cursor.insertText(QString(text[i]), fmt);
    }
    qDebug() << "refreshTextDisplay end";
}

void MainWindow::buildKeyboard()
{
    qDebug() << "buildKeyboard start";
    
    QWidget *kbWidget = ui->keyboardWidget;
    if (!kbWidget) {
        qDebug() << "keyboardWidget is null!";
        return;
    }
    
    qDebug() << "keyboardWidget OK";
    
    if (kbWidget->layout()) {
        QLayout *oldLayout = kbWidget->layout();
        
        QList<QWidget*> childWidgets = kbWidget->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        for (QWidget *child : childWidgets) {
            child->setParent(nullptr);
            delete child;
        }
        
        while (oldLayout->count() > 0) {
            QLayoutItem *item = oldLayout->takeAt(0);
            if (item) {
                delete item;
            }
        }
        
        delete oldLayout;
        qDebug() << "Old layout deleted";
    }

    qDebug() << "Creating new layout";
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(3);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    QList<QStringList> rows = Trainer::keyboardLayout(m_lang);
    qDebug() << "Got" << rows.size() << "keyboard rows";
    
    if (rows.isEmpty()) {
        qDebug() << "ERROR: No keyboard layout data!";
        kbWidget->setLayout(mainLayout);
        return;
    }
    
    QList<int> rowOffsets = {0, 15, 25, 10, 0};
    
    for (int rowIdx = 0; rowIdx < rows.size(); ++rowIdx) {
        const QStringList &row = rows[rowIdx];
        
        if (row.isEmpty()) {
            qDebug() << "Skipping empty row" << rowIdx;
            continue;
        }
        
        QHBoxLayout *rowLayout = new QHBoxLayout();
        rowLayout->setSpacing(2);
        
        if (rowIdx < rowOffsets.size()) {
            rowLayout->addSpacing(rowOffsets[rowIdx]);
        }
        
        for (int keyIdx = 0; keyIdx < row.size(); ++keyIdx) {
            const QString &key = row[keyIdx];
            
            if (key.isEmpty()) {
                rowLayout->addSpacing(8);
                continue;
            }
            
            QPushButton *btn = new QPushButton(key);
            
            int width = 34;
            if (key == "Space" || key.contains("Space")) {
                width = 200;
            } else if (key == "←" || key == "Backspace") {
                width = 55;
            } else if (key == "Tab") {
                width = 55;
            } else if (key == "Caps") {
                width = 60;
            } else if (key == "Shift") {
                width = 70;
            } else if (key == "Ctrl" || key == "Strg") {
                width = 55;
            } else if (key == "Alt" || key == "AltGr") {
                width = 45;
            } else if (key == "↵" || key == "Enter") {
                width = 65;
            } else if (key.length() > 2) {
                width = 42;
            }
            
            btn->setFixedSize(width, 32);
            btn->setFocusPolicy(Qt::NoFocus);
            
            QString style;
            if (key == "Space" || key.contains("Space") || 
                key == "←" || key == "Tab" || key == "Caps" || 
                key == "Shift" || key == "Ctrl" || key == "Strg" || 
                key == "Alt" || key == "AltGr" || key == "↵") {
                style = "QPushButton { font-size:8px; border-radius:3px; background:#4a4a4a; color:#ddd; border:1px solid #666; padding:2px; }";
            } else {
                style = "QPushButton { font-size:11px; border-radius:3px; background:#5a5a5a; color:white; border:1px solid #777; padding:2px; }";
            }
            
            btn->setStyleSheet(style);
            rowLayout->addWidget(btn);
        }
        
        rowLayout->addStretch();
        mainLayout->addLayout(rowLayout);
    }
    
    mainLayout->addStretch();
    kbWidget->setLayout(mainLayout);
    kbWidget->setMinimumHeight(200);
    
    qDebug() << "buildKeyboard end";
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "keyPressEvent called, key:" << event->key() << "text:" << event->text();
    
    if (!event) {
        QMainWindow::keyPressEvent(event);
        return;
    }
    
    if (event->key() == Qt::Key_Shift || 
        event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Alt ||
        event->key() == Qt::Key_Meta) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    int key = event->key();
    QString mappedText;
    
    qDebug() << "Looking for key:" << key << "in keyMap, map size:" << m_keyMap.size();
    
    if (m_keyMap.contains(key)) {
        mappedText = m_keyMap[key];
        qDebug() << "Found in keyMap:" << mappedText;
    } else {
        mappedText = event->text();
        qDebug() << "Not found in keyMap, using event text:" << mappedText;
    }
    
    if (mappedText.isEmpty()) {
        qDebug() << "Empty mapped text, ignoring";
        QMainWindow::keyPressEvent(event);
        return;
    }

    QChar ch = mappedText.at(0);
    
    if (ch != ' ' && !ch.isPrint()) {
        qDebug() << "Non-printable character, ignoring";
        QMainWindow::keyPressEvent(event);
        return;
    }
    
    qDebug() << "Got character:" << ch;
    
    if (!m_started && !m_trainer.finished()) {
        m_started = true;
        m_elapsed.restart();
        m_timer.start();
        qDebug() << "Timer started";
    }

    bool correct = m_trainer.typeChar(ch);
    qDebug() << "Char typed, correct:" << correct;
    
    refreshTextDisplay();
    updateStats();

    if (!correct && !m_trainer.finished()) {
        if (m_errorTimer) {
            m_errorTimer->start();
        }
    }

    if (m_trainer.finished()) {
        m_timer.stop();
        updateStats();
        qDebug() << "Training finished!";
    }
}

void MainWindow::onTimerTick()
{
    if (!m_started) return;
    
    qint64 ms = m_elapsed.elapsed();
    int totalSec = ms / 1000;
    int min = totalSec / 60;
    int sec = totalSec % 60;
    int tenth = (ms % 1000) / 100;
    ui->timerLabel->setText(QString("%1:%2.%3")
        .arg(min,  2, 10, QChar('0'))
        .arg(sec,  2, 10, QChar('0'))
        .arg(tenth));
    updateStats();
}

void MainWindow::updateStats()
{
    qint64 ms = m_started ? m_elapsed.elapsed() : 0;
    double wpm = m_trainer.wordsPerMinute(ms);
    ui->wpmLabel->setText(QString::number(wpm, 'f', 1));
    double acc = m_trainer.accuracy();
    ui->accuracyLabel->setText(QString::number(acc, 'f', 1) + "%");
}

void MainWindow::on_langCombo_currentIndexChanged(int index)
{
    qDebug() << "Language changed to index:" << index;
    if (index >= 0 && index <= 5) {
        Language newLang = static_cast<Language>(index);
        if (newLang != m_lang) {
            applyLanguage(newLang);
        }
    }
}

void MainWindow::on_openFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Открыть файл", "",
                                                "Text files (*.txt);;All files (*)");
    if (path.isEmpty()) return;
    
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    
    QTextStream in(&f);
    in.setEncoding(QStringConverter::Utf8);
    QString content = in.readAll().simplified();
    
    if (content.isEmpty()) {
        content = "Empty file";
    }
    
    m_trainer.setText(content);
    resetState();
    refreshTextDisplay();
    setFocus();
}

void MainWindow::on_restartButton_clicked()
{
    m_trainer.reset();
    resetState();
    refreshTextDisplay();
    setFocus();
}