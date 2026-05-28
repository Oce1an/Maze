#include "MainWindow.h"

#include "generators/MazeGenerator.h"
#include "solvers/PathSolver.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QApplication>

#include <algorithm>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("Maze Pathfinder"));
    resize(1120, 760);
    applyTheme();

    PathSolver::initRegistry();

    auto* central = new QWidget(this);
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(14, 14, 14, 14);
    root->setSpacing(14);

    auto* leftPanel = new QFrame(central);
    leftPanel->setObjectName(QStringLiteral("SidePanel"));
    leftPanel->setFixedWidth(300);
    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(12);

    auto* title = new QLabel(QStringLiteral("Maze Pathfinder"), leftPanel);
    title->setObjectName(QStringLiteral("AppTitle"));
    auto* subtitle = new QLabel(QStringLiteral("Генерация и поиск пути"), leftPanel);
    subtitle->setObjectName(QStringLiteral("AppSubtitle"));
    leftLayout->addWidget(title);
    leftLayout->addWidget(subtitle);

    auto* sizeGroup = new QGroupBox(QStringLiteral("Размер лабиринта"), leftPanel);
    auto* sizeForm = new QFormLayout(sizeGroup);
    m_widthSpin = new QSpinBox(sizeGroup);
    m_heightSpin = new QSpinBox(sizeGroup);
    m_widthSpin->setRange(5, 200);
    m_heightSpin->setRange(5, 200);
    m_widthSpin->setSingleStep(1);
    m_heightSpin->setSingleStep(1);
    m_widthSpin->setValue(m_maze.width());
    m_heightSpin->setValue(m_maze.height());
    sizeForm->addRow(QStringLiteral("Ширина"), m_widthSpin);
    sizeForm->addRow(QStringLiteral("Высота"), m_heightSpin);

    auto* genGroup = new QGroupBox(QStringLiteral("Генерация"), leftPanel);
    auto* genLayout = new QVBoxLayout(genGroup);
    m_generatorCombo = new QComboBox(genGroup);
    for (int i = 0; i < mazeGeneratorCount(); ++i) {
        m_generatorCombo->addItem(mazeGeneratorNameAt(i));
    }
    auto* generateBtn = new QPushButton(QStringLiteral("Сгенерировать"), genGroup);
    generateBtn->setObjectName(QStringLiteral("PrimaryButton"));
    genLayout->addWidget(m_generatorCombo);
    genLayout->addWidget(generateBtn);

    auto* solveGroup = new QGroupBox(QStringLiteral("Поиск пути"), leftPanel);
    auto* solveLayout = new QVBoxLayout(solveGroup);
    m_solverCombo = new QComboBox(solveGroup);
    for (int i = 0; i < PathSolver::count(); ++i) {
        m_solverCombo->addItem(PathSolver::nameAt(i));
    }
    m_solveBtn = new QPushButton(QStringLiteral("Найти путь"), solveGroup);
    m_solveBtn->setObjectName(QStringLiteral("PrimaryButton"));
    auto* clearBtn = new QPushButton(QStringLiteral("Очистить"), solveGroup);
    solveLayout->addWidget(m_solverCombo);
    solveLayout->addWidget(m_solveBtn);
    solveLayout->addWidget(clearBtn);

    auto* animGroup = new QGroupBox(QStringLiteral("Анимация"), leftPanel);
    auto* animLayout = new QVBoxLayout(animGroup);
    m_animCheck = new QCheckBox(QStringLiteral("Показывать анимацию поиска"), animGroup);
    m_animCheck->setChecked(true);
    auto* speedLabel = new QLabel(QStringLiteral("Скорость"), animGroup);
    m_speedSlider = new QSlider(Qt::Horizontal, animGroup);
    m_speedSlider->setRange(1, 100);
    m_speedSlider->setValue(50);
    m_speedSlider->setToolTip(QStringLiteral("Вправо — быстрее анимация"));
    animLayout->addWidget(m_animCheck);
    animLayout->addWidget(speedLabel);
    animLayout->addWidget(m_speedSlider);
    m_stopAnimBtn = new QPushButton(QStringLiteral("Пауза"), animGroup);
    m_stopAnimBtn->setEnabled(false);
    animLayout->addWidget(m_stopAnimBtn);

    auto* pointGroup = new QGroupBox(QStringLiteral("Старт / Финиш"), leftPanel);
    auto* pointLayout = new QHBoxLayout(pointGroup);
    auto* startBtn = new QPushButton(QStringLiteral("Старт"), pointGroup);
    auto* endBtn = new QPushButton(QStringLiteral("Финиш"), pointGroup);
    pointLayout->addWidget(startBtn);
    pointLayout->addWidget(endBtn);

    auto* debugGroup = new QGroupBox(QStringLiteral("Отладка"), leftPanel);
    debugGroup->setObjectName(QStringLiteral("DebugPanel"));
    auto* debugForm = new QFormLayout(debugGroup);
    debugForm->setLabelAlignment(Qt::AlignLeft);
    m_dbgTime = new QLabel(QStringLiteral("—"), debugGroup);
    m_dbgPathLen = new QLabel(QStringLiteral("—"), debugGroup);
    m_dbgVisitedPct = new QLabel(QStringLiteral("—"), debugGroup);
    debugForm->addRow(QStringLiteral("Время"), m_dbgTime);
    debugForm->addRow(QStringLiteral("Длина пути"), m_dbgPathLen);
    debugForm->addRow(QStringLiteral("Посещено"), m_dbgVisitedPct);

    leftLayout->addWidget(sizeGroup);
    leftLayout->addWidget(genGroup);
    leftLayout->addWidget(solveGroup);
    leftLayout->addWidget(animGroup);
    leftLayout->addWidget(pointGroup);
    leftLayout->addWidget(debugGroup, 1);

    m_mazeWidget = new MazeWidget(central);
    m_mazeWidget->setObjectName(QStringLiteral("MazeCanvas"));
    m_mazeWidget->setMaze(&m_maze);
    m_mazeWidget->setAnimationEnabled(true);
    m_mazeWidget->setAnimationSpeed(m_speedSlider->value());

    auto* rightPanel = new QFrame(central);
    rightPanel->setObjectName(QStringLiteral("CanvasPanel"));
    auto* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->addWidget(m_mazeWidget, 1);

    m_statusLabel = new QLabel(QStringLiteral("Готово. Нажмите «Сгенерировать»."), rightPanel);
    m_statusLabel->setObjectName(QStringLiteral("StatusBar"));
    m_statusLabel->setWordWrap(true);
    rightLayout->addWidget(m_statusLabel);

    root->addWidget(leftPanel);
    root->addWidget(rightPanel, 1);

    setCentralWidget(central);

    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerate);
    connect(m_solveBtn, &QPushButton::clicked, this, &MainWindow::onSolve);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearPath);
    connect(m_widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onSizeChanged);
    connect(m_heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onSizeChanged);
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onSetStart);
    connect(endBtn, &QPushButton::clicked, this, &MainWindow::onSetEnd);
    connect(m_animCheck, &QCheckBox::toggled, this, &MainWindow::onAnimationToggled);
    connect(m_speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    connect(m_speedSlider, &QSlider::sliderMoved, this, &MainWindow::onSpeedChanged);
    connect(m_mazeWidget, &MazeWidget::animationFinished, this, &MainWindow::onAnimationFinished);
    connect(m_mazeWidget, &MazeWidget::animationPaused, this, &MainWindow::onAnimationPaused);
    connect(m_stopAnimBtn, &QPushButton::clicked, this, &MainWindow::onPauseResumeAnimation);
    connect(m_mazeWidget, &MazeWidget::startChanged, this, [this](const QPoint& p) {
        updateStatus(QStringLiteral("Старт: (%1, %2)").arg(p.x()).arg(p.y()));
        updateDebugPanel();
    });
    connect(m_mazeWidget, &MazeWidget::endChanged, this, [this](const QPoint& p) {
        updateStatus(QStringLiteral("Финиш: (%1, %2)").arg(p.x()).arg(p.y()));
        updateDebugPanel();
    });

    updateDebugPanel();
    onGenerate();
}

void MainWindow::applyTheme() {
    setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget {
            background-color: #12151c;
            color: #e8ecf4;
            font-family: "Segoe UI", sans-serif;
            font-size: 13px;
        }
        #SidePanel, #CanvasPanel {
            background-color: #1a1f2b;
            border: 1px solid #2d3548;
            border-radius: 12px;
        }
        #AppTitle {
            font-size: 20px;
            font-weight: 700;
            color: #ffffff;
        }
        #AppSubtitle {
            color: #9aa6bf;
            margin-bottom: 6px;
        }
        QGroupBox {
            border: 1px solid #2d3548;
            border-radius: 10px;
            margin-top: 12px;
            padding: 12px 10px 10px 10px;
            font-weight: 600;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 6px;
            color: #b8c4dc;
        }
        #DebugPanel QLabel {
            color: #d0daf0;
            font-family: "Consolas", "Cascadia Mono", monospace;
            font-size: 12px;
        }
        QComboBox, QSpinBox {
            background: #252b3a;
            border: 1px solid #3a455c;
            border-radius: 6px;
            padding: 4px 8px;
            min-height: 26px;
        }
        QPushButton {
            background: #2f3749;
            border: 1px solid #44506a;
            border-radius: 8px;
            padding: 8px 12px;
            font-weight: 600;
        }
        QPushButton:hover { background: #3a455c; }
        QPushButton:pressed { background: #252b3a; }
        QPushButton:disabled { color: #6b768f; background: #222733; }
        #PrimaryButton {
            background: #3d5afe;
            border-color: #536dfe;
            color: white;
        }
        #PrimaryButton:hover { background: #536dfe; }
        QCheckBox { spacing: 8px; }
        QSlider::groove:horizontal {
            height: 6px;
            background: #2d3548;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            width: 14px;
            margin: -5px 0;
            background: #64b5f6;
            border-radius: 7px;
        }
        #StatusBar {
            background: #151a24;
            border-top: 1px solid #2d3548;
            padding: 10px 14px;
            color: #b0bdd6;
        }
        #MazeCanvas {
            background: transparent;
            border: none;
        }
    )"));
}

void MainWindow::onGenerate() {
    m_mazeWidget->resetAnimation();
    setControlsEnabled(true);
    m_stopAnimBtn->setEnabled(false);
    
    if (m_maze.width() > 300 || m_maze.height() > 300) {
        QMessageBox::warning(
            this,
            QStringLiteral("Предупреждение"),
            QStringLiteral("Большой лабиринт может работать медленно. Рекомендуемый размер: до 300x300."));
    }
    
    MazeGenerator* generator = MazeGenerator::byIndex(m_generatorCombo->currentIndex());
    if (!generator) {
        return;
    }
    
    QApplication::setOverrideCursor(Qt::WaitCursor);
    generator->generate(m_maze);
    QApplication::restoreOverrideCursor();
    
    m_maze.clearPath();
    refreshMazeView();
    updateDebugPanel();
    updateStatus(QStringLiteral("Лабиринт создан: %1 (%2×%3)")
                     .arg(generator->name())
                     .arg(m_maze.width())
                     .arg(m_maze.height()));
}

void MainWindow::onSolve() {
    if (m_mazeWidget->isAnimating() || m_mazeWidget->isAnimationPaused()) {
        return;
    }

    PathSolver* solver = PathSolver::byIndex(m_solverCombo->currentIndex());
    if (!solver) {
        return;
    }

    m_maze.clearPath();
    m_maze.clearSearchOverlay();

    SolveOptions opts;
    opts.recordTrace = m_animCheck->isChecked();

    const SolveResult result = solver->solve(m_maze, opts);
    m_lastResult = result;

    if (!result.found) {
        updateDebugPanel(&result);
        refreshMazeView();
        QMessageBox::warning(
            this,
            QStringLiteral("Путь не найден"),
            QStringLiteral("Между стартом и финишем нет прохода."));
        updateStatus(QStringLiteral("Путь не найден (%1).").arg(result.algorithm));
        return;
    }

    if (m_animCheck->isChecked() && !result.trace.empty()) {
        setControlsEnabled(false);
        m_stopAnimBtn->setEnabled(true);
        m_stopAnimBtn->setText(QStringLiteral("Пауза"));
        m_mazeWidget->startAnimation(result);
        updateDebugPanel(&result);
        updateStatus(QStringLiteral("Анимация поиска: %1…").arg(result.algorithm));
    } else {
        m_mazeWidget->applyResultInstant(result);
        onAnimationFinished(result);
    }
}

void MainWindow::onPauseResumeAnimation() {
    if (m_mazeWidget->isAnimationPaused()) {
        m_mazeWidget->resumeAnimation();
        m_stopAnimBtn->setText(QStringLiteral("Пауза"));
        updateStatus(QStringLiteral("Анимация продолжается…"));
        return;
    }
    if (m_mazeWidget->isAnimating()) {
        m_mazeWidget->pauseAnimation();
        m_stopAnimBtn->setText(QStringLiteral("Продолжить"));
        updateStatus(QStringLiteral("Анимация на паузе. Нажмите «Продолжить»."));
    }
}

void MainWindow::onAnimationPaused() {
    m_stopAnimBtn->setText(QStringLiteral("Продолжить"));
    setControlsEnabled(false);
    m_stopAnimBtn->setEnabled(true);
}

void MainWindow::onAnimationFinished(const SolveResult& result) {
    setControlsEnabled(true);
    m_stopAnimBtn->setEnabled(false);
    m_stopAnimBtn->setText(QStringLiteral("Пауза"));
    m_lastResult = result;
    updateDebugPanel(&result);

    const QString shortestNote = result.isShortest
                                     ? QStringLiteral("да")
                                     : QStringLiteral("нет (сравните с BFS/A*)");

    const double ms = result.elapsedMicros / 1000.0;
    m_maze.setSolveInfo(
        QStringLiteral("%1 · %2 шагов · %3 мс · раскрыто %4")
            .arg(result.algorithm)
            .arg(result.pathLength)
            .arg(ms, 0, 'f', 2)
            .arg(result.nodesExpanded));

    updateStatus(m_maze.lastSolveInfo() + QStringLiteral(" · кратчайший: ") + shortestNote);
    refreshMazeView();
}

void MainWindow::onClearPath() {
    m_mazeWidget->resetAnimation();
    setControlsEnabled(true);
    m_stopAnimBtn->setEnabled(false);
    m_maze.clearPath();
    m_mazeWidget->setPlacementMode(MazeWidget::PlacementMode::None);
    updateDebugPanel();
    refreshMazeView();
    updateStatus(QStringLiteral("Путь и подсветка поиска очищены."));
}

void MainWindow::onSizeChanged() {
    m_mazeWidget->resetAnimation();
    setControlsEnabled(true);
    m_stopAnimBtn->setEnabled(false);
    m_maze.resize(m_widthSpin->value(), m_heightSpin->value());
    m_mazeWidget->setPlacementMode(MazeWidget::PlacementMode::None);
    onGenerate();
}

void MainWindow::onSetStart() {
    if (m_mazeWidget->isAnimating() || m_mazeWidget->isAnimationPaused()) {
        return;
    }
    m_mazeWidget->setPlacementMode(MazeWidget::PlacementMode::Start);
    updateStatus(QStringLiteral("Кликните по клетке для старта."));
}

void MainWindow::onSetEnd() {
    if (m_mazeWidget->isAnimating() || m_mazeWidget->isAnimationPaused()) {
        return;
    }
    m_mazeWidget->setPlacementMode(MazeWidget::PlacementMode::End);
    updateStatus(QStringLiteral("Кликните по клетке для финиша."));
}

void MainWindow::onAnimationToggled(bool enabled) {
    m_mazeWidget->setAnimationEnabled(enabled);
}

void MainWindow::onSpeedChanged(int value) {
    m_mazeWidget->setAnimationSpeed(value);
}

void MainWindow::updateDebugPanel(const SolveResult* result) {
    if (!result) {
        m_dbgTime->setText(QStringLiteral("—"));
        m_dbgPathLen->setText(QStringLiteral("—"));
        m_dbgVisitedPct->setText(QStringLiteral("—"));
        return;
    }

    const int total = m_maze.cellCount();
    const double visitedPct =
        total > 0 ? (100.0 * result->nodesExpanded / total) : 0.0;

    m_dbgTime->setText(QStringLiteral("%1 ms").arg(result->elapsedMicros / 1000.0, 0, 'f', 2));
    m_dbgPathLen->setText(result->found ? QString::number(result->pathLength)
                                        : QStringLiteral("—"));
    m_dbgVisitedPct->setText(QStringLiteral("%1% (%2 / %3)")
                                  .arg(visitedPct, 0, 'f', 1)
                                  .arg(result->nodesExpanded)
                                  .arg(total));
}

void MainWindow::updateStatus(const QString& text) {
    m_statusLabel->setText(text);
}

void MainWindow::refreshMazeView() {
    m_mazeWidget->update();
}

void MainWindow::setControlsEnabled(bool enabled) {
    m_solveBtn->setEnabled(enabled);
    m_generatorCombo->setEnabled(enabled);
    m_widthSpin->setEnabled(enabled);
    m_heightSpin->setEnabled(enabled);
}
