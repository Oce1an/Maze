#include "rectangle.h"
#include <stdexcept>

Rectangle::Rectangle(QObject* parent) : Polygon(parent)
{
    updateVertices(QPointF(0, 0), 100, 50);
}

Rectangle::Rectangle(const QPointF& topLeft, double width, double height, QObject* parent)
    : Polygon(parent)
{
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Width and height must be positive");
    updateVertices(topLeft, width, height);
}

Rectangle::~Rectangle() {}

QString Rectangle::typeName() const { return "Rectangle"; }

void Rectangle::setRect(const QPointF& topLeft, double width, double height)
{
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Width and height must be positive");
    updateVertices(topLeft, width, height);
    emit shapeChanged();
}

QPointF Rectangle::topLeft() const { return m_vertices[0]; }
double Rectangle::width() const { return m_vertices[1].x() - m_vertices[0].x(); }
double Rectangle::height() const { return m_vertices[3].y() - m_vertices[0].y(); }

void Rectangle::updateVertices(const QPointF& topLeft, double width, double height)
{
    m_vertices.clear();
    m_vertices.push_back(topLeft);
    m_vertices.push_back(QPointF(topLeft.x() + width, topLeft.y()));
    m_vertices.push_back(QPointF(topLeft.x() + width, topLeft.y() + height));
    m_vertices.push_back(QPointF(topLeft.x(), topLeft.y() + height));
}