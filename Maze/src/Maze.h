#pragma once

#include <QPoint>
#include <QString>
#include <cstdint>
#include <vector>

enum class Wall { Top = 0, Right = 1, Bottom = 2, Left = 3 };

enum class Tile : uint8_t { Wall = 0, Passage = 1 };

enum class SearchCellState : uint8_t {
    None = 0,
    Frontier = 1,
    Expanded = 2,
    Path = 3,
};

class Maze {
public:
    static constexpr int kMaxTraceSteps = 50000;

    Maze(int width = 31, int height = 21);

    int width() const { return m_width; }
    int height() const { return m_height; }
    int gridWidth() const { return 2 * m_width + 1; }
    int gridHeight() const { return 2 * m_height + 1; }
    int cellCount() const { return m_width * m_height; }

    void resize(int width, int height);
    void reset();

    bool inBounds(int roomX, int roomY) const;
    bool inGridBounds(int gx, int gy) const;
    int index(int roomX, int roomY) const;
    QPoint pointFromIndex(int idx) const;

    Tile tileAt(int gx, int gy) const;
    bool isWall(int gx, int gy) const;
    bool isPassage(int gx, int gy) const;
    bool isRoomCell(int gx, int gy) const;

    static QPoint roomToGrid(int roomX, int roomY);
    QPoint gridToRoom(int gx, int gy) const;

    bool hasWallBetween(int x1, int y1, int x2, int y2) const;
    void removeWall(int x1, int y1, int x2, int y2);

    QPoint start() const { return m_start; }
    QPoint end() const { return m_end; }
    void setStart(const QPoint& p);
    void setEnd(const QPoint& p);

    const std::vector<QPoint>& path() const { return m_path; }
    void setPath(std::vector<QPoint> path);
    void clearPath();

    void clearSearchOverlay();
    void setSearchState(int cellIndex, SearchCellState state, int distanceFromStart = -1);
    SearchCellState searchState(int cellIndex) const;
    int searchDistance(int cellIndex) const;
    int maxSearchDistance() const { return m_maxSearchDistance; }

    QString lastSolveInfo() const { return m_solveInfo; }
    void setSolveInfo(const QString& info) { m_solveInfo = info; }

    static QPoint directionOffset(Wall wall);
    static Wall oppositeWall(Wall wall);

private:
    int gridIndex(int gx, int gy) const;
    void setTile(int gx, int gy, Tile tile);

    int m_width;
    int m_height;
    std::vector<Tile> m_grid;
    std::vector<SearchCellState> m_searchOverlay;
    std::vector<int> m_searchDistance;
    int m_maxSearchDistance{0};
    QPoint m_start{0, 0};
    QPoint m_end;
    std::vector<QPoint> m_path;
    QString m_solveInfo;
};
