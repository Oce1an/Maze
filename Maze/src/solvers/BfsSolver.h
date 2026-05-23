#pragma once

#include "solvers/PathSolver.h"

class BfsSolver : public PathSolver {
public:
    QString name() const override { return QStringLiteral("BFS"); }
    bool guaranteesShortest() const override { return true; }
    SolveResult solve(Maze& maze, const SolveOptions& options = {}) override;
};
