#include "solvers/DijkstraSolver.h"

#include "solvers/SolverUtils.h"

SolveResult DijkstraSolver::solve(Maze& maze, const SolveOptions& options) {
    return SolverUtils::weightedGridSearch(
        maze,
        options,
        name(),
        true,
        [](int, int gScore) { return gScore; });
}
