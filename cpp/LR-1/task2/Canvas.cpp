#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <cmath>

Canvas::Canvas(QWidget* parent)
    : QWidget(parent)
    , m_selectedIndex(-1)
    , m_dragging(false)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

void Canvas::addShape(QSharedPointer<Shape> shape)
{
    if (shape) {
        m_shapes.append(shape);
        connect(shape.data(), &Shape::shapeChanged, this, [this]() {
            update();
        });
        update();
    }
}

void Canvas::removeShape(int index)
{
    if (index >= 0 && index < m_shapes.size()) {
        if (m_shapes[index]) {
            m_shapes[index]->disconnect(this);
        }

        m_shapes.removeAt(index);

        if (m_shapes.isEmpty()) {
            m_selectedIndex = -1;
        }
        else if (m_selectedIndex >= m_shapes.size()) {
            m_selectedIndex = m_shapes.size() - 1;
        }

        emit selectionChanged(m_selectedIndex);
        update();
    }
}

void Canvas::clear()
{
    for (const auto& shape : m_shapes) {
        if (shape) {
            shape->disconnect(this);
        }
    }

    m_shapes.clear();
    m_selectedIndex = -1;
    m_dragging = false;

    emit selectionChanged(m_selectedIndex);
    update();
}

QSharedPointer<Shape> Canvas::shapeAt(int index) const
{
    if (index >= 0 && index < m_shapes.size()) {
        return m_shapes[index];
    }
    return QSharedPointer<Shape>();
}

int Canvas::shapeCount() const
{
    return m_shapes.size();
}

QSharedPointer<Shape> Canvas::selectedShape() const
{
    if (m_selectedIndex >= 0 && m_selectedIndex < m_shapes.size()) {
        return m_shapes[m_selectedIndex];
    }
    return QSharedPointer<Shape>();
}

void Canvas::setSelectedIndex(int index)
{
    int oldIndex = m_selectedIndex;

    if (m_shapes.isEmpty()) {
        m_selectedIndex = -1;
    }
    else if (index >= -1 && index < m_shapes.size()) {
        m_selectedIndex = index;
    }

    if (oldIndex != m_selectedIndex) {
        emit selectionChanged(m_selectedIndex);
        update();
    }
}

void Canvas::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::lightGray);
    for (int x = 0; x < width(); x += 20) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += 20) {
        painter.drawLine(0, y, width(), y);
    }

    for (int i = 0; i < m_shapes.size(); ++i) {
        if (m_shapes[i]) {
            if (i == m_selectedIndex) {
                painter.setPen(QPen(Qt::red, 2));
                painter.setBrush(QColor(255, 200, 200, 180));
            }
            else {
                painter.setPen(Qt::black);
                QColor fillColor(200, 200, 255);
                switch (i % 5) {
                case 0: fillColor = QColor(200, 255, 200, 180); break;
                case 1: fillColor = QColor(255, 255, 200, 180); break;
                case 2: fillColor = QColor(255, 200, 255, 180); break;
                case 3: fillColor = QColor(200, 255, 255, 180); break;
                case 4: fillColor = QColor(255, 220, 180, 180); break;
                }
                painter.setBrush(fillColor);
            }
            m_shapes[i]->draw(painter);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        for (int i = m_shapes.size() - 1; i >= 0; --i) {
            if (m_shapes[i] && m_shapes[i]->contains(event->pos())) {
                setSelectedIndex(i);
                m_dragging = true;
                m_lastMousePos = event->pos();
                break;
            }
        }
        
        if (m_selectedIndex != -1 && 
            (!m_shapes[m_selectedIndex] || !m_shapes[m_selectedIndex]->contains(event->pos()))) {
            setSelectedIndex(-1);
            m_dragging = false;
        }
    }
    
    QWidget::mousePressEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging && m_selectedIndex >= 0 && m_selectedIndex < m_shapes.size()) {
        QSharedPointer<Shape> shape = m_shapes[m_selectedIndex];
        if (shape) {
            QPointF delta = event->pos() - m_lastMousePos;
            shape->move(delta.x(), delta.y());
            m_lastMousePos = event->pos();
            update();
        }
    }
    
    QWidget::mouseMoveEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
    }
    
    QWidget::mouseReleaseEvent(event);
}