#include "generators/DepthFirstGenerator.h"

#include "Maze.h"

#include <QRandomGenerator>
#include <array>
#include <stack>
#include <vector>

void DepthFirstGenerator::generate(Maze& maze) {
    maze.reset();

    std::vector<std::vector<bool>> visited(
        static_cast<size_t>(maze.height()),
        std::vector<bool>(static_cast<size_t>(maze.width()), false));

    std::stack<QPoint> stack;
    stack.push(QPoint(0, 0));
    visited[0][0] = true;

    while (!stack.empty()) {
        const QPoint current = stack.top();
        std::array<Wall, 4> dirs{Wall::Top, Wall::Right, Wall::Bottom, Wall::Left};
        std::vector<Wall> neighbors;

        for (Wall wall : dirs) {
            const QPoint offset = Maze::directionOffset(wall);
            const int nx = current.x() + offset.x();
            const int ny = current.y() + offset.y();
            if (maze.inBounds(nx, ny) && !visited[static_cast<size_t>(ny)][static_cast<size_t>(nx)]) {
                neighbors.push_back(wall);
            }
        }

        if (neighbors.empty()) {
            stack.pop();
            continue;
        }

        const Wall chosen = neighbors[QRandomGenerator::global()->bounded(neighbors.size())];
        const QPoint offset = Maze::directionOffset(chosen);
        const int nx = current.x() + offset.x();
        const int ny = current.y() + offset.y();

        maze.removeWall(current.x(), current.y(), nx, ny);
        visited[static_cast<size_t>(ny)][static_cast<size_t>(nx)] = true;
        stack.push(QPoint(nx, ny));
    }
}