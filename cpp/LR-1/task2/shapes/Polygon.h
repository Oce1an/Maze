#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"
#include <vector>

class Polygon : public Shape
{
    Q_OBJECT
public:
    explicit Polygon(QObject* parent = nullptr);
    explicit Polygon(const std::vector<QPointF>& vertices, QObject* parent = nullptr);
    virtual ~Polygon();

    double area() const override;
    double perimeter() const override;
    QPointF centerOfMass() const override;
    void move(double dx, double dy) override;
    void rotate(double angle, const QPointF& center) override;
    void scale(double factor, const QPointF& center) override;
    void draw(QPainter& painter) const override;
    QString typeName() const override;
    bool contains(const QPointF& point) const override;

    void setVertices(const std::vector<QPointF>& vertices);
    std::vector<QPointF> vertices() const;
    int vertexCount() const;
    void setVertex(int index, const QPointF& point);
    QPointF vertex(int index) const;

protected:
    std::vector<QPointF> m_vertices;
};

#endif