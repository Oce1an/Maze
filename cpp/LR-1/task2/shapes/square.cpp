#include "square.h"
#include <QSharedPointer>
#include <stdexcept>

Square::Square(QObject* parent) : Rectangle(QPointF(0, 0), 100, 100, parent) {}

Square::Square(const QPointF& topLeft, double side, QObject* parent)
    : Rectangle(topLeft, side, side, parent)
{
    if (side <= 0)
        throw std::invalid_argument("Side must be positive");
}

Square::~Square() {}

QString Square::typeName() const { return "Square"; }

void Square::setSide(double side)
{
    if (side <= 0)
        throw std::invalid_argument("Side must be positive");
    setRect(topLeft(), side, side);
}

double Square::side() const { return width(); }

QSharedPointer<Square> Square::fromCenter(const QPointF& center, double side, QObject* parent)
{
    QPointF topLeft(center.x() - side/2, center.y() - side/2);
    return QSharedPointer<Square>::create(topLeft, side, parent);
}

QPointF Square::center() const
{
    return QPointF(topLeft().x() + side()/2, topLeft().y() + side()/2);
}

void Square::setCenter(const QPointF& center)
{
    QPointF newTopLeft(center.x() - side()/2, center.y() - side()/2);
    setRect(newTopLeft, side(), side());
}