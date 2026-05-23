#ifndef MOVINGRECTANGLE_H
#define MOVINGRECTANGLE_H

#include <QPoint>
#include <QPainter>

class MovingRectangle
{
public:
    MovingRectangle(int x = 50, int y = 50, int width = 100, int height = 50);
    virtual ~MovingRectangle() = default;

    void setPosition(int x, int y);

    void moveLeft(int step = 30);
    void moveRight(int step = 30);
    void moveUp(int step = 30);
    void moveDown(int step = 30);

    // Геттеры
    int x() const; // метод QPoint
    int y() const; // метод QPoint
    int width() const;
    int height() const;

    // Виртуальный метод отрисовки
    // virtual = Этот метод МОЖНО переопределить в классах-наследниках (ПОЛИМОРФИЗМ)
    // В методе draw() мы только читаем данные объекта для отрисовки, но не меняем их
    virtual void draw(QPainter& painter) const;

protected:
    QPoint m_position; // Верхний левый угол
    int m_width;
    int m_height;
};

#endif