#include "generators/KruskalGenerator.h"

#include "Maze.h"

#include <QRandomGenerator>
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

namespace {

struct Edge {
    int x1, y1, x2, y2;
};

class UnionFind {
public:
    explicit UnionFind(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[static_cast<size_t>(x)] != x) {
            parent[static_cast<size_t>(x)] = find(parent[static_cast<size_t>(x)]);
        }
        return parent[static_cast<size_t>(x)];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (rank[static_cast<size_t>(a)] < rank[static_cast<size_t>(b)]) {
            std::swap(a, b);
        }
        parent[static_cast<size_t>(b)] = a;
        if (rank[static_cast<size_t>(a)] == rank[static_cast<size_t>(b)]) {
            ++rank[static_cast<size_t>(a)];
        }
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

std::mt19937 makeRng() {
    return std::mt19937(static_cast<std::mt19937::result_type>(QRandomGenerator::global()->generate64()));
}

} // namespace

void KruskalGenerator::generate(Maze& maze) {
    maze.reset();

    std::vector<Edge> edges;
    const int w = maze.width();
    const int h = maze.height();

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (x + 1 < w) {
                edges.push_back({x, y, x + 1, y});
            }
            if (y + 1 < h) {
                edges.push_back({x, y, x, y + 1});
            }
        }
    }

    std::mt19937 rng = makeRng();
    std::shuffle(edges.begin(), edges.end(), rng);

    UnionFind uf(w * h);
    auto cellId = [w](int x, int y) { return y * w + x; };

    for (const Edge& e : edges) {
        const int id1 = cellId(e.x1, e.y1);
        const int id2 = cellId(e.x2, e.y2);
        if (uf.unite(id1, id2)) {
            maze.removeWall(e.x1, e.y1, e.x2, e.y2);
        }
    }
}
