#include "solvers/SolverUtils.h"

#include <QElapsedTimer>
#include <climits>
#include <queue>

namespace SolverUtils {

void forEachNeighbor(const Maze& maze, int w, int cellIdx, const std::function<void(int neighborIdx)>& visit) {
    const QPoint cp = indexToPoint(w, cellIdx);
    for (Wall wall : directions()) {
        if (!canMove(maze, cp.x(), cp.y(), wall)) {
            continue;
        }
        const QPoint offset = Maze::directionOffset(wall);
        const int nx = cp.x() + offset.x();
        const int ny = cp.y() + offset.y();
        visit(cellIndex(w, nx, ny));
    }
}

void applyPathToMaze(Maze& maze, const SolveResult& result, const SolveOptions& options) {
    if (!options.recordTrace && result.found) {
        maze.setPath(result.path);
    } else if (!result.found) {
        maze.setPath({});
    }
}

SolveResult orderedGridSearch(
    Maze& maze,
    const SolveOptions& options,
    const QString& algorithmName,
    bool guaranteesShortest,
    bool useStack) {
    QElapsedTimer timer;
    timer.start();

    SolveResult result;
    result.algorithm = algorithmName;
    result.isShortest = guaranteesShortest;

    const QPoint start = maze.start();
    const QPoint end = maze.end();
    const int w = maze.width();
    const int startIdx = cellIndex(w, start.x(), start.y());
    const int endIdx = cellIndex(w, end.x(), end.y());
    const int nCells = w * maze.height();

    std::vector<uint8_t> visited(static_cast<size_t>(nCells), 0);
    std::vector<int> parent(static_cast<size_t>(nCells), -1);
    std::vector<int> depth(static_cast<size_t>(nCells), -1);

    std::vector<int> fifo;
    fifo.push_back(startIdx);
    visited[static_cast<size_t>(startIdx)] = 1;
    depth[static_cast<size_t>(startIdx)] = 0;
    result.nodesEnqueued = 1;
    result.maxFrontier = 1;
    recordTrace(result.trace, startIdx, SearchTraceKind::Enqueued, 0, options.recordTrace);

    size_t head = 0;
    while (head < fifo.size()) {
        const int current = useStack ? fifo.back() : fifo[static_cast<size_t>(head)];
        if (useStack) {
            fifo.pop_back();
        } else {
            ++head;
        }

        result.maxFrontier = std::max(result.maxFrontier, static_cast<int>(fifo.size() - head));
        ++result.nodesExpanded;
        recordTrace(
            result.trace,
            current,
            SearchTraceKind::Expanded,
            depth[static_cast<size_t>(current)],
            options.recordTrace);

        if (current == endIdx) {
            result.found = true;
            result.path = reconstructPath(parent, w, startIdx, endIdx);
            result.pathLength = static_cast<int>(result.path.size());
            applyPathToMaze(maze, result, options);
            result.elapsedMicros = timer.nsecsElapsed() / 1000;
            return result;
        }

        const int curDepth = depth[static_cast<size_t>(current)];
        forEachNeighbor(maze, w, current, [&](int nIdx) {
            if (visited[static_cast<size_t>(nIdx)]) {
                return;
            }
            visited[static_cast<size_t>(nIdx)] = 1;
            parent[static_cast<size_t>(nIdx)] = current;
            depth[static_cast<size_t>(nIdx)] = curDepth + 1;
            fifo.push_back(nIdx);
            ++result.nodesEnqueued;
            recordTrace(
                result.trace,
                nIdx,
                SearchTraceKind::Enqueued,
                depth[static_cast<size_t>(nIdx)],
                options.recordTrace);
        });
    }

    applyPathToMaze(maze, result, options);
    result.elapsedMicros = timer.nsecsElapsed() / 1000;
    return result;
}

namespace {

struct PQNode {
    int priority;
    int idx;
    bool operator>(const PQNode& other) const { return priority > other.priority; }
};

}

SolveResult weightedGridSearch(
    Maze& maze,
    const SolveOptions& options,
    const QString& algorithmName,
    bool guaranteesShortest,
    const std::function<int(int cellIdx, int gScore)>& priorityKey) {
    QElapsedTimer timer;
    timer.start();

    SolveResult result;
    result.algorithm = algorithmName;
    result.isShortest = guaranteesShortest;

    const QPoint start = maze.start();
    const QPoint end = maze.end();
    const int w = maze.width();
    const int startIdx = cellIndex(w, start.x(), start.y());
    const int endIdx = cellIndex(w, end.x(), end.y());
    const int nCells = w * maze.height();

    std::vector<int> gScore(static_cast<size_t>(nCells), INT_MAX);
    std::vector<int> parent(static_cast<size_t>(nCells), -1);
    std::vector<uint8_t> inOpen(static_cast<size_t>(nCells), 0);

    std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> open;
    gScore[static_cast<size_t>(startIdx)] = 0;
    open.push({priorityKey(startIdx, 0), startIdx});
    inOpen[static_cast<size_t>(startIdx)] = 1;
    result.nodesEnqueued = 1;
    result.maxFrontier = 1;
    recordTrace(result.trace, startIdx, SearchTraceKind::Enqueued, 0, options.recordTrace);

    while (!open.empty()) {
        result.maxFrontier = std::max(result.maxFrontier, static_cast<int>(open.size()));
        const PQNode current = open.top();
        open.pop();

        if (!inOpen[static_cast<size_t>(current.idx)]) {
            continue;
        }
        inOpen[static_cast<size_t>(current.idx)] = 0;
        ++result.nodesExpanded;
        recordTrace(
            result.trace,
            current.idx,
            SearchTraceKind::Expanded,
            gScore[static_cast<size_t>(current.idx)],
            options.recordTrace);

        if (current.idx == endIdx) {
            result.found = true;
            result.path = reconstructPath(parent, w, startIdx, endIdx);
            result.pathLength = static_cast<int>(result.path.size());
            applyPathToMaze(maze, result, options);
            result.elapsedMicros = timer.nsecsElapsed() / 1000;
            return result;
        }

        const int g = gScore[static_cast<size_t>(current.idx)];
        forEachNeighbor(maze, w, current.idx, [&](int nIdx) {
            const int tentativeG = g + 1;
            if (tentativeG >= gScore[static_cast<size_t>(nIdx)]) {
                return;
            }
            gScore[static_cast<size_t>(nIdx)] = tentativeG;
            parent[static_cast<size_t>(nIdx)] = current.idx;
            open.push({priorityKey(nIdx, tentativeG), nIdx});
            inOpen[static_cast<size_t>(nIdx)] = 1;
            ++result.nodesEnqueued;
            recordTrace(
                result.trace,
                nIdx,
                SearchTraceKind::Enqueued,
                tentativeG,
                options.recordTrace);
        });
    }

    applyPathToMaze(maze, result, options);
    result.elapsedMicros = timer.nsecsElapsed() / 1000;
    return result;
}

}