#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DrawingWidget.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void moveTo();

    void toggleDoors();
    void toggleHeadlights();

private:
    void setupUI();
    void updateStatusLabel();

    DrawingWidget *m_drawingWidget;
    Car *m_car;

    QPushButton *m_btnLeft;
    QPushButton *m_btnRight;
    QPushButton *m_btnUp;
    QPushButton *m_btnDown;
    QLineEdit *m_editX;
    QLineEdit *m_editY;
    QPushButton *m_btnMoveTo;
    QPushButton *m_btnToggleDoors;
    QPushButton *m_btnToggleHeadlights;
    QLabel *m_statusLabel;
};

#endif