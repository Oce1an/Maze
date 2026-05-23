#pragma once

#include "generators/MazeGenerator.h"

class KruskalGenerator : public MazeGenerator {
public:
    QString name() const override { return QStringLiteral("Kruskal"); }
    void generate(Maze& maze) override;
};
