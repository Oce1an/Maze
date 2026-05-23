#include "../hdr/DrawingWidget.h"
#include <QPainter>

DrawingWidget::DrawingWidget(QWidget *parent) : QWidget(parent)
{
	// Размер окна
	setFixedSize(800, 600);
	// Цвет фона
	setAutoFillBackground(true);
	setPalette(QPalette(Qt::lightGray));
}

Car* DrawingWidget::car()
{
	return &m_car;
}

void DrawingWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);

	m_car.draw(painter);
}