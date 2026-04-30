<div align="center">

# C++ Algorithms Portfolio

**Six classical computer-science algorithms in clean, single-file, self-contained C++17: radix sort, union-find, topological sort, Kruskal's & Prim's MST, perfect-maze generation by Kruskal-style edge-knockdown, Huffman coding, and the optimal binary-search-tree DP.**

[![C++17](https://img.shields.io/badge/C%2B%2B-17-3178c6.svg)](#)
[![Build: GCC](https://img.shields.io/badge/build-g%2B%2B%20--std%3Dc%2B%2B17-orange.svg)](#)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

</div>

---

## Contents

- [Overview](#overview)
- [The algorithms](#the-algorithms)
- [Building & running](#building--running)
- [Repository layout](#repository-layout)
- [License](#license)

---

## Overview

Six standalone C++ algorithm implementations.  Each project folder
compiles to a single binary with one `g++` invocation, runs in well
under a second, and prints a small but illustrative demo of the
algorithm on hand-picked input.

The intent is to demonstrate fluency with the *building blocks* of
algorithm design — disjoint-set forests, dynamic programming on
intervals, greedy with a priority queue, induction-friendly graph
traversal — rather than to build production data-structure libraries.
Each implementation is small enough to read in five minutes.

---

## The algorithms

### 1. `radix-sort-and-disjoint-sets/`

Two foundational data-structure exercises bundled together.

**Radix sort** — `radix_sort.cpp` (~70 lines).  LSD (least-significant-
digit) radix sort over base-10 digits with stable counting sort as
the per-digit subroutine.  Time **O(d · (n + k))** where *d* is the
digit count of the largest value and *k = 10*.  Includes a 10 000-
element random-sort sanity check.

**Disjoint-set forest** — `disjoint_sets.cpp` (~80 lines).  Union-by-
rank + path compression DSU, with the rank stored as a *negative*
value in the same vector that holds parent pointers (a classic
storage trick: positive entries are parent indices, negative entries
mark roots and encode their rank).  Per-operation cost is amortised
**O(α(n))**, where α is the inverse Ackermann function (≤ 4 for any
*n* that fits in the universe).

### 2. `topological-sort/`

`topological_sort.cpp` — BFS-based Kahn's algorithm on a course-
prerequisite graph.  Builds the in-degree map, seeds a queue with all
zero-in-degree nodes, peels them off one at a time while decrementing
the in-degrees of their successors, and emits the result.

The fixed input is a real engineering curriculum prerequisite graph,
so the output is an order in which the courses *can* actually be
taken.  Time **O(V + E)**.

### 3. `maze-generator-union-find/`

`maze.cpp` — 5×5 perfect-maze generator built on top of a
disjoint-set forest.  This is *exactly* the Kruskal MST algorithm
applied to the unweighted grid graph: shuffle the wall list, then
walk it knocking each wall down only if it joins two previously-
disconnected components, until *N²−1* knockdowns have built a
spanning tree on the cells.

The remaining walls form the interior structure of a perfect maze
(exactly one path between any two cells, no loops, no isolated
regions).

### 4. `kruskal-mst/`

`kruskal_mst.cpp` — Kruskal's MST and **Prim's MST**, both implemented
on the same hand-defined 10-vertex weighted graph so their MSTs can
be compared directly.

- **Kruskal**: sort edges by weight, walk the list, add each edge to
  the MST if it doesn't form a cycle.  Cycle-detection via a DSU.
  Time **O(E log E)**.
- **Prim**: priority-queue–based greedy frontier expansion from a
  start vertex.  Time **O(E log V)** with a binary heap.

Both report a total MST weight of 40 on the embedded graph.

### 5. `huffman-coding/`

`huffman.cpp` — character-level Huffman code construction over a
sample English paragraph.  Builds the priority queue with raw
`Node*` pointers + a custom comparator, repeatedly pops the two
lowest-frequency nodes, merges them under a parent whose frequency
is the sum, and pushes the parent back into the queue.  When the
queue is down to one element, that's the root of the optimal prefix
tree.

A pre-order DFS reads the codes off the tree, with `0` meaning
"go left" and `1` meaning "go right".  The text is then encoded
into the resulting bitstream and printed.

### 6. `optimal-bst/`

`optimal_bst.cpp` — the textbook dynamic programme for **optimal
binary-search-tree construction** with weighted access probabilities
(CLRS Ch. 15.5 / Knuth 1971).  Fills a 2-D `cost[i][j]` table where
`cost[i][j]` is the minimum expected access cost of any BST built
from the keys in positions `i..j`, given the access probabilities.

Recurrence:

```
cost[i][j] = min over r in [i..j] of
                cost[i][r-1] + cost[r+1][j] + weight[i][j]
```

Time **O(N³)** with the obvious table-fill order (improvable to
**O(N²)** with Knuth's quadrangle-inequality observation, left as
extension).

---

## Building & running

Every folder is single-file or two-file with no build system.
Compile with:

```bash
g++ -std=c++17 -O2 -Wall <folder>/<name>.cpp -o <name>
./<name>
```

For example:

```bash
g++ -std=c++17 -O2 -Wall radix-sort-and-disjoint-sets/radix_sort.cpp -o radix_sort
./radix_sort
# input:  170, 45, 75, 90, 802, 24, 2, 66
# sorted: 2, 24, 45, 66, 75, 90, 170, 802
# 10 000-element random sort sanity check: OK
```

A trivial helper to compile everything:

```bash
for src in */**.cpp; do
  out="bin/$(basename ${src%.cpp})"
  mkdir -p bin
  g++ -std=c++17 -O2 -Wall "$src" -o "$out"
done
```

---

## Repository layout

```
cpp-algorithms-portfolio/
├── radix-sort-and-disjoint-sets/
│   ├── radix_sort.cpp           # LSD radix sort + 10k-element check
│   └── disjoint_sets.cpp        # DSU with union-by-rank + path compression
│
├── topological-sort/
│   └── topological_sort.cpp     # BFS / Kahn's algorithm on a curriculum
│
├── maze-generator-union-find/
│   └── maze.cpp                 # 5×5 perfect maze via Kruskal + DSU
│
├── kruskal-mst/
│   └── kruskal_mst.cpp          # Kruskal's + Prim's on a 10-vertex graph
│
├── huffman-coding/
│   └── huffman.cpp              # Optimal prefix-code tree via priority queue
│
├── optimal-bst/
│   └── optimal_bst.cpp          # CLRS dynamic-programme on weighted keys
│
├── README.md
└── LICENSE
```

---

## License

[MIT](LICENSE) — code is free for commercial, academic and educational use.
