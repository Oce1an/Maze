#include "generators/PrimGenerator.h"

#include "Maze.h"

#include <QRandomGenerator>
#include <array>
#include <vector>

namespace {

struct Edge {
    int x1, y1, x2, y2;
};

constexpr std::array<QPoint, 4> kNeighborOffsets{
    QPoint(0, -1),
    QPoint(1, 0),
    QPoint(0, 1),
    QPoint(-1, 0),
};

}

void PrimGenerator::generate(Maze& maze) {
    maze.reset();

    std::vector<std::vector<bool>> inMaze(
        static_cast<size_t>(maze.height()),
        std::vector<bool>(static_cast<size_t>(maze.width()), false));

    std::vector<Edge> frontier;
    inMaze[0][0] = true;

    auto addFrontier = [&](int x, int y) {
        for (const QPoint& off : kNeighborOffsets) {
            const int nx = x + off.x();
            const int ny = y + off.y();
            if (maze.inBounds(nx, ny) && !inMaze[static_cast<size_t>(ny)][static_cast<size_t>(nx)]) {
                frontier.push_back({x, y, nx, ny});
            }
        }
    };

    addFrontier(0, 0);

    while (!frontier.empty()) {
        const int idx = QRandomGenerator::global()->bounded(static_cast<int>(frontier.size()));
        const Edge edge = frontier[static_cast<size_t>(idx)];
        frontier.erase(frontier.begin() + idx);

        if (inMaze[static_cast<size_t>(edge.y2)][static_cast<size_t>(edge.x2)]) {
            continue;
        }

        inMaze[static_cast<size_t>(edge.y2)][static_cast<size_t>(edge.x2)] = true;
        maze.removeWall(edge.x1, edge.y1, edge.x2, edge.y2);
        addFrontier(edge.x2, edge.y2);
    }
}
