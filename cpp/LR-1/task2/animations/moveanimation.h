#ifndef MOVEANIMATION_H
#define MOVEANIMATION_H

#include <QObject>
#include <QTimer>
#include "../shapes/shape.h"

class MoveAnimation : public QObject
{
    Q_OBJECT
public:
    explicit MoveAnimation(Shape* shape, double dx, double dy, int durationMs, QObject* parent = nullptr);
    ~MoveAnimation();
    void start();
    void stop();

signals:
    void finished();

private slots:
    void step();

private:
    Shape* m_shape;
    double m_dx, m_dy, m_stepDx, m_stepDy;
    int m_duration, m_elapsed;
    QTimer* m_timer;
};

#endif