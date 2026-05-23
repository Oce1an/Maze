#pragma once

#include <QMainWindow>

#include "Maze.h"
#include "MazeWidget.h"
#include "solvers/PathSolver.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QPushButton;
class QSlider;
class QSpinBox;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onGenerate();
    void onSolve();
    void onClearPath();
    void onSizeChanged();
    void onSetStart();
    void onSetEnd();
    void onAnimationToggled(bool enabled);
    void onSpeedChanged(int value);
    void onAnimationFinished(const SolveResult& result);
    void onPauseResumeAnimation();
    void onAnimationPaused();

private:
    void applyTheme();
    void updateDebugPanel(const SolveResult* result = nullptr);
    void updateStatus(const QString& text);
    void refreshMazeView();
    void setControlsEnabled(bool enabled);

    Maze m_maze;
    MazeWidget* m_mazeWidget{nullptr};
    QComboBox* m_generatorCombo{nullptr};
    QComboBox* m_solverCombo{nullptr};
    QSpinBox* m_widthSpin{nullptr};
    QSpinBox* m_heightSpin{nullptr};
    QCheckBox* m_animCheck{nullptr};
    QSlider* m_speedSlider{nullptr};
    QLabel* m_statusLabel{nullptr};

    QLabel* m_dbgTime{nullptr};
    QLabel* m_dbgPathLen{nullptr};
    QLabel* m_dbgVisitedPct{nullptr};

    QPushButton* m_solveBtn{nullptr};
    QPushButton* m_stopAnimBtn{nullptr};
    SolveResult m_lastResult;
};
