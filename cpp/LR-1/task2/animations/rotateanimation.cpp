#include "rotateanimation.h"
#include <QtMath>

RotateAnimation::RotateAnimation(Shape* shape, double totalAngle, const QPointF& center, int durationMs, QObject* parent)
    : QObject(parent), m_shape(shape), m_totalAngle(totalAngle), m_center(center), m_duration(durationMs), m_elapsed(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &RotateAnimation::step);
    int steps = durationMs / 50;
    if (steps == 0) steps = 1;
    m_stepAngle = totalAngle / steps;
}

RotateAnimation::~RotateAnimation() { stop(); }

void RotateAnimation::start()
{
    m_elapsed = 0;
    m_timer->start(50);
}

void RotateAnimation::stop() { m_timer->stop(); }

void RotateAnimation::step()
{
    m_elapsed += 50;
    if (m_elapsed >= m_duration) {
        m_timer->stop();
        double remaining = m_totalAngle - m_stepAngle * (m_elapsed / 50 - 1);
        if (qAbs(remaining) > 1e-6)
            m_shape->rotate(remaining, m_center);
        emit finished();
    }
    else {
        m_shape->rotate(m_stepAngle, m_center);
    }
    emit m_shape->shapeChanged();
}