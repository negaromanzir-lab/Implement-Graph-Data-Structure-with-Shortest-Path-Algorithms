# Implement Graph Data Structure with Shortest Path Algorithms

A C++17 implementation of a weighted graph data structure with Dijkstra's single-source shortest path algorithm.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Project Structure](#project-structure)
- [Building the Project](#building-the-project)
- [Running the Program](#running-the-program)
- [Graph Data Structure](#graph-data-structure)
- [Dijkstra's Algorithm](#dijkstras-algorithm)
- [Complexity Summary](#complexity-summary)
- [Algorithm Assumptions and Limitations](#algorithm-assumptions-and-limitations)
- [Documentation](#documentation)
- [References](#references)

---

## Project Overview

This project implements:

1. **A weighted graph** (directed or undirected) backed by an **adjacency list**.
2. **Dijkstra's algorithm** for computing single-source shortest paths using a **binary min-heap priority queue**.
3. **Path reconstruction** using a predecessor (parent) array.
4. An **interactive console interface** for building graphs and querying shortest paths.

**Language:** C++17  
**Build System:** CMake

---

## Project Structure

```
.
├── CMakeLists.txt
├── README.md
├── docs/
│   ├── technical-documentation.md   ← Full technical documentation (Issue #5)
│   ├── algorithm-analysis.md        ← Dijkstra design and edge-case analysis
│   ├── complexity-analysis.md       ← Time and space complexity details
│   └── testing-results.md           ← Test results and coverage
├── include/
│   ├── Graph.h                      ← Graph class declaration
│   ├── Dijkstra.h                   ← Dijkstra class + DijkstraResult struct
│   ├── PathReconstruction.h         ← Path reconstruction utilities
│   └── ConsoleInterface.h           ← Console interface declaration
├── src/
│   ├── main.cpp                     ← Interactive console entry point
│   ├── Graph.cpp                    ← Graph implementation
│   ├── Dijkstra.cpp                 ← Dijkstra's algorithm implementation
│   ├── PathReconstruction.cpp       ← Path reconstruction implementation
│   └── ConsoleInterface.cpp         ← Console interface implementation
├── examples/
└── tests/
```

---

## Building the Project

**Prerequisites:** CMake ≥ 3.14, a C++17-compatible compiler (GCC ≥ 7, Clang ≥ 5, MSVC ≥ 2017).

```bash
# From the project root directory:
cmake -S . -B build
cmake --build build
```

The compiled executable will be located in `build/` (or `build/Debug/` on Windows with MSVC).

---

## Running the Program

```bash
./build/GraphShortestPath
```

The interactive console will prompt you to:

1. Enter the number of vertices.
2. Choose directed or undirected.
3. Enter edges (source, destination, weight).
4. Choose source and destination vertices.
5. View the shortest path and total distance.

---

## Graph Data Structure

### Representation: Adjacency List

The `Graph` class (`include/Graph.h`, `src/Graph.cpp`) stores the graph as:

```cpp
std::vector<std::vector<Graph::Edge>> adjacencyList_;
```

Each `Graph::Edge` holds:
- `int to` — destination vertex index
- `double weight` — non-negative edge weight

**Key properties:**
- Vertices are indexed from `0` to `V − 1`.
- Both **directed** and **undirected** graphs are supported.
- For undirected graphs, `addEdge(u, v, w)` automatically stores both `u→v` and `v→u`.
- Edge weights must be **finite, non-negative integers** (enforced at runtime).

### Key Operations

| Method               | Time Complexity    | Description                             |
|----------------------|--------------------|-----------------------------------------|
| `addVertex()`        | O(1) amortised     | Appends an isolated vertex              |
| `addEdge(u, v, w)`   | O(1) amortised     | Inserts a weighted edge                 |
| `getNeighbors(u)`    | O(1)               | Returns reference to u's edge list      |
| `hasEdge(u, v)`      | O(deg(u))          | Checks if edge u→v exists               |
| `getEdgeWeight(u, v)`| O(deg(u))          | Returns weight of first u→v edge        |
| `vertexCount()`      | O(1)               | Returns number of vertices              |
| `edgeCount()`        | O(1)               | Returns number of logical edges         |

### Storage Complexity

| Representation    | Space    | Used in this project |
|-------------------|----------|----------------------|
| Adjacency List    | O(V + E) | ✅ Yes               |
| Adjacency Matrix  | O(V²)    | ❌ No                |

---

## Dijkstra's Algorithm

The `Dijkstra` class (`include/Dijkstra.h`, `src/Dijkstra.cpp`) implements single-source shortest path using a **binary min-heap** (`std::priority_queue` with `std::greater`).

### Usage

```cpp
Graph g(5, true); // 5-vertex directed graph
g.addEdge(0, 1, 10);
g.addEdge(0, 2, 3);
g.addEdge(2, 1, 4);
g.addEdge(1, 3, 2);
g.addEdge(2, 4, 2);
g.addEdge(4, 3, 1);

DijkstraResult result = Dijkstra::shortestPath(g, 0 /*source*/);

// Query shortest distance
long long dist = result.getDistanceTo(3); // → 6

// Reconstruct path
std::vector<int> path = result.getPathTo(3); // → [0, 2, 4, 3]

// Check reachability
bool reachable = result.hasPathTo(3); // → true
```

### Algorithm Overview

1. Initialise `d[source] = 0`, `d[v] = ∞` for all other vertices.
2. Push `(0, source)` onto the min-heap.
3. While the heap is not empty:
   - Extract the vertex `u` with the smallest tentative distance.
   - Skip if already visited (lazy deletion of stale entries).
   - For each neighbour `v` of `u`: if `d[u] + w(u,v) < d[v]`, update `d[v]` and push `(d[v], v)`.
4. Return distance and parent arrays.

### Why a Binary Min-Heap?

Using a binary min-heap (`std::priority_queue`) gives **O((V + E) log V)** overall — optimal for the adjacency-list representation. An adjacency matrix combined with a linear scan for the minimum gives O(V²), which is slower for sparse graphs.

---

## Complexity Summary

> **Notation:** V = number of vertices, E = number of (logical) edges.

### Time Complexity

| Operation                              | Complexity          |
|----------------------------------------|---------------------|
| Graph construction (inserting V + E)  | O(V + E)            |
| Edge insertion with adjacency list     | O(1) amortised      |
| Dijkstra's algorithm (binary min-heap) | O((V + E) log V)    |
| Path reconstruction                    | O(V)                |
| **End-to-end**                         | **O((V + E) log V)** |

### Space Complexity

| Structure                             | Space       |
|---------------------------------------|-------------|
| Adjacency list storage                | O(V + E)    |
| Dijkstra auxiliary (dist, parent, pq) | O(V + E)    |
| Path reconstruction                   | O(V)        |
| **Overall**                           | **O(V + E)** |

For comparison, an adjacency matrix would use **O(V²)** space, and Dijkstra with a matrix scan would run in **O(V²)** time.

---

## Algorithm Assumptions and Limitations

### Assumptions

- **Non-negative edge weights:** Dijkstra's algorithm requires all weights ≥ 0. This implementation enforces it and throws `std::invalid_argument` for negative or non-finite weights.
- **Whole-number weights only:** Edge weights stored as `double` are converted to `long long` internally. Fractional weights (e.g., 1.5) are rejected.
- **Integer overflow protection:** Distances exceeding `std::numeric_limits<long long>::max()` are detected and skipped.
- **Disconnected graphs:** Supported. Unreachable vertices have distance `Dijkstra::INF` and no path.

### Limitations

| Limitation                       | Description                                                              |
|----------------------------------|--------------------------------------------------------------------------|
| No negative-weight edges         | Use Bellman-Ford for graphs with negative weights                        |
| No fractional weights            | Implementation converts to `long long`; fractional values are rejected   |
| No decrease-key operation        | Lazy deletion used; heap may hold O(V + E) stale entries                 |
| Single-source only               | For all-pairs shortest paths, run Dijkstra V times                       |
| No Fibonacci heap                | Binary heap gives O((V+E) log V); Fibonacci heap gives O(E + V log V)   |

---

## Documentation

Full technical documentation for Issue #5 is available in the `docs/` directory:

| Document                                                              | Contents                                                                 |
|-----------------------------------------------------------------------|--------------------------------------------------------------------------|
| [`docs/technical-documentation.md`](docs/technical-documentation.md) | Complete technical report: graph theory, representations, Dijkstra's algorithm, complexity analysis, worked example, pseudocode, and references |
| [`docs/algorithm-analysis.md`](docs/algorithm-analysis.md)           | Dijkstra design decisions, priority queue details, and edge-case handling |
| [`docs/complexity-analysis.md`](docs/complexity-analysis.md)         | Detailed time and space complexity derivations                           |
| [`docs/testing-results.md`](docs/testing-results.md)                 | Test suite results and coverage                                          |

---

## References

1. Dijkstra, E. W. (1959). "A note on two problems in connexion with graphs." *Numerische Mathematik*, 1(1), 269–271.
2. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to Algorithms* (4th ed.). MIT Press. Ch. 22, 24.
3. Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley. Part 4.
4. ISO/IEC 14882:2017. *Programming Languages — C++* (C++17 Standard).
5. cppreference.com (2024). `std::priority_queue`. https://en.cppreference.com/w/cpp/container/priority_queue
6. cppreference.com (2024). `std::vector`. https://en.cppreference.com/w/cpp/container/vector
7. Skiena, S. S. (2008). *The Algorithm Design Manual* (2nd ed.). Springer. Ch. 6.
