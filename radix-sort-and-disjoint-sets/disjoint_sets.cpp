// =============================================================================
//  Disjoint-Set / Union-Find with union-by-rank and path compression.
// =============================================================================
//
//  Time:  effectively O(α(n)) per operation amortised, where α(·) is the
//         inverse Ackermann function (≤ 4 for any n that fits in a universe).
//  Space: O(n)
//
//  How it works
//  ------------
//  Each element is a node in an inverted forest, with edges pointing from
//  child to parent.  The root of an element's tree identifies its set; two
//  elements share a set iff `find(x) == find(y)`.
//
//  Two optimisations make this fast:
//
//  1. **Path compression** — in `find()`, we re-parent every node on the
//     path to the root directly to the root.  After repeated calls the trees
//     flatten almost completely.
//
//  2. **Union by rank (≈ height)** — when merging two trees, the shorter one
//     hangs under the taller one's root.  Combined with path compression
//     this drives the amortised cost down to O(α(n)).
//
//  Storage trick
//  -------------
//  We use a single `vector<int> s` with a clever sign convention:
//
//    * non-negative entries → "this node's parent is at index s[i]"
//    * negative entries     → "this node is a root, and its rank is -s[i]"
//
//  This avoids the cost of two parallel arrays.
//
//  Build & run:
//      g++ -std=c++17 -O2 -Wall disjoint_sets.cpp -o dsu && ./dsu
// =============================================================================

#include <iostream>
#include <vector>

using namespace std;

class DisjointSets {
public:
    explicit DisjointSets(int n) : s_(n, -1) {}

    int find(int x) {
        if (s_[x] < 0) return x;
        return s_[x] = find(s_[x]);   // path compression
    }

    // Union two sets by rank.  Returns true if a merge actually happened.
    bool union_by_rank(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;

        // s_[root] holds -rank, so "more negative" == "taller tree".
        if (s_[rx] < s_[ry]) {
            s_[ry] = rx;
        } else {
            if (s_[rx] == s_[ry]) s_[ry]--;
            s_[rx] = ry;
        }
        return true;
    }

    bool same_set(int x, int y) { return find(x) == find(y); }

    int size() const { return static_cast<int>(s_.size()); }

private:
    vector<int> s_;
};

int main() {
    DisjointSets dsu(7);

    cout << "initial: 7 singletons {0} {1} {2} {3} {4} {5} {6}\n";
    cout << "  same_set(0, 3)?  " << (dsu.same_set(0, 3) ? "yes" : "no ") << "\n";

    dsu.union_by_rank(0, 1);
    dsu.union_by_rank(2, 3);
    dsu.union_by_rank(4, 5);
    dsu.union_by_rank(0, 3);          // merges {0,1} with {2,3}
    cout << "\nafter unions {0,1}, {2,3}, {4,5}, {0,1,2,3}:\n";
    cout << "  same_set(0, 3)?  " << (dsu.same_set(0, 3) ? "yes" : "no ") << "\n";
    cout << "  same_set(0, 4)?  " << (dsu.same_set(0, 4) ? "yes" : "no ") << "\n";
    cout << "  same_set(4, 6)?  " << (dsu.same_set(4, 6) ? "yes" : "no ") << "\n";

    return 0;
}
