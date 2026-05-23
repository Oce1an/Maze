#include "solvers/AStarSolver.h"

#include "solvers/SolverUtils.h"

#include <cmath>

SolveResult AStarSolver::solve(Maze& maze, const SolveOptions& options) {
    const QPoint end = maze.end();
    const int w = maze.width();

    return SolverUtils::weightedGridSearch(
        maze,
        options,
        name(),
        true,
        [w, end](int cellIdx, int gScore) {
            const QPoint p = SolverUtils::indexToPoint(w, cellIdx);
            const int h = std::abs(p.x() - end.x()) + std::abs(p.y() - end.y());
            return gScore + h;
        });
}
