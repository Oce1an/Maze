#pragma once

#include "generators/MazeGenerator.h"

class PrimGenerator : public MazeGenerator {
public:
    QString name() const override { return QStringLiteral("Prim"); }
    void generate(Maze& maze) override;
};
