#ifndef ROTATEANIMATION_H
#define ROTATEANIMATION_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include "../shapes/shape.h"

class RotateAnimation : public QObject
{
    Q_OBJECT
public:
    explicit RotateAnimation(Shape* shape, double totalAngle, const QPointF& center, int durationMs, QObject* parent = nullptr);
    ~RotateAnimation();
    void start();
    void stop();

signals:
    void finished();

private slots:
    void step();

private:
    Shape* m_shape;
    double m_totalAngle, m_stepAngle;
    QPointF m_center;
    int m_duration, m_elapsed;
    QTimer* m_timer;
};

#endif