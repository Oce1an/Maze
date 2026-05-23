#include "circle.h"
#include <QtMath>
#include <stdexcept>

Circle::Circle(QObject* parent) : Shape(parent), m_center(0, 0), m_radius(1) {}

Circle::Circle(const QPointF& center, double radius, QObject* parent)
    : Shape(parent), m_center(center), m_radius(radius)
{
    checkPositive(radius, "radius");
}

Circle::~Circle() {}

double Circle::area() const { return M_PI * m_radius * m_radius; }
double Circle::perimeter() const { return 2 * M_PI * m_radius; }
QPointF Circle::centerOfMass() const { return m_center; }

void Circle::move(double dx, double dy)
{
    m_center += QPointF(dx, dy);
    emit shapeChanged();
}

bool Circle::contains(const QPointF& point) const
{
    double dx = point.x() - m_center.x();
    double dy = point.y() - m_center.y();
    return (dx * dx + dy * dy) <= m_radius * m_radius;
}

void Circle::rotate(double /*angle*/, const QPointF&/*center*/) { /* ничего не делаем */ }

void Circle::scale(double factor, const QPointF& center)
{
    checkPositive(factor, "factor");
    QPointF vec = m_center - center;
    m_center = center + vec * factor;
    m_radius *= factor;
    emit shapeChanged();
}

void Circle::draw(QPainter& painter) const
{
    painter.drawEllipse(m_center, m_radius, m_radius);
}

QString Circle::typeName() const { return "Circle"; }

void Circle::setCenter(const QPointF& center) { m_center = center; emit shapeChanged(); }
QPointF Circle::center() const { return m_center; }
void Circle::setRadius(double radius) { checkPositive(radius, "radius"); m_radius = radius; emit shapeChanged(); }
double Circle::radius() const { return m_radius; }