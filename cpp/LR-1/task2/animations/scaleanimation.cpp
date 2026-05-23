#include "scaleanimation.h"

ScaleAnimation::ScaleAnimation(Shape* shape, double factor, const QPointF& center, int durationMs, QObject* parent)
    : QObject(parent), m_shape(shape), m_factor(factor), m_center(center), m_duration(durationMs), m_elapsed(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ScaleAnimation::step);
    int steps = durationMs / 50;
    if (steps == 0) steps = 1;
    m_stepFactor = (factor - 1.0) / steps;
}

ScaleAnimation::~ScaleAnimation() { stop(); }

void ScaleAnimation::start()
{
    m_elapsed = 0;
    m_timer->start(50);
}

void ScaleAnimation::stop() { m_timer->stop(); }

void ScaleAnimation::step()
{
    m_elapsed += 50;
    if (m_elapsed >= m_duration) {
        m_timer->stop();
        double remaining = m_factor - (1.0 + m_stepFactor * (m_elapsed / 50 - 1));
        if (qAbs(remaining) > 1e-6)
            m_shape->scale(1.0 + remaining, m_center);
        emit finished();
    }
    else {
        m_shape->scale(1.0 + m_stepFactor, m_center);
    }
    emit m_shape->shapeChanged();
}