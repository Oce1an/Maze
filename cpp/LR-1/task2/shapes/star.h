#ifndef STAR_H
#define STAR_H

#include "polygon.h"

class Star : public Polygon
{
    Q_OBJECT
public:
    explicit Star(QObject* parent = nullptr);
    Star(const QPointF& center, double outerRadius, double innerRadius, int points, double angle = 0, QObject* parent = nullptr);
    virtual ~Star();

    QString typeName() const override;

    void setCenter(const QPointF& center);
    QPointF center() const;
    void setRadii(double outer, double inner);
    double outerRadius() const;
    double innerRadius() const;
    void setPoints(int points);
    int points() const;
    void setRotation(double angle);
    double rotation() const;

private:
    void updateVertices();
    QPointF m_center;
    double m_outerRadius, m_innerRadius;
    int m_points;
    double m_angle;
};

#endif