#pragma once

#include <QPoint>
#include <QString>
#include <cstdint>
#include <memory>
#include <vector>

class Maze;

enum class SearchTraceKind : uint8_t {
    Enqueued = 0,
    Expanded = 1,
    Path = 2,
};

struct SearchTraceEntry {
    int cellIndex{0};
    SearchTraceKind kind{SearchTraceKind::Enqueued};
    int depth{0};
};

struct SolveOptions {
    bool recordTrace{false};
};

struct SolveResult {
    bool found{false};
    std::vector<QPoint> path;
    std::vector<SearchTraceEntry> trace;
    int nodesExpanded{0};
    int nodesEnqueued{0};
    int maxFrontier{0};
    int pathLength{0};
    bool isShortest{false};
    qint64 elapsedMicros{0};
    QString algorithm;
};

class PathSolver {
public:
    virtual ~PathSolver() = default;
    virtual QString name() const = 0;
    virtual bool guaranteesShortest() const = 0;
    virtual SolveResult solve(Maze& maze, const SolveOptions& options = {}) = 0;

    static void initRegistry();
    static int count();
    static PathSolver* byIndex(int index);
    static QString nameAt(int index);

    static void shutdownRegistry();

private:
    friend struct SolverRegistryCleanup;
    static std::vector<std::unique_ptr<PathSolver>> s_solvers;
};

struct SolverRegistryCleanup {
    ~SolverRegistryCleanup();
};
