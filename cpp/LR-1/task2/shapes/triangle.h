#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "polygon.h"

class Triangle : public Polygon
{
    Q_OBJECT
public:
    explicit Triangle(QObject* parent = nullptr);
    Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3, QObject* parent = nullptr);
    virtual ~Triangle();

    QString typeName() const override;

    void setPoints(const QPointF& p1, const QPointF& p2, const QPointF& p3);
    QPointF p1() const;
    QPointF p2() const;
    QPointF p3() const;
};

#endif