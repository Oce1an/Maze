#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class Ellipse : public Shape
{
    Q_OBJECT
public:
    explicit Ellipse(QObject* parent = nullptr);
    Ellipse(const QPointF& center, double rx, double ry, double angle = 0, QObject* parent = nullptr);
    virtual ~Ellipse();

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
    void setRadii(double rx, double ry);
    double radiusX() const;
    double radiusY() const;
    void setRotation(double angle);
    double rotation() const;

private:
    QPointF m_center;
    double m_rx, m_ry, m_angle;
};

#endif