#pragma once

#include "generators/MazeGenerator.h"

class DepthFirstGenerator : public MazeGenerator {
public:
    QString name() const override { return QStringLiteral("DFS (Recursive Backtracker)"); }
    void generate(Maze& maze) override;
};
