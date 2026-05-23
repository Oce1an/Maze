#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include "Car.h"

class DrawingWidget : public QWidget
{
	Q_OBJECT

public:

	explicit DrawingWidget(QWidget *parent = nullptr);

	Car* car();// Доступ к Car

protected:

	void paintEvent(QPaintEvent *event) override;

private:

	Car m_car;

};

#endif