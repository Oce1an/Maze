#include "shape.h"
#include <stdexcept>

Shape::Shape(QObject* parent) : QObject(parent)
{
}

Shape::~Shape()
{
}

void Shape::moveCenterTo(const QPointF& newCenter)
{
    QPointF old = centerOfMass();
    move(newCenter.x() - old.x(), newCenter.y() - old.y());
}

void Shape::checkPositive(double value, const QString& paramName) const
{
    if (value <= 0) {
        throw std::invalid_argument(("Parameter " + paramName + " must be positive").toStdString());
    }
}