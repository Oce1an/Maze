#include "../hdr/MovingRectangle.h"
#include <QPainter>

MovingRectangle::MovingRectangle(int x, int y, int width, int height) // Конструктор вне класса "::" = принадлежит
	: m_position(x, y), m_width(width), m_height(height) // Члены класса
{

}

void MovingRectangle::moveLeft(int step)
{
	m_position.setX(m_position.x() - step); // setX() и setY() методы QPoint
}

void MovingRectangle::moveRight(int step)
{
	m_position.setX(m_position.x() + step);
}

void MovingRectangle::moveUp(int step)
{
	m_position.setY(m_position.y() - step);
}

void MovingRectangle::moveDown(int step)
{
	m_position.setY(m_position.y() + step);
}

void MovingRectangle::setPosition(int x, int y)
{
	m_position.setX(x);
	m_position.setY(y);
}

int MovingRectangle::x() const 
{
	return m_position.x();
}

int MovingRectangle::y() const 
{
	return m_position.y();
}

int MovingRectangle::width() const
{
	return m_width;
}

int MovingRectangle::height() const
{
	return m_height;
}

void MovingRectangle::draw(QPainter& painter) const
{
	painter.drawRect(m_position.x(), m_position.y(), m_width, m_height);
}