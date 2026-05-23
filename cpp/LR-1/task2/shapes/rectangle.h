#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "polygon.h"

class Rectangle : public Polygon
{
    Q_OBJECT
public:
    explicit Rectangle(QObject* parent = nullptr);
    Rectangle(const QPointF& topLeft, double width, double height, QObject* parent = nullptr);
    virtual ~Rectangle();

    QString typeName() const override;

    void setRect(const QPointF& topLeft, double width, double height);
    QPointF topLeft() const;
    double width() const;
    double height() const;

private:
    void updateVertices(const QPointF& topLeft, double width, double height);
};

#endif