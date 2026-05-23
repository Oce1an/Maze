#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QMap>
#include "trainer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_langCombo_currentIndexChanged(int index);
    void on_openFileButton_clicked();
    void on_restartButton_clicked();
    void onTimerTick();

private:
    Ui::MainWindow *ui;
    Trainer         m_trainer;
    Language        m_lang;
    QTimer          m_timer;
    QTimer          *m_errorTimer;
    QElapsedTimer   m_elapsed;
    bool            m_started;
    QMap<int, QString> m_keyMap;

    void applyLanguage(Language lang);
    void resetState();
    void refreshTextDisplay();
    void buildKeyboard();
    void updateStats();
};

#endif