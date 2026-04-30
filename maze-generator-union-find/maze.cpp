// =============================================================================
//  Random perfect-maze generation by Kruskal-style edge knockdown.
// =============================================================================
//
//  Builds a 5×5 grid where every cell is initially walled off from its
//  neighbours, then knocks down walls one at a time — but only when doing
//  so connects two previously-disjoint regions.  The resulting maze has
//  exactly one path between any two cells (a "perfect" maze: no loops,
//  no isolated regions).
//
//  Algorithm:
//
//    1. Build the list of all interior walls (edges between adjacent cells).
//    2. Shuffle the list with a high-quality PRNG.
//    3. Walk the list:
//         - if the wall separates two different DSU components, merge them
//           and record the wall as "removed";
//         - otherwise the wall stays in place.
//       Stop after we've performed (n-1) merges (the maze is now fully
//       connected; any remaining walls are interior structure).
//
//  Why DSU?
//  --------
//  The maze-construction problem is *exactly* "build a spanning tree on the
//  grid graph", and the classical algorithm for that on an unweighted graph
//  is Kruskal's MST + union-find.  With path compression + union-by-rank the
//  per-edge cost is amortised O(α(n)), so even very large mazes are fast.
//
//  Build & run:
//      g++ -std=c++17 -O2 -Wall maze.cpp -o maze && ./maze
// =============================================================================

#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

using namespace std;

class DisjointSets {
public:
    explicit DisjointSets(int n) : s_(n, -1) {}

    int find(int x) {
        if (s_[x] < 0) return x;
        return s_[x] = find(s_[x]);
    }

    bool union_by_rank(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (s_[rx] < s_[ry]) {
            s_[ry] = rx;
        } else {
            if (s_[rx] == s_[ry]) s_[ry]--;
            s_[rx] = ry;
        }
        return true;
    }

private:
    vector<int> s_;
};

int main() {
    constexpr int N = 5;
    constexpr int CELLS = N * N;

    DisjointSets maze(CELLS);

    // 1. Build the list of all interior walls.
    vector<pair<int, int>> walls;
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            int cell = row * N + col;
            if (col + 1 < N) walls.emplace_back(cell, cell + 1);     // horizontal wall
            if (row + 1 < N) walls.emplace_back(cell, cell + N);     // vertical wall
        }
    }

    // 2. Shuffle.
    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(walls.begin(), walls.end(), g);

    // 3. Knock walls down until the grid is fully connected.
    vector<pair<int, int>> remaining;
    int unions = 0;
    size_t i = 0;
    for (; i < walls.size(); ++i) {
        if (maze.union_by_rank(walls[i].first, walls[i].second)) {
            ++unions;
            if (unions == CELLS - 1) { ++i; break; }
        } else {
            remaining.push_back(walls[i]);
        }
    }
    for (; i < walls.size(); ++i) remaining.push_back(walls[i]);

    cout << "Generated 5×5 perfect maze.\n";
    cout << "Walls remaining (interior structure):\n";
    for (const auto& w : remaining) {
        cout << "  (" << w.first << ", " << w.second << ")\n";
    }
    cout << "\nTotal walls knocked down: " << unions
         << "  (must equal " << CELLS - 1 << " = N*N - 1 for a spanning tree)\n";
    return 0;
}
