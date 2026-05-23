#include "mainwindow.h"
#include "canvas.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QDoubleValidator>
#include <QMessageBox>
#include "shapes/circle.h"
#include "shapes/triangle.h"
#include "shapes/rectangle.h"
#include "shapes/square.h"
#include "shapes/rhombus.h"
#include "shapes/star.h"
#include "shapes/hexagon.h"
#include "shapes/ellipse.h"
#include "animations/rotateanimation.h"
#include "animations/moveanimation.h"
#include "animations/scaleanimation.h"
#include "shapeinfo.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_shapeList(nullptr)
    , m_canvas(nullptr)
    , m_moveDx(nullptr)
    , m_moveDy(nullptr)
    , m_rotateAngle(nullptr)
    , m_scaleFactor(nullptr)
    , m_durationSlider(nullptr)
    , m_durationLabel(nullptr)
{
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    m_shapeList = new QListWidget(this);
    m_shapeList->setMaximumWidth(200);
    leftLayout->addWidget(new QLabel("Shapes:"));
    leftLayout->addWidget(m_shapeList);

    QPushButton* deleteBtn = new QPushButton("Delete", this);
    leftLayout->addWidget(deleteBtn);

    QPushButton* clearBtn = new QPushButton("Clear All", this);
    leftLayout->addWidget(clearBtn);
    
    QPushButton* infoBtn = new QPushButton("Show Info", this);
    leftLayout->addWidget(infoBtn);

    leftLayout->addStretch();
    mainLayout->addLayout(leftLayout);

    QVBoxLayout* rightLayout = new QVBoxLayout();

    m_canvas = new Canvas(this);
    m_canvas->setMinimumSize(600, 400);
    m_canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightLayout->addWidget(m_canvas, 1);

    QGroupBox* createGroup = new QGroupBox("Create Shape", this);
    QHBoxLayout* createLayout = new QHBoxLayout(createGroup);

    QPushButton* btnCircle = new QPushButton("Circle", this);
    QPushButton* btnTriangle = new QPushButton("Triangle", this);
    QPushButton* btnRectangle = new QPushButton("Rectangle", this);
    QPushButton* btnSquare = new QPushButton("Square", this);
    QPushButton* btnRhombus = new QPushButton("Rhombus", this);
    QPushButton* btnStar = new QPushButton("Star", this);
    QPushButton* btnHexagon = new QPushButton("Hexagon", this);
    QPushButton* btnEllipse = new QPushButton("Ellipse", this);

    createLayout->addWidget(btnCircle);
    createLayout->addWidget(btnTriangle);
    createLayout->addWidget(btnRectangle);
    createLayout->addWidget(btnSquare);
    createLayout->addWidget(btnRhombus);
    createLayout->addWidget(btnStar);
    createLayout->addWidget(btnHexagon);
    createLayout->addWidget(btnEllipse);
    rightLayout->addWidget(createGroup);

    QGroupBox* controlGroup = new QGroupBox("Shape Control", this);
    QFormLayout* controlLayout = new QFormLayout(controlGroup);

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    doubleValidator->setLocale(QLocale::C);

    m_moveDx = new QLineEdit(this);
    m_moveDy = new QLineEdit(this);
    m_moveDx->setValidator(doubleValidator);
    m_moveDy->setValidator(doubleValidator);
    m_moveDx->setText("0");
    m_moveDy->setText("0");
    QPushButton* btnMove = new QPushButton("Move", this);

    m_rotateAngle = new QLineEdit(this);
    m_rotateAngle->setValidator(doubleValidator);
    m_rotateAngle->setText("0");
    QPushButton* btnRotate = new QPushButton("Rotate", this);

    m_scaleFactor = new QLineEdit(this);
    m_scaleFactor->setValidator(doubleValidator);
    m_scaleFactor->setText("1.0");
    QPushButton* btnScale = new QPushButton("Scale", this);

    m_durationSlider = new QSlider(Qt::Horizontal, this);
    m_durationSlider->setRange(100, 5000);
    m_durationSlider->setValue(1000);
    m_durationSlider->setTickInterval(100);
    m_durationSlider->setTickPosition(QSlider::TicksBelow);

    m_durationLabel = new QLabel("1000 ms", this);

    QHBoxLayout* durationLayout = new QHBoxLayout();
    durationLayout->addWidget(m_durationSlider);
    durationLayout->addWidget(m_durationLabel);

    controlLayout->addRow("dx:", m_moveDx);
    controlLayout->addRow("dy:", m_moveDy);
    controlLayout->addRow("", btnMove);
    controlLayout->addRow("Angle:", m_rotateAngle);
    controlLayout->addRow("", btnRotate);
    controlLayout->addRow("Scale:", m_scaleFactor);
    controlLayout->addRow("", btnScale);
    controlLayout->addRow("Duration:", durationLayout);

    rightLayout->addWidget(controlGroup);
    mainLayout->addLayout(rightLayout);

    m_buttons["circle"] = btnCircle;
    m_buttons["triangle"] = btnTriangle;
    m_buttons["rectangle"] = btnRectangle;
    m_buttons["square"] = btnSquare;
    m_buttons["rhombus"] = btnRhombus;
    m_buttons["star"] = btnStar;
    m_buttons["hexagon"] = btnHexagon;
    m_buttons["ellipse"] = btnEllipse;
    m_buttons["move"] = btnMove;
    m_buttons["rotate"] = btnRotate;
    m_buttons["scale"] = btnScale;
    m_buttons["delete"] = deleteBtn;
    m_buttons["clear"] = clearBtn;
    m_buttons["info"] = infoBtn;
}

void MainWindow::setupConnections()
{
    connect(m_buttons["circle"], &QPushButton::clicked, this, &MainWindow::onAddCircle);
    connect(m_buttons["triangle"], &QPushButton::clicked, this, &MainWindow::onAddTriangle);
    connect(m_buttons["rectangle"], &QPushButton::clicked, this, &MainWindow::onAddRectangle);
    connect(m_buttons["square"], &QPushButton::clicked, this, &MainWindow::onAddSquare);
    connect(m_buttons["rhombus"], &QPushButton::clicked, this, &MainWindow::onAddRhombus);
    connect(m_buttons["star"], &QPushButton::clicked, this, &MainWindow::onAddStar);
    connect(m_buttons["hexagon"], &QPushButton::clicked, this, &MainWindow::onAddHexagon);
    connect(m_buttons["ellipse"], &QPushButton::clicked, this, &MainWindow::onAddEllipse);

    connect(m_buttons["move"], &QPushButton::clicked, this, &MainWindow::onMoveShape);
    connect(m_buttons["rotate"], &QPushButton::clicked, this, &MainWindow::onRotateShape);
    connect(m_buttons["scale"], &QPushButton::clicked, this, &MainWindow::onScaleShape);
    connect(m_buttons["delete"], &QPushButton::clicked, this, &MainWindow::onDeleteShape);
    connect(m_buttons["clear"], &QPushButton::clicked, this, &MainWindow::onClearAll);

    connect(m_shapeList, &QListWidget::currentRowChanged, this, &MainWindow::onShapeSelected);
    connect(m_durationSlider, &QSlider::valueChanged, this, &MainWindow::onDurationChanged);

    if (m_canvas) {
        connect(m_canvas, &Canvas::selectionChanged, this, &MainWindow::onCanvasSelectionChanged);
    }

    connect(m_buttons["info"], &QPushButton::clicked, this, &MainWindow::onShowShapeInfo);
}

template<typename T, typename... Args>
void MainWindow::createShape(Args&&... args)
{
    if (!m_canvas) {
        showWarning("Canvas is not initialized");
        return;
    }

    try {
        auto shape = QSharedPointer<T>::create(std::forward<Args>(args)...);
        connect(shape.data(), &Shape::shapeChanged, m_canvas, [this]() {
            if (m_canvas) m_canvas->update();
            });
        m_canvas->addShape(shape);
        updateShapeList();
    }
    catch (const std::exception& e) {
        showWarning(QString("Failed to create shape: %1").arg(e.what()));
    }
}

void MainWindow::onAddCircle()
{
    createShape<Circle>(QPointF(200, 200), 50);
}

void MainWindow::onAddTriangle()
{
    createShape<Triangle>(QPointF(100, 100), QPointF(200, 100), QPointF(150, 200));
}

void MainWindow::onAddRectangle()
{
    createShape<Rectangle>(QPointF(150, 150), 120, 80);
}

void MainWindow::onAddSquare()
{
    createShape<Square>(QPointF(200, 200), 100);
}

void MainWindow::onAddRhombus()
{
    createShape<Rhombus>(QPointF(300, 300), 150, 80, 30);
}

void MainWindow::onAddStar()
{
    createShape<Star>(QPointF(400, 200), 80, 40, 5, 0);
}

void MainWindow::onAddHexagon()
{
    createShape<Hexagon>(QPointF(500, 300), 70, 15);
}

void MainWindow::onAddEllipse()
{
    createShape<Ellipse>(QPointF(350, 400), 100, 50, 45);
}

void MainWindow::onShapeSelected(int row)
{
    if (m_canvas) {
        if (row >= 0 && row < m_canvas->shapeCount()) {
            m_canvas->setSelectedIndex(row);
        }
        else {
            m_canvas->setSelectedIndex(-1);
        }
    }
}

void MainWindow::onCanvasSelectionChanged(int index)
{
    if (!m_shapeList) return;

    if (index >= 0 && index < m_shapeList->count()) {
        m_shapeList->blockSignals(true);
        m_shapeList->setCurrentRow(index);
        m_shapeList->blockSignals(false);
    }
    else {
        m_shapeList->blockSignals(true);
        m_shapeList->clearSelection();
        m_shapeList->blockSignals(false);
    }
}

void MainWindow::onMoveShape()
{
    if (!validateCanvasAndShape()) return;

    QSharedPointer<Shape> shape = m_canvas->selectedShape();
    if (!shape) return;

    bool ok1, ok2;
    QLocale locale(QLocale::C);
    double dx = locale.toDouble(m_moveDx->text(), &ok1);
    double dy = locale.toDouble(m_moveDy->text(), &ok2);

    if (!ok1 || !ok2) {
        showWarning("Enter valid numbers for dx and dy");
        return;
    }

    auto* anim = new MoveAnimation(shape.data(), dx, dy, m_durationSlider->value(), this);
    m_activeAnimations.insert(anim);
    connect(anim, &MoveAnimation::finished, this, [this, anim]() {
        m_activeAnimations.remove(anim);
        anim->deleteLater();
    });
    anim->start();
}
void MainWindow::onRotateShape()
{
    if (!validateCanvasAndShape()) return;

    QSharedPointer<Shape> shape = m_canvas->selectedShape();
    if (!shape) return;

    bool ok;
    QLocale locale(QLocale::C);
    double angle = locale.toDouble(m_rotateAngle->text(), &ok);

    if (!ok) {
        showWarning("Enter a valid angle");
        return;
    }

    auto* anim = new RotateAnimation(shape.data(), angle, shape->centerOfMass(),
        m_durationSlider->value(), this);
    m_activeAnimations.insert(anim);
    connect(anim, &RotateAnimation::finished, this, [this, anim]() {
        m_activeAnimations.remove(anim);
        anim->deleteLater();
    });
    anim->start();
}

void MainWindow::onScaleShape()
{
    if (!validateCanvasAndShape()) return;

    QSharedPointer<Shape> shape = m_canvas->selectedShape();
    if (!shape) return;

    bool ok;
    QLocale locale(QLocale::C);
    double factor = locale.toDouble(m_scaleFactor->text(), &ok);

    if (!ok || factor <= 0) {
        showWarning("Enter a positive scale factor (e.g., 0.5 or 2.0)");
        return;
    }

    auto* anim = new ScaleAnimation(shape.data(), factor, shape->centerOfMass(),
        m_durationSlider->value(), this);
    m_activeAnimations.insert(anim);
    connect(anim, &ScaleAnimation::finished, this, [this, anim]() {
        m_activeAnimations.remove(anim);
        anim->deleteLater();
    });
    anim->start();
}

void MainWindow::onDeleteShape()
{
    if (!m_canvas) return;

    int row = m_canvas->selectedIndex();
    if (row >= 0) {
        QSharedPointer<Shape> shapeToDelete = m_canvas->shapeAt(row);
        if (shapeToDelete) {
            QList<QObject*> toRemove;
            for (QObject* anim : m_activeAnimations) {
                bool shouldStop = false;
                if (auto* moveAnim = qobject_cast<MoveAnimation*>(anim)) {
                    shouldStop = (moveAnim->parent() == this);
                }
                if (shouldStop) {
                    if (auto* moveAnim = qobject_cast<MoveAnimation*>(anim)) {
                        moveAnim->stop();
                    }
                    toRemove.append(anim);
                }
            }
            for (QObject* anim : toRemove) {
                m_activeAnimations.remove(anim);
            }
        }
        
        m_canvas->removeShape(row);
        updateShapeList();

        if (m_canvas->shapeCount() == 0) {
            m_shapeList->clearSelection();
        }
        else {
            int newIndex = qMin(row, m_canvas->shapeCount() - 1);
            m_shapeList->setCurrentRow(newIndex);
        }
    }
}

void MainWindow::onClearAll()
{
    for (QObject* anim : m_activeAnimations) {
        if (auto* moveAnim = qobject_cast<MoveAnimation*>(anim)) {
            moveAnim->stop();
        } else if (auto* rotateAnim = qobject_cast<RotateAnimation*>(anim)) {
            rotateAnim->stop();
        } else if (auto* scaleAnim = qobject_cast<ScaleAnimation*>(anim)) {
            scaleAnim->stop();
        }
    }
    m_activeAnimations.clear();
    
    if (m_canvas) {
        m_canvas->clear();
        updateShapeList();
        m_shapeList->clearSelection();
    }
}

void MainWindow::onDurationChanged(int value)
{
    if (m_durationLabel) {
        m_durationLabel->setText(QString("%1 ms").arg(value));
    }
}

void MainWindow::updateShapeList()
{
    if (!m_shapeList || !m_canvas) return;

    m_shapeList->clear();
    for (int i = 0; i < m_canvas->shapeCount(); ++i) {
        auto shape = m_canvas->shapeAt(i);
        if (shape) {
            m_shapeList->addItem(QString("%1 %2").arg(shape->typeName()).arg(i + 1));
        }
    }

    if (m_canvas->shapeCount() == 0) {
        m_canvas->setSelectedIndex(-1);
    }
}

void MainWindow::showWarning(const QString& message)
{
    QMessageBox::warning(this, "Error", message);
}

bool MainWindow::validateCanvasAndShape() const
{
    if (!m_canvas) {
        const_cast<MainWindow*>(this)->showWarning("Canvas is not initialized");
        return false;
    }

    if (m_canvas->shapeCount() == 0) {
        const_cast<MainWindow*>(this)->showWarning("No shapes available");
        return false;
    }

    if (!m_canvas->selectedShape()) {
        const_cast<MainWindow*>(this)->showWarning("Select a shape from the list");
        return false;
    }

    return true;
}
void MainWindow::onShowShapeInfo()
{
    if (!validateCanvasAndShape()) return;
    
    QSharedPointer<Shape> shape = m_canvas->selectedShape();
    if (!shape) return;
    
    ShapeInfoDialog dialog(shape, this);
    dialog.exec();
}