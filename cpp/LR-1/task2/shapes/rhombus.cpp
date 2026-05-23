#include "rhombus.h"
#include <QtMath>
#include <stdexcept>

Rhombus::Rhombus(QObject* parent) : Polygon(parent), m_center(0, 0), m_diag1(100), m_diag2(60), m_angle(0)
{
    updateVertices();
}

Rhombus::Rhombus(const QPointF& center, double diag1, double diag2, double angle, QObject* parent)
    : Polygon(parent), m_center(center), m_diag1(diag1), m_diag2(diag2), m_angle(angle)
{
    if (diag1 <= 0 || diag2 <= 0)
        throw std::invalid_argument("Diagonals must be positive");
    updateVertices();
}

Rhombus::~Rhombus() {}

QString Rhombus::typeName() const { return "Rhombus"; }

void Rhombus::setCenter(const QPointF& center) { m_center = center; updateVertices(); emit shapeChanged(); }
QPointF Rhombus::center() const { return m_center; }
void Rhombus::setDiagonals(double diag1, double diag2) { m_diag1 = diag1; m_diag2 = diag2; updateVertices(); emit shapeChanged(); }
double Rhombus::diagonal1() const { return m_diag1; }
double Rhombus::diagonal2() const { return m_diag2; }
void Rhombus::setRotation(double angle) { m_angle = angle; updateVertices(); emit shapeChanged(); }
double Rhombus::rotation() const { return m_angle; }

void Rhombus::updateVertices()
{
    double rad = qDegreesToRadians(m_angle);
    double cosA = qCos(rad);
    double sinA = qSin(rad);
    auto rot = [&](const QPointF& p) {
        return QPointF(p.x() * cosA - p.y() * sinA, p.x() * sinA + p.y() * cosA);
        };
    QPointF v1(m_diag1 / 2, 0);
    QPointF v2(0, m_diag2 / 2);
    QPointF v3(-m_diag1 / 2, 0);
    QPointF v4(0, -m_diag2 / 2);
    m_vertices.clear();
    m_vertices.push_back(m_center + rot(v1));
    m_vertices.push_back(m_center + rot(v2));
    m_vertices.push_back(m_center + rot(v3));
    m_vertices.push_back(m_center + rot(v4));
}