#include "moveanimation.h"

MoveAnimation::MoveAnimation(Shape* shape, double dx, double dy, int durationMs, QObject* parent)
    : QObject(parent), m_shape(shape), m_dx(dx), m_dy(dy), m_duration(durationMs), m_elapsed(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MoveAnimation::step);
    int steps = durationMs / 50;
    if (steps == 0) steps = 1;
    m_stepDx = dx / steps;
    m_stepDy = dy / steps;
}

MoveAnimation::~MoveAnimation() { stop(); }

void MoveAnimation::start()
{
    m_elapsed = 0;
    m_timer->start(50);
}

void MoveAnimation::stop() { m_timer->stop(); }

void MoveAnimation::step()
{
    m_elapsed += 50;
    if (m_elapsed >= m_duration) {
        m_timer->stop();
        double remDx = m_dx - m_stepDx * (m_elapsed / 50 - 1);
        double remDy = m_dy - m_stepDy * (m_elapsed / 50 - 1);
        if (qAbs(remDx) > 1e-6 || qAbs(remDy) > 1e-6)
            m_shape->move(remDx, remDy);
        emit finished();
    }
    else {
        m_shape->move(m_stepDx, m_stepDy);
    }
    emit m_shape->shapeChanged();
}