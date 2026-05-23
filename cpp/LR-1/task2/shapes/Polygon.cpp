#include "polygon.h"
#include <QPolygonF>
#include <QtMath>
#include <stdexcept>

Polygon::Polygon(QObject* parent) : Shape(parent)
{
}

Polygon::Polygon(const std::vector<QPointF>& vertices, QObject* parent)
    : Shape(parent), m_vertices(vertices)
{
    if (vertices.size() < 3)
        throw std::invalid_argument("Polygon must have at least 3 vertices");
}

Polygon::~Polygon()
{
}

double Polygon::area() const
{
    double a = 0;
    int n = m_vertices.size();
    for (int i = 0; i < n; ++i) {
        const QPointF& p1 = m_vertices[i];
        const QPointF& p2 = m_vertices[(i + 1) % n];
        a += p1.x() * p2.y() - p2.x() * p1.y();
    }
    return qAbs(a) / 2.0;
}

double Polygon::perimeter() const
{
    double p = 0;
    int n = m_vertices.size();
    for (int i = 0; i < n; ++i) {
        const QPointF& p1 = m_vertices[i];
        const QPointF& p2 = m_vertices[(i + 1) % n];
        p += QLineF(p1, p2).length();
    }
    return p;
}

QPointF Polygon::centerOfMass() const
{
    if (m_vertices.empty())
        return QPointF();
    double sx = 0, sy = 0;
    for (const QPointF& p : m_vertices) {
        sx += p.x();
        sy += p.y();
    }
    return QPointF(sx / m_vertices.size(), sy / m_vertices.size());
}

void Polygon::move(double dx, double dy)
{
    for (QPointF& p : m_vertices) {
        p += QPointF(dx, dy);
    }
    emit shapeChanged();
}

void Polygon::rotate(double angle, const QPointF& center)
{
    double rad = qDegreesToRadians(angle);
    double cosA = qCos(rad);
    double sinA = qSin(rad);
    for (QPointF& p : m_vertices) {
        double x = p.x() - center.x();
        double y = p.y() - center.y();
        p.setX(center.x() + x * cosA - y * sinA);
        p.setY(center.y() + x * sinA + y * cosA);
    }
    emit shapeChanged();
}

void Polygon::scale(double factor, const QPointF& center)
{
    if (factor <= 0)
        throw std::invalid_argument("Scale factor must be positive");
    for (QPointF& p : m_vertices) {
        double x = p.x() - center.x();
        double y = p.y() - center.y();
        p.setX(center.x() + x * factor);
        p.setY(center.y() + y * factor);
    }
    emit shapeChanged();
}

void Polygon::draw(QPainter& painter) const
{
    if (m_vertices.empty()) return;
    QPolygonF poly;
    for (const QPointF& p : m_vertices) poly << p;
    painter.drawPolygon(poly);
}

QString Polygon::typeName() const
{
    return "Polygon";
}

void Polygon::setVertices(const std::vector<QPointF>& vertices)
{
    if (vertices.size() < 3)
        throw std::invalid_argument("Polygon must have at least 3 vertices");
    m_vertices = vertices;
    emit shapeChanged();
}

std::vector<QPointF> Polygon::vertices() const
{
    return m_vertices;
}

int Polygon::vertexCount() const
{
    return m_vertices.size();
}

void Polygon::setVertex(int index, const QPointF& point)
{
    if (index < 0 || index >= (int)m_vertices.size())
        throw std::out_of_range("Vertex index out of range");
    m_vertices[index] = point;
    emit shapeChanged();
}

QPointF Polygon::vertex(int index) const
{
    if (index < 0 || index >= (int)m_vertices.size())
        throw std::out_of_range("Vertex index out of range");
    return m_vertices[index];
}
bool Polygon::contains(const QPointF& point) const
{
    if (m_vertices.empty())
        return false;
    QPolygonF poly;
    for (const auto& p : m_vertices)
        poly << p;
    return poly.containsPoint(point, Qt::OddEvenFill);
}