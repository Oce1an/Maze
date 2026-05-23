#include "../hdr/MainWindow.h"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	// Виджет рисования
	m_drawingWidget = new DrawingWidget(this);
	m_car = m_drawingWidget->car();

	setupUI();
	setWindowTitle("CAR");
	setFixedSize(1200, 700);

}

MainWindow::~MainWindow()
{
	
}

void MainWindow::setupUI()
{
	QWidget *central = new QWidget(this);
	setCentralWidget(central);

	QHBoxLayout *mainLayout = new QHBoxLayout(central);

	// Область рисования
	mainLayout->addWidget(m_drawingWidget);

	// Панель 
	QWidget *controlPanel = new QWidget;
    QVBoxLayout *panelLayout = new QVBoxLayout(controlPanel);

	// Группа "Перемещение"
    QGroupBox *moveGroup = new QGroupBox(tr("Movement"));
    QVBoxLayout *moveLayout = new QVBoxLayout(moveGroup);

	// Кнопки направлений
    QHBoxLayout *dirLayout1 = new QHBoxLayout;
    m_btnUp = new QPushButton(tr("Up"));
    dirLayout1->addWidget(m_btnUp);
    moveLayout->addLayout(dirLayout1);

    QHBoxLayout *dirLayout2 = new QHBoxLayout;
    m_btnLeft = new QPushButton(tr("Left"));
    m_btnRight = new QPushButton(tr("Right"));
    m_btnDown = new QPushButton(tr("Down"));
    dirLayout2->addWidget(m_btnLeft);
    dirLayout2->addWidget(m_btnDown);
    dirLayout2->addWidget(m_btnRight);
    moveLayout->addLayout(dirLayout2);

	// Перемещение в точку
    QHBoxLayout *coordLayout = new QHBoxLayout;
    coordLayout->addWidget(new QLabel(tr("X:")));
    m_editX = new QLineEdit;
    m_editX->setValidator(new QIntValidator(0, 400, this));
    coordLayout->addWidget(m_editX);
    coordLayout->addWidget(new QLabel(tr("Y:")));
    m_editY = new QLineEdit;
    m_editY->setValidator(new QIntValidator(0, 400, this));
    coordLayout->addWidget(m_editY);
    m_btnMoveTo = new QPushButton(tr("Move To"));
    coordLayout->addWidget(m_btnMoveTo);
    moveLayout->addLayout(coordLayout);

    panelLayout->addWidget(moveGroup);

     // Группа "Двери и Фары"
    QGroupBox *featuresGroup = new QGroupBox(tr("Features"));
    QVBoxLayout *featuresLayout = new QVBoxLayout(featuresGroup);

    m_btnToggleDoors = new QPushButton(tr("Open Doors"));
    m_btnToggleDoors->setCheckable(true);
    featuresLayout->addWidget(m_btnToggleDoors);

    m_btnToggleHeadlights = new QPushButton(tr("Turn Headlights On"));
    m_btnToggleHeadlights->setCheckable(true);
    featuresLayout->addWidget(m_btnToggleHeadlights);

    panelLayout->addWidget(featuresGroup);

     // Статус
    m_statusLabel = new QLabel;
    updateStatusLabel();
    panelLayout->addWidget(m_statusLabel);

    panelLayout->addStretch();

    mainLayout->addWidget(controlPanel);

    // Подключаем сигналы к слотам
    connect(m_btnLeft, &QPushButton::clicked, this, &MainWindow::moveLeft);
    connect(m_btnRight, &QPushButton::clicked, this, &MainWindow::moveRight);
    connect(m_btnUp, &QPushButton::clicked, this, &MainWindow::moveUp);
    connect(m_btnDown, &QPushButton::clicked, this, &MainWindow::moveDown);
    connect(m_btnMoveTo, &QPushButton::clicked, this, &MainWindow::moveTo);
    connect(m_btnToggleDoors, &QPushButton::clicked, this, &MainWindow::toggleDoors);
    connect(m_btnToggleHeadlights, &QPushButton::clicked, this, &MainWindow::toggleHeadlights);
}

void MainWindow::updateStatusLabel()
{
    QString doorsStatus = m_car->areDoorsOpen() ? "Open" : "Closed";
    QString lightsStatus = m_car->areHeadlightsOn() ? "On" : "Off";
    
    m_statusLabel->setText(tr("Doors: %1, Headlights: %2, Position: (%3, %4)")
                           .arg(doorsStatus)
                           .arg(lightsStatus)
                           .arg(m_car->x())
                           .arg(m_car->y()));
}

void MainWindow::moveLeft()
{
    m_car->moveLeft();
    m_drawingWidget->update();
    updateStatusLabel();
}

void MainWindow::moveRight()
{
    m_car->moveRight();
    m_drawingWidget->update();
    updateStatusLabel();
}

void MainWindow::moveUp()
{
    m_car->moveUp();
    m_drawingWidget->update();
    updateStatusLabel();
}

void MainWindow::moveDown()
{
    m_car->moveDown();
    m_drawingWidget->update();
    updateStatusLabel();
}

void MainWindow::moveTo()
{
    bool okX, okY;
    int newX = m_editX->text().toInt(&okX);
    int newY = m_editY->text().toInt(&okY);

    if (!okX || !okY) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please enter valid integer coordinates."));
        return;
    }

    // Проверка, чтобы автомобиль не выходил за пределы виджета
    if (newX < 0 || newX > m_drawingWidget->width() - m_car->width() ||
        newY < 0 || newY > m_drawingWidget->height() - m_car->height()) {
        QMessageBox::warning(this, tr("Boundary Error"), tr("Coordinates out of visible area."));
        return;
    }

    m_car->setPosition(newX, newY);
    m_drawingWidget->update();
    updateStatusLabel();
}

void MainWindow::toggleDoors()
{
    if (m_btnToggleDoors->isChecked()) {
        m_car->openDoors();
        m_btnToggleDoors->setText(tr("Close Doors"));
    } else {
        m_car->closeDoors();
        m_btnToggleDoors->setText(tr("Open Doors"));
    }
    m_drawingWidget->update();
    updateStatusLabel();
}

void MainWindow::toggleHeadlights()
{
    if (m_btnToggleHeadlights->isChecked()) {
        m_car->turnHeadlightsOn();
        m_btnToggleHeadlights->setText(tr("Turn Headlights Off"));
    } else {
        m_car->turnHeadlightsOff();
        m_btnToggleHeadlights->setText(tr("Turn Headlights On"));
    }
    m_drawingWidget->update();
    updateStatusLabel();
}

