#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QPainter>
#include <QPointF>
#include <QString>

class Shape : public QObject
{
    Q_OBJECT
public:
    explicit Shape(QObject* parent = nullptr);
    virtual ~Shape();

    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual QPointF centerOfMass() const = 0;

    virtual void move(double dx, double dy) = 0;
    virtual void rotate(double angle, const QPointF& center) = 0;
    virtual void scale(double factor, const QPointF& center) = 0;

    virtual void moveCenterTo(const QPointF& newCenter);

    virtual void draw(QPainter& painter) const = 0;
    virtual QString typeName() const = 0;
    virtual bool contains(const QPointF& point) const = 0;

signals:
    void shapeChanged();

protected:
    void checkPositive(double value, const QString& paramName) const;
};

#endif