#include "shapeinfo.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <cmath>
#include <QLineF>

ShapeInfoDialog::ShapeInfoDialog(QSharedPointer<Shape> shape, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Shape Information");
    setMinimumSize(400, 300);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_textEdit = new QTextEdit(this);
    m_textEdit->setReadOnly(true);
    m_textEdit->setHtml(getShapeInfo(shape));
    layout->addWidget(m_textEdit);
    
    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}

QString ShapeInfoDialog::getShapeInfo(QSharedPointer<Shape> shape)
{
    if (!shape) return "<h2>No shape selected</h2>";
    
    QString info;
    info += QString("<h2>%1</h2>").arg(shape->typeName());
    info += "<table border='0' cellspacing='5'>";
    
    info += QString("<tr><td><b>Area:</b></td><td>%1</td></tr>").arg(shape->area(), 0, 'f', 2);
    info += QString("<tr><td><b>Perimeter:</b></td><td>%1</td></tr>").arg(shape->perimeter(), 0, 'f', 2);
    
    QPointF com = shape->centerOfMass();
    info += QString("<tr><td><b>Center of Mass:</b></td><td>(%1, %2)</td></tr>")
                .arg(com.x(), 0, 'f', 2).arg(com.y(), 0, 'f', 2);
    
    QString typeName = shape->typeName();
    
    if (typeName == "Circle") {
        auto circlePtr = qSharedPointerCast<Circle>(shape);
        if (circlePtr) {
            info += QString("<tr><td><b>Radius:</b></td><td>%1</td></tr>").arg(circlePtr->radius(), 0, 'f', 2);
            info += QString("<tr><td><b>Diameter:</b></td><td>%1</td></tr>").arg(2 * circlePtr->radius(), 0, 'f', 2);
            info += QString("<tr><td><b>Circumference:</b></td><td>%1</td></tr>").arg(2 * M_PI * circlePtr->radius(), 0, 'f', 2);
        }
    }
    else if (typeName == "Ellipse") {
        auto ellipsePtr = qSharedPointerCast<Ellipse>(shape);
        if (ellipsePtr) {
            info += QString("<tr><td><b>Radius X:</b></td><td>%1</td></tr>").arg(ellipsePtr->radiusX(), 0, 'f', 2);
            info += QString("<tr><td><b>Radius Y:</b></td><td>%1</td></tr>").arg(ellipsePtr->radiusY(), 0, 'f', 2);
            info += QString("<tr><td><b>Rotation:</b></td><td>%1°</td></tr>").arg(ellipsePtr->rotation(), 0, 'f', 2);
            
            double eccentricity = sqrt(1 - pow(std::min(ellipsePtr->radiusX(), ellipsePtr->radiusY()) / 
                                               std::max(ellipsePtr->radiusX(), ellipsePtr->radiusY()), 2));
            info += QString("<tr><td><b>Eccentricity:</b></td><td>%1</td></tr>").arg(eccentricity, 0, 'f', 3);
        }
    }
    else if (typeName == "Rectangle") {
        auto rectPtr = qSharedPointerCast<Rectangle>(shape);
        if (rectPtr) {
            info += QString("<tr><td><b>Width:</b></td><td>%1</td></tr>").arg(rectPtr->width(), 0, 'f', 2);
            info += QString("<tr><td><b>Height:</b></td><td>%1</td></tr>").arg(rectPtr->height(), 0, 'f', 2);
            info += QString("<tr><td><b>Aspect Ratio:</b></td><td>%1</td></tr>").arg(rectPtr->width() / rectPtr->height(), 0, 'f', 2);
        }
    }
    else if (typeName == "Square") {
        auto squarePtr = qSharedPointerCast<Square>(shape);
        if (squarePtr) {
            info += QString("<tr><td><b>Side:</b></td><td>%1</td></tr>").arg(squarePtr->side(), 0, 'f', 2);
            info += QString("<tr><td><b>Diagonal:</b></td><td>%1</td></tr>").arg(squarePtr->side() * sqrt(2), 0, 'f', 2);
        }
    }
    else if (typeName == "Triangle") {
        auto trianglePtr = qSharedPointerCast<Triangle>(shape);
        if (trianglePtr) {
            double a = QLineF(trianglePtr->p1(), trianglePtr->p2()).length();
            double b = QLineF(trianglePtr->p2(), trianglePtr->p3()).length();
            double c = QLineF(trianglePtr->p3(), trianglePtr->p1()).length();
            info += QString("<tr><td><b>Sides:</b></td><td>%1, %2, %3</td></tr>")
                        .arg(a, 0, 'f', 2).arg(b, 0, 'f', 2).arg(c, 0, 'f', 2);
            
            QString triType;
            if (qAbs(a - b) < 1e-6 && qAbs(b - c) < 1e-6)
                triType = "Equilateral";
            else if (qAbs(a - b) < 1e-6 || qAbs(b - c) < 1e-6 || qAbs(c - a) < 1e-6)
                triType = "Isosceles";
            else
                triType = "Scalene";
            
            info += QString("<tr><td><b>Type:</b></td><td>%1</td></tr>").arg(triType);
            
            double angleA = acos((b*b + c*c - a*a) / (2*b*c)) * 180.0 / M_PI;
            double angleB = acos((a*a + c*c - b*b) / (2*a*c)) * 180.0 / M_PI;
            double angleC = 180.0 - angleA - angleB;
            info += QString("<tr><td><b>Angles:</b></td><td>%1°, %2°, %3°</td></tr>")
                        .arg(angleA, 0, 'f', 1).arg(angleB, 0, 'f', 1).arg(angleC, 0, 'f', 1);
        }
    }
    else if (typeName == "Rhombus") {
        auto rhombusPtr = qSharedPointerCast<Rhombus>(shape);
        if (rhombusPtr) {
            info += QString("<tr><td><b>Diagonal 1:</b></td><td>%1</td></tr>").arg(rhombusPtr->diagonal1(), 0, 'f', 2);
            info += QString("<tr><td><b>Diagonal 2:</b></td><td>%1</td></tr>").arg(rhombusPtr->diagonal2(), 0, 'f', 2);
            
            double side = sqrt(pow(rhombusPtr->diagonal1()/2, 2) + pow(rhombusPtr->diagonal2()/2, 2));
            info += QString("<tr><td><b>Side:</b></td><td>%1</td></tr>").arg(side, 0, 'f', 2);
            info += QString("<tr><td><b>Rotation:</b></td><td>%1°</td></tr>").arg(rhombusPtr->rotation(), 0, 'f', 2);
            
            double angle1 = 2 * atan2(rhombusPtr->diagonal2(), rhombusPtr->diagonal1()) * 180.0 / M_PI;
            double angle2 = 180.0 - angle1;
            info += QString("<tr><td><b>Angles:</b></td><td>%1°, %2°</td></tr>")
                        .arg(angle1, 0, 'f', 1).arg(angle2, 0, 'f', 1);
        }
    }
    else if (typeName == "Hexagon") {
        auto hexagonPtr = qSharedPointerCast<Hexagon>(shape);
        if (hexagonPtr) {
            info += QString("<tr><td><b>Radius:</b></td><td>%1</td></tr>").arg(hexagonPtr->radius(), 0, 'f', 2);
            info += QString("<tr><td><b>Side:</b></td><td>%1</td></tr>").arg(hexagonPtr->radius(), 0, 'f', 2);
            info += QString("<tr><td><b>Rotation:</b></td><td>%1°</td></tr>").arg(hexagonPtr->rotation(), 0, 'f', 2);
            
            double apothem = hexagonPtr->radius() * sqrt(3) / 2;
            info += QString("<tr><td><b>Apothem:</b></td><td>%1</td></tr>").arg(apothem, 0, 'f', 2);
        }
    }
    else if (typeName == "Star") {
        auto starPtr = qSharedPointerCast<Star>(shape);
        if (starPtr) {
            info += QString("<tr><td><b>Outer Radius:</b></td><td>%1</td></tr>").arg(starPtr->outerRadius(), 0, 'f', 2);
            info += QString("<tr><td><b>Inner Radius:</b></td><td>%1</td></tr>").arg(starPtr->innerRadius(), 0, 'f', 2);
            info += QString("<tr><td><b>Points:</b></td><td>%1</td></tr>").arg(starPtr->points());
            info += QString("<tr><td><b>Rotation:</b></td><td>%1°</td></tr>").arg(starPtr->rotation(), 0, 'f', 2);
            
            double ratio = starPtr->innerRadius() / starPtr->outerRadius();
            info += QString("<tr><td><b>Radius Ratio:</b></td><td>%1</td></tr>").arg(ratio, 0, 'f', 3);
        }
    }
    else if (typeName == "Polygon") {
        auto polygonPtr = qSharedPointerCast<Polygon>(shape);
        if (polygonPtr) {
            info += QString("<tr><td><b>Vertices:</b></td><td>%1</td></tr>").arg(polygonPtr->vertexCount());
        }
    }
    
    info += "</table>";
    return info;
}