#pragma once

#include "solvers/PathSolver.h"

class AStarSolver : public PathSolver {
public:
    QString name() const override { return QStringLiteral("A*"); }
    bool guaranteesShortest() const override { return true; }
    SolveResult solve(Maze& maze, const SolveOptions& options = {}) override;
};
