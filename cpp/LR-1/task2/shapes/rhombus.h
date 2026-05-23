#ifndef RHOMBUS_H
#define RHOMBUS_H

#include "polygon.h"

class Rhombus : public Polygon
{
    Q_OBJECT
public:
    explicit Rhombus(QObject* parent = nullptr);
    Rhombus(const QPointF& center, double diag1, double diag2, double angle = 0, QObject* parent = nullptr);
    virtual ~Rhombus();

    QString typeName() const override;

    void setCenter(const QPointF& center);
    QPointF center() const;
    void setDiagonals(double diag1, double diag2);
    double diagonal1() const;
    double diagonal2() const;
    void setRotation(double angle);
    double rotation() const;

private:
    void updateVertices();
    QPointF m_center;
    double m_diag1, m_diag2, m_angle;
};

#endif