#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QList>
#include <QSharedPointer>
#include <QPointF>
#include "shapes/shape.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget* parent = nullptr);

    void addShape(QSharedPointer<Shape> shape);
    void removeShape(int index);
    void clear();
    QSharedPointer<Shape> shapeAt(int index) const;
    int shapeCount() const;
    QSharedPointer<Shape> selectedShape() const;
    int selectedIndex() const { return m_selectedIndex; }
    void setSelectedIndex(int index);

signals:
    void selectionChanged(int index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QList<QSharedPointer<Shape>> m_shapes;
    int m_selectedIndex;
    bool m_dragging;
    QPointF m_lastMousePos;
};

#endif