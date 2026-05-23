#pragma once

#include "Maze.h"
#include "solvers/PathSolver.h"

#include <array>
#include <functional>
#include <vector>

namespace SolverUtils {

inline int cellIndex(int w, int x, int y) {
    return y * w + x;
}

inline QPoint indexToPoint(int w, int idx) {
    return QPoint(idx % w, idx / w);
}

inline bool canMove(const Maze& maze, int x, int y, Wall wall) {
    const QPoint offset = Maze::directionOffset(wall);
    const int nx = x + offset.x();
    const int ny = y + offset.y();
    if (!maze.inBounds(nx, ny)) {
        return false;
    }
    return !maze.hasWallBetween(x, y, nx, ny);
}

inline void recordTrace(
    std::vector<SearchTraceEntry>& trace,
    int idx,
    SearchTraceKind kind,
    int depth,
    bool enabled) {
    if (!enabled) {
        return;
    }
    if (static_cast<int>(trace.size()) >= Maze::kMaxTraceSteps) {
        return;
    }
    trace.push_back({idx, kind, depth});
}

inline std::vector<QPoint> reconstructPath(const std::vector<int>& parent, int w, int startIdx, int endIdx) {
    std::vector<QPoint> path;
    int cur = endIdx;
    while (cur != -1) {
        path.push_back(indexToPoint(w, cur));
        if (cur == startIdx) {
            break;
        }
        cur = parent[static_cast<size_t>(cur)];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

inline const std::array<Wall, 4>& directions() {
    static const std::array<Wall, 4> dirs{Wall::Top, Wall::Right, Wall::Bottom, Wall::Left};
    return dirs;
}

void forEachNeighbor(const Maze& maze, int w, int cellIdx, const std::function<void(int neighborIdx)>& visit);

void applyPathToMaze(Maze& maze, const SolveResult& result, const SolveOptions& options);

SolveResult orderedGridSearch(
    Maze& maze,
    const SolveOptions& options,
    const QString& algorithmName,
    bool guaranteesShortest,
    bool useStack);

SolveResult weightedGridSearch(
    Maze& maze,
    const SolveOptions& options,
    const QString& algorithmName,
    bool guaranteesShortest,
    const std::function<int(int cellIdx, int gScore)>& priorityKey);

} // namespace
