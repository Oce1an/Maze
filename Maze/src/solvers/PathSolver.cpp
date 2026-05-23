#include "solvers/PathSolver.h"

#include "solvers/AStarSolver.h"
#include "solvers/BfsSolver.h"
#include "solvers/DfsSolver.h"
#include "solvers/DijkstraSolver.h"

std::vector<std::unique_ptr<PathSolver>> PathSolver::s_solvers;

void PathSolver::shutdownRegistry() {
    s_solvers.clear();
}

SolverRegistryCleanup::~SolverRegistryCleanup() {
    PathSolver::shutdownRegistry();
}

static SolverRegistryCleanup g_solverCleanup;

void PathSolver::initRegistry() {
    if (!s_solvers.empty()) {
        return;
    }
    s_solvers.push_back(std::make_unique<BfsSolver>());
    s_solvers.push_back(std::make_unique<DfsSolver>());
    s_solvers.push_back(std::make_unique<DijkstraSolver>());
    s_solvers.push_back(std::make_unique<AStarSolver>());
}

int PathSolver::count() {
    initRegistry();
    return static_cast<int>(s_solvers.size());
}

PathSolver* PathSolver::byIndex(int index) {
    initRegistry();
    if (index < 0 || index >= static_cast<int>(s_solvers.size())) {
        return nullptr;
    }
    return s_solvers[static_cast<size_t>(index)].get();
}

QString PathSolver::nameAt(int index) {
    initRegistry();
    if (index < 0 || index >= static_cast<int>(s_solvers.size())) {
        return {};
    }
    return s_solvers[static_cast<size_t>(index)]->name();
}
