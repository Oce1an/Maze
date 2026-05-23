#pragma once

#include "solvers/PathSolver.h"

class DijkstraSolver : public PathSolver {
public:
    QString name() const override { return QStringLiteral("Dijkstra"); }
    bool guaranteesShortest() const override { return true; }
    SolveResult solve(Maze& maze, const SolveOptions& options = {}) override;
};
