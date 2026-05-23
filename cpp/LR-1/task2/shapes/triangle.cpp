#include "triangle.h"

Triangle::Triangle(QObject* parent) : Polygon(parent)
{
    m_vertices = { QPointF(0,0), QPointF(100,0), QPointF(0,100) };
}

Triangle::Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3, QObject* parent)
    : Polygon(parent)
{
    m_vertices = { p1, p2, p3 };
}

Triangle::~Triangle() {}

QString Triangle::typeName() const { return "Triangle"; }

void Triangle::setPoints(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    m_vertices = { p1, p2, p3 };
    emit shapeChanged();
}

QPointF Triangle::p1() const { return m_vertices[0]; }
QPointF Triangle::p2() const { return m_vertices[1]; }
QPointF Triangle::p3() const { return m_vertices[2]; }