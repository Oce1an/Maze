#include "hexagon.h"
#include <QtMath>
#include <stdexcept>

Hexagon::Hexagon(QObject* parent) : Polygon(parent), m_center(0, 0), m_radius(100), m_angle(0)
{
    updateVertices();
}

Hexagon::Hexagon(const QPointF& center, double radius, double angle, QObject* parent)
    : Polygon(parent), m_center(center), m_radius(radius), m_angle(angle)
{
    if (radius <= 0)
        throw std::invalid_argument("Radius must be positive");
    updateVertices();
}

Hexagon::~Hexagon() {}

QString Hexagon::typeName() const { return "Hexagon"; }

void Hexagon::setCenter(const QPointF& center) { m_center = center; updateVertices(); emit shapeChanged(); }
QPointF Hexagon::center() const { return m_center; }
void Hexagon::setRadius(double radius) { m_radius = radius; updateVertices(); emit shapeChanged(); }
double Hexagon::radius() const { return m_radius; }
void Hexagon::setRotation(double angle) { m_angle = angle; updateVertices(); emit shapeChanged(); }
double Hexagon::rotation() const { return m_angle; }

void Hexagon::updateVertices()
{
    m_vertices.clear();
    double start = qDegreesToRadians(m_angle);
    for (int i = 0; i < 6; ++i) {
        double a = start + i * M_PI / 3;
        m_vertices.push_back(m_center + QPointF(m_radius * qCos(a), m_radius * qSin(a)));
    }
}