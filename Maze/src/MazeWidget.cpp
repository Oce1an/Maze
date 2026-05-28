#include "MazeWidget.h"

#include "Maze.h"

#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QRadialGradient>
#include <algorithm>
#include <optional>

namespace {

struct OverlaySample {
    SearchCellState state{SearchCellState::None};
    int depth{0};
};

QColor passageColor(int gx, int gy, bool isRoom) {
    if (!isRoom) {
        return QColor(52, 58, 72);
    }
    const bool checker = ((gx / 2) + (gy / 2)) % 2 == 0;
    return checker ? QColor(46, 52, 68) : QColor(40, 45, 58);
}

QColor wallColor() {
    return QColor(18, 20, 28);
}

int statePriority(SearchCellState state) {
    switch (state) {
    case SearchCellState::Path:
        return 3;
    case SearchCellState::Expanded:
        return 2;
    case SearchCellState::Frontier:
        return 1;
    default:
        return 0;
    }
}

QColor frontierByDepth(int depth, int maxDepth) {
    const double t = maxDepth > 0 ? std::clamp(depth / static_cast<double>(maxDepth), 0.0, 1.0) : 0.0;
    return QColor::fromHsvF(
        static_cast<float>(0.60 - t * 0.15),
        0.50f + static_cast<float>(t) * 0.20f,
        0.45f + static_cast<float>(t) * 0.35f,
        0.92f
    );
}

QColor expandedByDepth(int depth, int maxDepth) {
    const double t = maxDepth > 0 ? std::clamp(depth / static_cast<double>(maxDepth), 0.0, 1.0) : 0.0;
    return QColor::fromHsvF(
        static_cast<float>(0.48 - t * 0.1),
        0.50f,
        0.50f + static_cast<float>(t) * 0.45f,
        0.88f
    );
}

QColor pathColor(int step, int total) {
    const double t = total > 1 ? std::clamp(step / static_cast<double>(total - 1), 0.0, 1.0) : 0.0;
    const float hue = static_cast<float>(0.42 - t * 0.34);
    const float sat = 0.72f + static_cast<float>(t) * 0.2f;
    const float val = 0.88f + static_cast<float>(t) * 0.1f;
    return QColor::fromHsvF(hue, sat, val, 1.0f);
}

QColor colorForOverlay(SearchCellState state, int depth, int maxDepth, int pathLength) {
    switch (state) {
    case SearchCellState::Frontier:
        return frontierByDepth(depth, maxDepth);
    case SearchCellState::Expanded:
        return expandedByDepth(depth, maxDepth);
    case SearchCellState::Path:
        return pathColor(depth, std::max(1, pathLength));
    default:
        return QColor(0, 0, 0, 0);
    }
}

OverlaySample mergeOverlay(OverlaySample a, OverlaySample b) {
    if (statePriority(a.state) < statePriority(b.state)) {
        return b;
    }
    if (statePriority(b.state) < statePriority(a.state)) {
        return a;
    }
    return {a.state, std::max(a.depth, b.depth)};
}

std::optional<OverlaySample> overlayAtRoom(const Maze& maze, int roomIdx) {
    const SearchCellState st = maze.searchState(roomIdx);
    if (st == SearchCellState::None) {
        return std::nullopt;
    }
    return OverlaySample{st, maze.searchDistance(roomIdx)};
}

std::optional<OverlaySample> overlayAtCorridor(const Maze& maze, int gx, int gy) {
    if (!maze.isPassage(gx, gy) || maze.isRoomCell(gx, gy)) {
        return std::nullopt;
    }

    std::optional<OverlaySample> merged;

    const auto addRoom = [&](int rx, int ry) {
        if (!maze.inBounds(rx, ry)) {
            return;
        }
        if (auto sample = overlayAtRoom(maze, maze.index(rx, ry))) {
            merged = merged ? mergeOverlay(*merged, *sample) : *sample;
        }
    };

    if (gy % 2 == 1 && gx % 2 == 0) {
        const int ry = (gy - 1) / 2;
        const int rxRight = gx / 2;
        addRoom(rxRight - 1, ry);
        addRoom(rxRight, ry);
    } else if (gx % 2 == 1 && gy % 2 == 0) {
        const int rx = (gx - 1) / 2;
        const int ryBottom = gy / 2;
        addRoom(rx, ryBottom - 1);
        addRoom(rx, ryBottom);
    }

    return merged;
}

}

MazeWidget::MazeWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(480, 360);
    setAttribute(Qt::WA_OpaquePaintEvent);
    m_animTimer.setInterval(kAnimTimerMs);
    m_animTimer.setTimerType(Qt::PreciseTimer);
    connect(&m_animTimer, &QTimer::timeout, this, &MazeWidget::onAnimTick);
}

void MazeWidget::setMaze(Maze* maze) {
    m_maze = maze;
    resetPathAndAnimation();
    update();
}

void MazeWidget::setPlacementMode(PlacementMode mode) {
    m_placementMode = mode;
    setCursor(mode == PlacementMode::None ? Qt::ArrowCursor : Qt::CrossCursor);
}

void MazeWidget::setAnimationEnabled(bool enabled) {
    m_animationEnabled = enabled;
}

void MazeWidget::setAnimationSpeed(int speedLevel) {
    m_speedLevel = std::clamp(speedLevel, 1, 100);
}

int MazeWidget::stepsPerTick() const {
    if (m_pathPhase) {
        const int pathLen = static_cast<int>(m_pendingResult.path.size());
        if (pathLen <= 0) {
            return 1;
        }
        
        const double norm = (m_speedLevel - 1) / 99.0;
        const int baseSteps = std::max(3, pathLen / 30);
        const int maxBatch = std::max(baseSteps, pathLen / 16);
        
        return std::max(baseSteps, baseSteps + static_cast<int>(norm * norm * (maxBatch - baseSteps)));
    } else {
        const int total = static_cast<int>(m_pendingResult.trace.size());
        if (total <= 0) {
            return 1;
        }

        const double norm = (m_speedLevel - 1) / 99.0;
        const int maxBatch = std::max(1, total / 25);
        return std::max(1, 1 + static_cast<int>(norm * norm * (maxBatch - 1)));
    }
}

int MazeWidget::pathVisualLength() const {
    if (!m_maze) {
        return 1;
    }
    if (!m_pendingResult.path.empty()) {
        return static_cast<int>(m_pendingResult.path.size());
    }
    if (!m_maze->path().empty()) {
        return static_cast<int>(m_maze->path().size());
    }
    return std::max(1, m_maze->maxSearchDistance() + 1);
}

void MazeWidget::resetAnimation() {
    m_animTimer.stop();
    m_animRunning = false;
    m_animPaused = false;
    m_hasActiveAnimation = false;
    m_traceIndex = 0;
    m_pathRevealIndex = 0;
    m_pathPhase = false;
}

void MazeWidget::resetPathAndAnimation() {
    resetAnimation();
    m_pendingResult = SolveResult{};
    if (m_maze) {
        m_maze->clearPath();
        m_maze->clearSearchOverlay();
    }
    update();
}

void MazeWidget::pauseAnimation() {
    if (!m_hasActiveAnimation || m_animPaused) {
        return;
    }
    m_animTimer.stop();
    m_animRunning = false;
    m_animPaused = true;
    emit animationPaused();
    update();
}

void MazeWidget::resumeAnimation() {
    if (!m_hasActiveAnimation || !m_animPaused) {
        return;
    }
    m_animPaused = false;
    m_animRunning = true;
    m_animTimer.start();
}

void MazeWidget::startAnimation(const SolveResult& result) {
    resetAnimation();
    if (!m_maze) {
        return;
    }

    m_pendingResult = result;
    m_maze->clearSearchOverlay();
    m_maze->setPath({});

    if (!m_animationEnabled || result.trace.empty()) {
        applyResultInstant(result);
        emit animationFinished(result);
        return;
    }

    m_hasActiveAnimation = true;
    m_animRunning = true;
    m_traceIndex = 0;
    m_pathRevealIndex = 0;
    m_pathPhase = false;
    m_animTimer.start();
}

void MazeWidget::applySearchEntry(const SearchTraceEntry& entry) {
    if (!m_maze) {
        return;
    }
    const SearchCellState state = entry.kind == SearchTraceKind::Enqueued ? SearchCellState::Frontier
                                                                          : SearchCellState::Expanded;
    if (m_maze->searchState(entry.cellIndex) != SearchCellState::Path) {
        m_maze->setSearchState(entry.cellIndex, state, entry.depth);
    }
}

void MazeWidget::applyResultInstant(const SolveResult& result) {
    if (!m_maze) {
        return;
    }
    resetAnimation();
    m_maze->clearSearchOverlay();
    if (result.found) {
        const int pathLen = static_cast<int>(result.path.size());
        for (int i = 0; i < pathLen; ++i) {
            const QPoint& p = result.path[static_cast<size_t>(i)];
            m_maze->setSearchState(m_maze->index(p.x(), p.y()), SearchCellState::Path, i);
        }
        m_maze->setPath(result.path);
    } else {
        m_maze->setPath({});
    }
    update();
}

void MazeWidget::advanceAnimationFrame() {
    if (!m_maze || m_animPaused) {
        return;
    }

    const int batch = stepsPerTick();

    if (!m_pathPhase) {
        int steps = 0;
        while (steps < batch && m_traceIndex < static_cast<int>(m_pendingResult.trace.size())) {
            applySearchEntry(m_pendingResult.trace[static_cast<size_t>(m_traceIndex++)]);
            ++steps;
        }
        if (m_traceIndex >= static_cast<int>(m_pendingResult.trace.size())) {
            m_pathPhase = true;
            m_pathRevealIndex = 0;
        }
    } else if (m_pendingResult.found) {
        int steps = 0;
        const int pathLen = static_cast<int>(m_pendingResult.path.size());
        while (steps < batch && m_pathRevealIndex < pathLen) {
            const QPoint& p = m_pendingResult.path[static_cast<size_t>(m_pathRevealIndex)];
            m_maze->setSearchState(
                m_maze->index(p.x(), p.y()),
                SearchCellState::Path,
                m_pathRevealIndex);
            ++m_pathRevealIndex;
            ++steps;
        }
    }

    if (m_pathPhase &&
        (!m_pendingResult.found ||
         m_pathRevealIndex >= static_cast<int>(m_pendingResult.path.size()))) {
        resetAnimation();
        if (m_pendingResult.found) {
            m_maze->setPath(m_pendingResult.path);
        }
        update();
        emit animationFinished(m_pendingResult);
        return;
    }

    update();
}

void MazeWidget::onAnimTick() {
    advanceAnimationFrame();
}

MazeWidget::LayoutMetrics MazeWidget::layoutMetrics() const {
    LayoutMetrics lm;
    if (!m_maze || m_maze->width() <= 0 || m_maze->height() <= 0) {
        return lm;
    }

    const int gw = m_maze->gridWidth();
    const int gh = m_maze->gridHeight();
    const int pad = 16;
    const int availW = width() - pad * 2;
    const int availH = height() - pad * 2;
    lm.cellSize = std::max(3, std::min(availW / gw, availH / gh));
    const int mazeW = lm.cellSize * gw;
    const int mazeH = lm.cellSize * gh;
    lm.offsetX = (width() - mazeW) / 2;
    lm.offsetY = (height() - mazeH) / 2;
    lm.mazeRect = QRect(lm.offsetX, lm.offsetY, mazeW, mazeH);
    return lm;
}

QPoint MazeWidget::roomAt(const QPoint& pos, const LayoutMetrics& lm) const {
    if (!m_maze || lm.cellSize <= 0 || !lm.mazeRect.contains(pos)) {
        return QPoint(-1, -1);
    }

    const int gx = (pos.x() - lm.offsetX) / lm.cellSize;
    const int gy = (pos.y() - lm.offsetY) / lm.cellSize;
    if (!m_maze->isRoomCell(gx, gy) || m_maze->isWall(gx, gy)) {
        return QPoint(-1, -1);
    }

    const QPoint room = m_maze->gridToRoom(gx, gy);
    if (!m_maze->inBounds(room.x(), room.y())) {
        return QPoint(-1, -1);
    }
    return room;
}

void MazeWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0, QColor(22, 26, 36));
    bg.setColorAt(1, QColor(14, 17, 24));
    painter.fillRect(rect(), bg);

    if (!m_maze) {
        return;
    }

    const LayoutMetrics lm = layoutMetrics();
    if (lm.cellSize <= 0) {
        return;
    }

    painter.save();
    painter.translate(lm.offsetX, lm.offsetY);

    QRect panel(0, 0, lm.mazeRect.width(), lm.mazeRect.height());
    painter.setPen(QPen(QColor(58, 66, 86), 2));
    painter.setBrush(QColor(28, 32, 42));
    painter.drawRoundedRect(panel.adjusted(-6, -6, 6, 6), 10, 10);

    drawMaze(painter, lm);
    painter.restore();
}

void MazeWidget::drawMaze(QPainter& painter, const LayoutMetrics& lm) {
    const int cell = lm.cellSize;
    const int gw = m_maze->gridWidth();
    const int gh = m_maze->gridHeight();
    const int maxDepth = m_maze->maxSearchDistance();
    const int pathLen = pathVisualLength();

    painter.setPen(Qt::NoPen);
    
    const QPoint startGrid = Maze::roomToGrid(m_maze->start().x(), m_maze->start().y());
    const QPoint endGrid = Maze::roomToGrid(m_maze->end().x(), m_maze->end().y());
    
    static QColor wallClr = wallColor();

    QSet<QPoint> pathCells;
    QHash<QPoint, int> pathStepIndex; 
    
    const bool hasPathAnimation = m_hasActiveAnimation && m_pathPhase;
    const bool hasInstantPath = !m_hasActiveAnimation && !m_maze->path().empty();
    
    if (hasPathAnimation || hasInstantPath) {
        const auto& path = hasPathAnimation ? m_pendingResult.path : m_maze->path();
        const int revealedCount = hasPathAnimation ? m_pathRevealIndex : static_cast<int>(path.size());
        
        for (int i = 0; i < revealedCount && i < static_cast<int>(path.size()); ++i) {
            const QPoint& room = path[i];
            QPoint gridPos = Maze::roomToGrid(room.x(), room.y());
            pathCells.insert(gridPos);
            pathStepIndex[gridPos] = i;
            
            if (i > 0) {
                const QPoint& prevRoom = path[i - 1];
                QPoint prevGridPos = Maze::roomToGrid(prevRoom.x(), prevRoom.y());
                QPoint corridorPos(
                    (gridPos.x() + prevGridPos.x()) / 2,
                    (gridPos.y() + prevGridPos.y()) / 2
                );
                pathCells.insert(corridorPos);
                pathStepIndex[corridorPos] = i - 1; 
            }
        }
    }

    for (int gy = 0; gy < gh; ++gy) {
        for (int gx = 0; gx < gw; ++gx) {
            const QRect r(gx * cell, gy * cell, cell, cell);
            const QPoint gridPos(gx, gy);
            
            if (m_maze->isWall(gx, gy)) {
                painter.setBrush(wallClr);
                painter.drawRect(r);
                continue;
            }

            if (pathCells.contains(gridPos)) {
                int stepIndex = pathStepIndex.value(gridPos, 0);
                const int totalPathSize = std::max(1, static_cast<int>(
                    m_pendingResult.path.size() > 0 ? m_pendingResult.path.size() : m_maze->path().size()
                ));
                painter.setBrush(pathColor(stepIndex, totalPathSize));
                painter.drawRect(r);
                continue;
            }

            const bool isStart = (gx == startGrid.x() && gy == startGrid.y());
            const bool isEnd = (gx == endGrid.x() && gy == endGrid.y());
            
            if (isStart) {
                QLinearGradient grad(r.topLeft(), r.bottomRight());
                grad.setColorAt(0, QColor(76, 175, 80));
                grad.setColorAt(1, QColor(46, 125, 50));
                painter.setBrush(grad);
                painter.drawRect(r);
                continue;
            }
            
            if (isEnd) {
                QLinearGradient grad(r.topLeft(), r.bottomRight());
                grad.setColorAt(0, QColor(239, 83, 80));
                grad.setColorAt(1, QColor(198, 40, 40));
                painter.setBrush(grad);
                painter.drawRect(r);
                continue;
            }

            bool overlayDrawn = false;
            if (m_hasActiveAnimation || hasInstantPath) {
                if (m_maze->isRoomCell(gx, gy) && m_maze->isPassage(gx, gy)) {
                    const QPoint room = m_maze->gridToRoom(gx, gy);
                    if (auto overlay = overlayAtRoom(*m_maze, m_maze->index(room.x(), room.y()))) {
                        if (overlay->state != SearchCellState::Path) {
                            painter.setBrush(colorForOverlay(overlay->state, overlay->depth, m_maze->maxSearchDistance(), pathVisualLength()));
                            painter.drawRect(r);
                            overlayDrawn = true;
                        }
                    }
                } else if (m_maze->isPassage(gx, gy)) {
                    if (auto overlay = overlayAtCorridor(*m_maze, gx, gy)) {
                        if (overlay->state != SearchCellState::Path) {
                            painter.setBrush(colorForOverlay(overlay->state, overlay->depth, m_maze->maxSearchDistance(), pathVisualLength()));
                            painter.drawRect(r);
                            overlayDrawn = true;
                        }
                    }
                }
            }

            if (!overlayDrawn) {
                const bool isRoom = m_maze->isRoomCell(gx, gy);
                painter.setBrush(passageColor(gx, gy, isRoom));
                painter.drawRect(r);
            }
        }
    }
}

void MazeWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    update();
}

void MazeWidget::mousePressEvent(QMouseEvent* event) {
    if (!m_maze || m_placementMode == PlacementMode::None || m_animRunning) {
        return;
    }

    const LayoutMetrics lm = layoutMetrics();
    const QPoint room = roomAt(event->pos(), lm);
    if (room.x() < 0) {
        return;
    }

    resetPathAndAnimation();
    if (m_placementMode == PlacementMode::Start) {
        m_maze->setStart(room);
        emit startChanged(room);
    } else {
        m_maze->setEnd(room);
        emit endChanged(room);
    }
    m_maze->clearPath();
    update();
}