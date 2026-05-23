#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape
{
    Q_OBJECT
public:
    explicit Circle(QObject* parent = nullptr);
    Circle(const QPointF& center, double radius, QObject* parent = nullptr);
    virtual ~Circle();

    double area() const override;
    double perimeter() const override;
    QPointF centerOfMass() const override;
    void move(double dx, double dy) override;
    void rotate(double angle, const QPointF& center) override;
    void scale(double factor, const QPointF& center) override;
    void draw(QPainter& painter) const override;
    QString typeName() const override;
    bool contains(const QPointF& point) const override;

    void setCenter(const QPointF& center);
    QPointF center() const;
    void setRadius(double radius);
    double radius() const;

private:
    QPointF m_center;
    double m_radius;
};

#endif