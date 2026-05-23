#ifndef CAR_H
#define CAR_H

#include "MovingRectangle.h"

class Car : public MovingRectangle
{
public:
	Car(int x = 50, int y = 50);

	void openDoors();
	void closeDoors();
	bool areDoorsOpen() const;

	void turnHeadlightsOn();
    void turnHeadlightsOff();
    bool areHeadlightsOn() const;

	// override = Я НАМЕРЕННО переопределяю виртуальный метод базового класса.
	// тут автоматически ставиться virtual перед void
    void draw(QPainter& painter) const override;

private:
	bool m_doorsOpen;
	bool m_headLightsOn;
};

#endif