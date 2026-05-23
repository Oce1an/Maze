#include "../hdr/Car.h"
#include <QPainter>
#include <QBrush>
#include <QPen>

Car::Car(int x, int y)
	: MovingRectangle(x, y, 200, 80) // Размеры автомобиля
	, m_doorsOpen(false), m_headLightsOn(false)
{
	
}

void Car::openDoors()
{
	m_doorsOpen = true;
}

void Car::closeDoors()
{
	m_doorsOpen = false;
}

bool Car::areDoorsOpen() const
{
    return m_doorsOpen;
}

void Car::turnHeadlightsOn()
{
	m_headLightsOn = true;
}

void Car::turnHeadlightsOff()
{
	m_headLightsOn = false;
}

bool Car::areHeadlightsOn() const
{
	return m_headLightsOn;
}

void Car::draw(QPainter& painter) const
{
	painter.save();

	// Корпус
	QRect body(x(), y(), width(), height());
	painter.setBrush(QBrush(Qt::blue)); // GlobalColor цвет корпуса
	painter.setPen(QPen(Qt::black, 2)); // Окантовка, ширина
	painter.drawRect(body);

	// Кабина
	QRect cabin(x() + width() / 4, // расположение кабины по Ox
				y() - height() / 2, // расположение кабины по Oy
				width() / 2, // ширина кабины
				height() / 2); // высота кабины
	painter.setBrush(QBrush(Qt::cyan));
	painter.drawRect(cabin);

	//Колеса
	painter.setBrush(QBrush(Qt::black));
	int wheelRadius = height() / 4;
	painter.drawEllipse(QPoint(x() + wheelRadius, y() + height()), // расположение по Oxy
							   wheelRadius, // радиус по Ox
							   wheelRadius);

	painter.drawEllipse(QPoint(x() + width() - wheelRadius, y() + height()), // расположение по Oxy
							   wheelRadius, // радиус по Ox
							   wheelRadius); // радиус по Oy


	// Передние фары
	if(m_headLightsOn)
	{
		painter.setBrush(QBrush(Qt::yellow));
	} else 
	{
		painter.setBrush(QBrush(Qt::gray));
	}
	painter.drawEllipse(QPoint(x() + width() - 5, y() + height() / 3),
						8,
						8);
	painter.drawEllipse(QPoint(x() + width() - 5, y() + 2 * height() / 3),
						8,	
						8);

	// Задние фары
	painter.setBrush(QBrush(Qt::red));
	painter.drawEllipse(QPoint(x() + 5, y() + height() / 3),
						6,
						6);
	painter.drawEllipse(QPoint(x() + 5, y() + 2 * height() / 3),
						6,
						6);
	// Двери
	if(m_doorsOpen)
	{
		painter.setPen(QPen(Qt::darkGray, 3));

		// Левая дверь
		painter.drawLine(x() + width() / 4, // Координаты точки 1 по Ox
						 y(), // Координаты точки 1 по Oy
						 x() + width() / 4 - 20, // Координаты точки 2 по Ox
						 y() + height() / 2); // Координаты точки 2 по Oy
		// Правая дверь
		painter.drawLine(x() + 3 * width() / 4,
						 y(),
						 x() + 3 * width() / 4 + 20,
						 y() + height() / 2);
	} else
	{
		painter.setPen(QPen(Qt::black, 2));

		painter.drawLine(x() + width() / 4,
						 y(),
						 x() + width() / 4,
						 y() + height());

		painter.drawLine(x() + 3 * width() / 4,
						 y(),
						 x() + 3 * width() / 4,
						 y() + height());
	}

	painter.restore();
}