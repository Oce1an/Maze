#ifndef SCALEANIMATION_H
#define SCALEANIMATION_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include "../shapes/shape.h"

class ScaleAnimation : public QObject
{
    Q_OBJECT
public:
    explicit ScaleAnimation(Shape* shape, double factor, const QPointF& center, int durationMs, QObject* parent = nullptr);
    ~ScaleAnimation();
    void start();
    void stop();

signals:
    void finished();

private slots:
    void step();

private:
    Shape* m_shape;
    double m_factor, m_stepFactor;
    QPointF m_center;
    int m_duration, m_elapsed;
    QTimer* m_timer;
};

#endif