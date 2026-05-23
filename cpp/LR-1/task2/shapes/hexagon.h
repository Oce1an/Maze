#ifndef HEXAGON_H
#define HEXAGON_H

#include "polygon.h"

class Hexagon : public Polygon
{
    Q_OBJECT
public:
    explicit Hexagon(QObject* parent = nullptr);
    Hexagon(const QPointF& center, double radius, double angle = 0, QObject* parent = nullptr);
    virtual ~Hexagon();

    QString typeName() const override;

    void setCenter(const QPointF& center);
    QPointF center() const;
    void setRadius(double radius);
    double radius() const;
    void setRotation(double angle);
    double rotation() const;

private:
    void updateVertices();
    QPointF m_center;
    double m_radius, m_angle;
};

#endif