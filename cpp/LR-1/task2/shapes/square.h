#ifndef SQUARE_H
#define SQUARE_H

#include "rectangle.h"

class Square : public Rectangle
{
    Q_OBJECT
public:
    explicit Square(QObject* parent = nullptr);
    Square(const QPointF& topLeft, double side, QObject* parent = nullptr);
    static QSharedPointer<Square> fromCenter(const QPointF& center, double side, QObject* parent = nullptr);
    virtual ~Square();

    QString typeName() const override;

    void setSide(double side);
    double side() const;
    
    QPointF center() const;
    void setCenter(const QPointF& center);
};

#endif