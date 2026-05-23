#pragma once

#include <QString>
#include <memory>
#include <vector>

class Maze;

class MazeGenerator {
public:
    virtual ~MazeGenerator() = default;
    virtual QString name() const = 0;
    virtual void generate(Maze& maze) = 0;

    static MazeGenerator* byIndex(int index);
};

int mazeGeneratorCount();
QString mazeGeneratorNameAt(int index);
