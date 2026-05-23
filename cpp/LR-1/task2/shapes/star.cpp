#include "star.h"
#include <QtMath>
#include <stdexcept>

Star::Star(QObject* parent) : Polygon(parent), m_center(0, 0), m_outerRadius(100), m_innerRadius(50), m_points(5), m_angle(0)
{
    updateVertices();
}

Star::Star(const QPointF& center, double outerRadius, double innerRadius, int points, double angle, QObject* parent)
    : Polygon(parent), m_center(center), m_outerRadius(outerRadius), m_innerRadius(innerRadius), m_points(points), m_angle(angle)
{
    if (outerRadius <= 0 || innerRadius <= 0)
        throw std::invalid_argument("Radii must be positive");
    if (points < 3)
        throw std::invalid_argument("Star must have at least 3 points");
    updateVertices();
}

Star::~Star() {}

QString Star::typeName() const { return "Star"; }

void Star::setCenter(const QPointF& center) { m_center = center; updateVertices(); emit shapeChanged(); }
QPointF Star::center() const { return m_center; }
void Star::setRadii(double outer, double inner) { m_outerRadius = outer; m_innerRadius = inner; updateVertices(); emit shapeChanged(); }
double Star::outerRadius() const { return m_outerRadius; }
double Star::innerRadius() const { return m_innerRadius; }
void Star::setPoints(int points) { m_points = points; updateVertices(); emit shapeChanged(); }
int Star::points() const { return m_points; }
void Star::setRotation(double angle) { m_angle = angle; updateVertices(); emit shapeChanged(); }
double Star::rotation() const { return m_angle; }

void Star::updateVertices()
{
    m_vertices.clear();
    double step = M_PI / m_points;
    double start = qDegreesToRadians(m_angle);
    for (int i = 0; i < 2 * m_points; ++i) {
        double r = (i % 2 == 0) ? m_outerRadius : m_innerRadius;
        double a = start + i * step;
        m_vertices.push_back(m_center + QPointF(r * qCos(a), r * qSin(a)));
    }
}