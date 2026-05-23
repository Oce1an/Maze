#include "solvers/DfsSolver.h"

#include "solvers/SolverUtils.h"

SolveResult DfsSolver::solve(Maze& maze, const SolveOptions& options) {
    return SolverUtils::orderedGridSearch(maze, options, name(), false, true);
}
