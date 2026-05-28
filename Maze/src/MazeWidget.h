#pragma once

#include "Maze.h"
#include "solvers/PathSolver.h"

#include <QTimer>
#include <QWidget>

class Maze;

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    enum class PlacementMode { None, Start, End };

    explicit MazeWidget(QWidget* parent = nullptr);

    void setMaze(Maze* maze);
    void setPlacementMode(PlacementMode mode);
    void setAnimationEnabled(bool enabled);
    void setAnimationSpeed(int speedLevel);
    bool isAnimating() const { return m_animRunning; }
    bool isAnimationPaused() const { return m_animPaused; }
    bool hasActiveAnimation() const { return m_hasActiveAnimation; }
    void resetAnimation();
    void pauseAnimation();
    void resumeAnimation();
    void startAnimation(const SolveResult& result);
    void applyResultInstant(const SolveResult& result);
    void resetPathAndAnimation();

signals:
    void startChanged(const QPoint& p);
    void endChanged(const QPoint& p);
    void animationFinished(const SolveResult& result);
    void animationPaused();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onAnimTick();

private:
    struct LayoutMetrics {
        int cellSize{0};
        int offsetX{0};
        int offsetY{0};
        QRect mazeRect;
    };

    LayoutMetrics layoutMetrics() const;
    QPoint roomAt(const QPoint& pos, const LayoutMetrics& lm) const;
    void drawMaze(QPainter& painter, const LayoutMetrics& lm);
    void advanceAnimationFrame();
    void applySearchEntry(const SearchTraceEntry& entry);
    int stepsPerTick() const;
    int pathVisualLength() const;

    Maze* m_maze{nullptr};
    PlacementMode m_placementMode{PlacementMode::None};

    bool m_animationEnabled{true};
    int m_speedLevel{50};
    static constexpr int kAnimTimerMs = 30;
    bool m_animRunning{false};
    bool m_animPaused{false};
    bool m_hasActiveAnimation{false};
    int m_traceIndex{0};
    int m_pathRevealIndex{0};
    bool m_pathPhase{false};
    SolveResult m_pendingResult;

    QTimer m_animTimer;
};
