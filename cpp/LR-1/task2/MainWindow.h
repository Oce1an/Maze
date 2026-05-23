#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMap>
#include <QSharedPointer>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QDoubleValidator>

class Canvas;
class Shape;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddCircle();
    void onAddTriangle();
    void onAddRectangle();
    void onAddSquare();
    void onAddRhombus();
    void onAddStar();
    void onAddHexagon();
    void onAddEllipse();

    void onShowShapeInfo();
    void onShapeSelected(int row);
    void onCanvasSelectionChanged(int index);

    void onMoveShape();
    void onRotateShape();
    void onScaleShape();
    void onDeleteShape();
    void onClearAll();

    void onDurationChanged(int value);

private:
    QPushButton* m_infoButton;
    QSet<QObject*> m_activeAnimations;
    void setupUI();
    void setupConnections();
    void updateShapeList();
    void showWarning(const QString& message);
    bool validateCanvasAndShape() const;

    template<typename T, typename... Args>
    void createShape(Args&&... args);

private:
    QListWidget* m_shapeList;
    Canvas* m_canvas;
    QLineEdit* m_moveDx;
    QLineEdit* m_moveDy;
    QLineEdit* m_rotateAngle;
    QLineEdit* m_scaleFactor;
    QSlider* m_durationSlider;
    QLabel* m_durationLabel;

    QMap<QString, QPushButton*> m_buttons;
};

#endif