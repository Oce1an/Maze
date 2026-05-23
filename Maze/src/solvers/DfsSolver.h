#pragma once

#include "solvers/PathSolver.h"

class DfsSolver : public PathSolver {
public:
    QString name() const override { return QStringLiteral("DFS"); }
    bool guaranteesShortest() const override { return false; }
    SolveResult solve(Maze& maze, const SolveOptions& options = {}) override;
};
