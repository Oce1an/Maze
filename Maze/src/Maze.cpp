#include "Maze.h"

#include <algorithm>
#include <cstdlib>
#include <stdexcept>

Maze::Maze(int width, int height) : m_width(width), m_height(height) {
    m_end = QPoint(width - 1, height - 1);
    reset();
}

void Maze::resize(int width, int height) {
    m_width = std::max(1, width);
    m_height = std::max(1, height);
    m_start.setX(std::clamp(m_start.x(), 0, m_width - 1));
    m_start.setY(std::clamp(m_start.y(), 0, m_height - 1));
    m_end.setX(std::clamp(m_end.x(), 0, m_width - 1));
    m_end.setY(std::clamp(m_end.y(), 0, m_height - 1));
    if (m_start == m_end && m_width > 1) {
        m_end = QPoint(m_width - 1, m_height - 1);
    }
    reset();
}

void Maze::reset() {
    m_grid.assign(static_cast<size_t>(gridWidth() * gridHeight()), Tile::Wall);
    m_searchOverlay.assign(static_cast<size_t>(m_width * m_height), SearchCellState::None);
    m_searchDistance.assign(static_cast<size_t>(m_width * m_height), -1);
    m_maxSearchDistance = 0;
    m_path.clear();
    m_solveInfo.clear();
}

bool Maze::inBounds(int roomX, int roomY) const {
    return roomX >= 0 && roomY >= 0 && roomX < m_width && roomY < m_height;
}

bool Maze::inGridBounds(int gx, int gy) const {
    return gx >= 0 && gy >= 0 && gx < gridWidth() && gy < gridHeight();
}

int Maze::index(int roomX, int roomY) const {
    return roomY * m_width + roomX;
}

QPoint Maze::pointFromIndex(int idx) const {
    return QPoint(idx % m_width, idx / m_width);
}

int Maze::gridIndex(int gx, int gy) const {
    return gy * gridWidth() + gx;
}

Tile Maze::tileAt(int gx, int gy) const {
    if (!inGridBounds(gx, gy)) {
        return Tile::Wall;
    }
    return m_grid[static_cast<size_t>(gridIndex(gx, gy))];
}

bool Maze::isWall(int gx, int gy) const {
    return tileAt(gx, gy) == Tile::Wall;
}

bool Maze::isPassage(int gx, int gy) const {
    return tileAt(gx, gy) == Tile::Passage;
}

bool Maze::isRoomCell(int gx, int gy) const {
    return (gx % 2 == 1) && (gy % 2 == 1);
}

QPoint Maze::roomToGrid(int roomX, int roomY) {
    return QPoint(2 * roomX + 1, 2 * roomY + 1);
}

QPoint Maze::gridToRoom(int gx, int gy) const {
    return QPoint((gx - 1) / 2, (gy - 1) / 2);
}

void Maze::setTile(int gx, int gy, Tile tile) {
    if (!inGridBounds(gx, gy)) {
        return;
    }
    m_grid[static_cast<size_t>(gridIndex(gx, gy))] = tile;
}

bool Maze::hasWallBetween(int x1, int y1, int x2, int y2) const {
    const int dx = std::abs(x2 - x1);
    const int dy = std::abs(y2 - y1);
    if (dx + dy != 1) {
        return true;
    }
    const int gx = x1 + x2 + 1;
    const int gy = y1 + y2 + 1;
    return isWall(gx, gy);
}

void Maze::removeWall(int x1, int y1, int x2, int y2) {
    const int dx = x2 - x1;
    const int dy = y2 - y1;

    if (std::abs(dx) + std::abs(dy) != 1) {
        return;
    }
    if (!inBounds(x1, y1) || !inBounds(x2, y2)) {
        return;
    }

    const QPoint g1 = roomToGrid(x1, y1);
    const QPoint g2 = roomToGrid(x2, y2);
    setTile(g1.x(), g1.y(), Tile::Passage);
    setTile(g2.x(), g2.y(), Tile::Passage);
    setTile((g1.x() + g2.x()) / 2, (g1.y() + g2.y()) / 2, Tile::Passage);
}

void Maze::setStart(const QPoint& p) {
    if (inBounds(p.x(), p.y())) {
        m_start = p;
    }
}

void Maze::setEnd(const QPoint& p) {
    if (inBounds(p.x(), p.y())) {
        m_end = p;
    }
}

void Maze::setPath(std::vector<QPoint> path) {
    m_path = std::move(path);
}

void Maze::clearPath() {
    m_path.clear();
    m_solveInfo.clear();
    clearSearchOverlay();
}

void Maze::clearSearchOverlay() {
    std::fill(m_searchOverlay.begin(), m_searchOverlay.end(), SearchCellState::None);
    std::fill(m_searchDistance.begin(), m_searchDistance.end(), -1);
    m_maxSearchDistance = 0;
}

void Maze::setSearchState(int cellIndex, SearchCellState state, int distanceFromStart) {
    if (cellIndex < 0 || cellIndex >= static_cast<int>(m_searchOverlay.size())) {
        return;
    }
    m_searchOverlay[static_cast<size_t>(cellIndex)] = state;
    if (distanceFromStart >= 0) {
        m_searchDistance[static_cast<size_t>(cellIndex)] = distanceFromStart;
        m_maxSearchDistance = std::max(m_maxSearchDistance, distanceFromStart);
    }
}

SearchCellState Maze::searchState(int cellIndex) const {
    if (cellIndex < 0 || cellIndex >= static_cast<int>(m_searchOverlay.size())) {
        return SearchCellState::None;
    }
    return m_searchOverlay[static_cast<size_t>(cellIndex)];
}

int Maze::searchDistance(int cellIndex) const {
    if (cellIndex < 0 || cellIndex >= static_cast<int>(m_searchDistance.size())) {
        return -1;
    }
    return m_searchDistance[static_cast<size_t>(cellIndex)];
}

QPoint Maze::directionOffset(Wall wall) {
    switch (wall) {
    case Wall::Top: return QPoint(0, -1);
    case Wall::Right: return QPoint(1, 0);
    case Wall::Bottom: return QPoint(0, 1);
    case Wall::Left: return QPoint(-1, 0);
    }
    return QPoint(0, 0);
}

Wall Maze::oppositeWall(Wall wall) {
    switch (wall) {
    case Wall::Top: return Wall::Bottom;
    case Wall::Right: return Wall::Left;
    case Wall::Bottom: return Wall::Top;
    case Wall::Left: return Wall::Right;
    }
    return Wall::Top;
}
