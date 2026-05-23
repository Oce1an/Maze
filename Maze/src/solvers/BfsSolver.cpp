#include "solvers/BfsSolver.h"

#include "solvers/SolverUtils.h"

SolveResult BfsSolver::solve(Maze& maze, const SolveOptions& options) {
    return SolverUtils::orderedGridSearch(maze, options, name(), true, false);
}
