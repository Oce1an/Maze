#include "generators/MazeGenerator.h"

#include "generators/DepthFirstGenerator.h"
#include "generators/KruskalGenerator.h"
#include "generators/PrimGenerator.h"

static std::vector<std::unique_ptr<MazeGenerator>> g_generators;

static void ensureGenerators() {
    if (!g_generators.empty()) {
        return;
    }
    g_generators.push_back(std::make_unique<DepthFirstGenerator>());
    g_generators.push_back(std::make_unique<PrimGenerator>());
    g_generators.push_back(std::make_unique<KruskalGenerator>());
}

struct GeneratorCleanup {
    ~GeneratorCleanup() { g_generators.clear(); }
};

static GeneratorCleanup g_generatorCleanup;

int mazeGeneratorCount() {
    ensureGenerators();
    return static_cast<int>(g_generators.size());
}

QString mazeGeneratorNameAt(int index) {
    ensureGenerators();
    if (index < 0 || index >= static_cast<int>(g_generators.size())) {
        return {};
    }
    return g_generators[static_cast<size_t>(index)]->name();
}

MazeGenerator* MazeGenerator::byIndex(int index) {
    ensureGenerators();
    if (index < 0 || index >= static_cast<int>(g_generators.size())) {
        return nullptr;
    }
    return g_generators[static_cast<size_t>(index)].get();
}
