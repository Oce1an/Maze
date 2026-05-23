#ifndef SHAPEINFODIALOG_H
#define SHAPEINFODIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include "shapes/shape.h"
#include "shapes/circle.h"
#include "shapes/ellipse.h"
#include "shapes/rectangle.h"
#include "shapes/square.h"
#include "shapes/triangle.h"
#include "shapes/rhombus.h"
#include "shapes/hexagon.h"
#include "shapes/star.h"
#include "shapes/polygon.h"

class ShapeInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShapeInfoDialog(QSharedPointer<Shape> shape, QWidget* parent = nullptr);
    
private:
    QString getShapeInfo(QSharedPointer<Shape> shape);
    QTextEdit* m_textEdit;
};

#endif