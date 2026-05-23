#include "ellipse.h"
#include <QtMath>
#include <stdexcept>

Ellipse::Ellipse(QObject* parent) : Shape(parent), m_center(0, 0), m_rx(100), m_ry(60), m_angle(0) {}
Ellipse::Ellipse(const QPointF& center, double rx, double ry, double angle, QObject* parent)
    : Shape(parent), m_center(center), m_rx(rx), m_ry(ry), m_angle(angle)
{
    if (rx <= 0 || ry <= 0)
        throw std::invalid_argument("Radii must be positive");
}
Ellipse::~Ellipse() {}

double Ellipse::area() const { return M_PI * m_rx * m_ry; }
double Ellipse::perimeter() const
{
    double h = qPow((m_rx - m_ry) / (m_rx + m_ry), 2);
    return M_PI * (m_rx + m_ry) * (1 + (3 * h) / (10 + qSqrt(4 - 3 * h)));
}
QPointF Ellipse::centerOfMass() const { return m_center; }

void Ellipse::move(double dx, double dy)
{
    m_center += QPointF(dx, dy);
    emit shapeChanged();
}

void Ellipse::rotate(double angle, const QPointF& center)
{
    double rad = qDegreesToRadians(angle);
    double cosA = qCos(rad);
    double sinA = qSin(rad);
    double x = m_center.x() - center.x();
    double y = m_center.y() - center.y();
    m_center.setX(center.x() + x * cosA - y * sinA);
    m_center.setY(center.y() + x * sinA + y * cosA);
    m_angle += angle;
    emit shapeChanged();
}

void Ellipse::scale(double factor, const QPointF& center)
{
    if (factor <= 0)
        throw std::invalid_argument("Scale factor must be positive");
    QPointF vec = m_center - center;
    m_center = center + vec * factor;
    m_rx *= factor;
    m_ry *= factor;
    emit shapeChanged();
}

void Ellipse::draw(QPainter& painter) const
{
    painter.save();
    painter.translate(m_center);
    painter.rotate(m_angle);
    painter.drawEllipse(QPointF(0, 0), m_rx, m_ry);
    painter.restore();
}

QString Ellipse::typeName() const { return "Ellipse"; }

void Ellipse::setCenter(const QPointF& center) { m_center = center; emit shapeChanged(); }
QPointF Ellipse::center() const { return m_center; }
void Ellipse::setRadii(double rx, double ry) { m_rx = rx; m_ry = ry; emit shapeChanged(); }
double Ellipse::radiusX() const { return m_rx; }
double Ellipse::radiusY() const { return m_ry; }
void Ellipse::setRotation(double angle) { m_angle = angle; emit shapeChanged(); }
double Ellipse::rotation() const { return m_angle; }

bool Ellipse::contains(const QPointF& point) const
{
    double dx = point.x() - m_center.x();
    double dy = point.y() - m_center.y();
    double rad = qDegreesToRadians(-m_angle);
    double cosA = qCos(rad);
    double sinA = qSin(rad);
    double x = dx * cosA - dy * sinA;
    double y = dx * sinA + dy * cosA;
    return (x * x) / (m_rx * m_rx) + (y * y) / (m_ry * m_ry) <= 1.0;
}