# Technical Documentation: Graph Data Structure and Dijkstra's Shortest Path Algorithm

**Project:** Implement Graph Data Structure with Shortest Path Algorithms  
**Issue:** #5 — Complexity Analysis and Documentation  
**Branch:** `bini`  
**Language:** C++17  

---

## Table of Contents

1. [Introduction to Graph Data Structures](#1-introduction-to-graph-data-structures)
2. [Vertices and Edges](#2-vertices-and-edges)
3. [Weighted and Unweighted Graphs](#3-weighted-and-unweighted-graphs)
4. [Directed and Undirected Graphs](#4-directed-and-undirected-graphs)
5. [Adjacency List Representation](#5-adjacency-list-representation)
6. [Adjacency Matrix Representation](#6-adjacency-matrix-representation)
7. [Comparison: Adjacency List vs. Adjacency Matrix](#7-comparison-adjacency-list-vs-adjacency-matrix)
8. [Dijkstra's Shortest Path Algorithm](#8-dijkstras-shortest-path-algorithm)
9. [Step-by-Step Algorithm Execution](#9-step-by-step-algorithm-execution)
10. [Priority Queue Explanation](#10-priority-queue-explanation)
11. [Path Reconstruction](#11-path-reconstruction)
12. [Time Complexity Analysis](#12-time-complexity-analysis)
13. [Space Complexity Analysis](#13-space-complexity-analysis)
14. [Algorithm Assumptions and Limitations](#14-algorithm-assumptions-and-limitations)
15. [References](#15-references)

---

## 1. Introduction to Graph Data Structures

A **graph** is a non-linear data structure used to model relationships between objects. It is defined as an ordered pair G = (V, E), where:

- **V** is a finite, non-empty set of **vertices** (also called nodes), representing the objects.
- **E** is a set of **edges**, representing relationships or connections between pairs of vertices.

Graphs are one of the most versatile and widely studied abstractions in computer science. They are used to model:

- **Road networks** — where intersections are vertices and roads are edges.
- **Social networks** — where people are vertices and friendships are edges.
- **Computer networks** — where machines are vertices and links are edges.
- **Dependency graphs** — where tasks are vertices and ordering constraints are edges.
- **Maps and navigation systems** — the core use-case of this project.

This project implements a weighted graph as an **adjacency list** using the `Graph` class defined in [`include/Graph.h`](../include/Graph.h) and [`src/Graph.cpp`](../src/Graph.cpp). The implementation supports both directed and undirected graphs and provides an interface for Dijkstra's single-source shortest path algorithm.

---

## 2. Vertices and Edges

### 2.1 Vertices (V)

A **vertex** (plural: vertices) is a fundamental unit of a graph representing an entity or an object. In this implementation:

- Vertices are identified by non-negative integer indices in the range **[0, V − 1]**, where **V** is the total number of vertices.
- Vertices are stored implicitly as indices into the `adjacencyList_` vector inside the `Graph` class.
- New isolated vertices can be added dynamically using `Graph::addVertex()`.

**Notation:** Throughout this document, **V** denotes the total number of vertices in the graph.

### 2.2 Edges (E)

An **edge** is a connection between two vertices. In this implementation, an edge is represented by the `Graph::Edge` struct:

```cpp
struct Edge {
    int    to;     // Destination vertex index
    double weight; // Non-negative edge weight
};
```

Edges are stored in the adjacency list of their source vertex. Important properties:

- Edges must have **finite, non-negative weights** (enforced by `Graph::addEdge`).
- **Self-loops** (an edge from a vertex to itself) are permitted.
- **Parallel edges** (multiple edges between the same ordered pair) are permitted.
- For undirected graphs, each logical edge is stored as two directed entries in the adjacency lists (one in each direction).

**Notation:** Throughout this document, **E** denotes the total number of **logical edges** in the graph. For an undirected graph, while each logical edge produces two adjacency-list entries, it is counted as one edge in `Graph::edgeCount()`.

---

## 3. Weighted and Unweighted Graphs

### 3.1 Weighted Graphs

A **weighted graph** assigns a numerical value (weight) to each edge. The weight can represent:

- Distance or length (e.g., kilometres between cities)
- Cost (e.g., travel fare)
- Capacity (e.g., bandwidth of a network link)
- Time (e.g., travel duration)

This project implements a **weighted graph**. Edge weights are stored as `double` in the `Graph::Edge` struct. Dijkstra's algorithm uses these weights to compute shortest paths by minimising the total sum of edge weights along a path.

### 3.2 Unweighted Graphs

An **unweighted graph** treats all edges as equal (weight = 1). Shortest paths in unweighted graphs are measured by the number of edges traversed, and are typically found using Breadth-First Search (BFS) in O(V + E) time. This project does not implement BFS, but all edges with equal weight produce equivalent results to an unweighted traversal.

---

## 4. Directed and Undirected Graphs

### 4.1 Directed Graphs (Digraphs)

In a **directed graph**, every edge has an explicit direction from a source vertex to a destination vertex. An edge (u → v) does not imply the existence of edge (v → u).

The `Graph` class is constructed with `directed = true` (the default):

```cpp
Graph g(5, true); // 5-vertex directed graph
g.addEdge(0, 1, 4.0); // edge from 0 to 1 only
```

Use cases: web hyperlinks, one-way streets, task scheduling.

### 4.2 Undirected Graphs

In an **undirected graph**, every edge is bidirectional. An edge between u and v can be traversed in either direction.

The `Graph` class supports undirected graphs via `directed = false`. Internally, `addEdge(u, v, w)` stores two directed entries: (u → v) and (v → u):

```cpp
Graph g(5, false); // 5-vertex undirected graph
g.addEdge(0, 1, 4.0); // stores both 0→1 and 1→0
```

Use cases: friendship networks, undirected road maps, communication links.

### 4.3 Implementation Detail

The `Graph::directed_` private member records the graph's directedness. When `directed_ == false` and `from != to`, `addEdge` automatically appends the reverse entry to maintain symmetric adjacency lists. Only one logical edge is counted in `edgeCount_` regardless.

---

## 5. Adjacency List Representation

### 5.1 Definition

An **adjacency list** represents a graph by storing, for each vertex u, a list of all vertices v such that an edge (u, v) exists, along with any associated edge data (e.g., weight).

### 5.2 Implementation in This Project

The `Graph` class uses the following internal representation:

```cpp
// From include/Graph.h
std::vector<std::vector<Edge>> adjacencyList_;
```

Each element `adjacencyList_[u]` is a `std::vector<Edge>` containing all outgoing edges from vertex u. Each `Edge` holds:
- `to` — the destination vertex index
- `weight` — the non-negative edge weight

**Example** — the following directed, weighted graph:

```
Vertices: {0, 1, 2, 3}
Edges: 0→1 (w=4), 0→2 (w=1), 1→3 (w=1), 2→1 (w=2), 2→3 (w=5)
```

Is stored as:

```
adjacencyList_[0] = [ {to=1, w=4}, {to=2, w=1} ]
adjacencyList_[1] = [ {to=3, w=1} ]
adjacencyList_[2] = [ {to=1, w=2}, {to=3, w=5} ]
adjacencyList_[3] = []
```

### 5.3 Storage Complexity

An adjacency list stores exactly one entry per directed edge (or two per undirected logical edge). The total storage is:

| Component                   | Space     |
|-----------------------------|-----------|
| Vertex array (outer vector) | O(V)      |
| All edge entries (inner)    | O(E)      |
| **Total**                   | **O(V + E)** |

For a directed graph with V = 4 and E = 5, the adjacency list stores exactly 4 + 5 = 9 elements.

### 5.4 Key Operations

| Operation          | Time Complexity  | Notes                                      |
|--------------------|------------------|--------------------------------------------|
| `addVertex()`      | O(1) amortised   | Appends an empty vector                    |
| `addEdge(u, v, w)` | O(1) amortised   | `push_back` to `adjacencyList_[u]`         |
| `getNeighbors(u)`  | O(1)             | Returns reference to `adjacencyList_[u]`   |
| `hasEdge(u, v)`    | O(deg(u))        | Linear scan of u's neighbour list          |
| `getEdgeWeight(u,v)` | O(deg(u))      | Linear scan of u's neighbour list          |

> **Note on `addEdge` amortised cost:** `std::vector::push_back` is O(1) amortised, though occasional resizing is O(n). Over a sequence of E insertions, the total cost is O(E), so each insertion averages O(1).

---

## 6. Adjacency Matrix Representation

### 6.1 Definition

An **adjacency matrix** represents a graph as a V × V 2D array M, where:

- `M[u][v] = w` if an edge from u to v with weight w exists.
- `M[u][v] = 0` (or ∞) if no edge exists from u to v.

### 6.2 Conceptual Illustration

For the same example graph (directed, weighted):

```
Edges: 0→1 (w=4), 0→2 (w=1), 1→3 (w=1), 2→1 (w=2), 2→3 (w=5)
```

The adjacency matrix would be:

```
     0    1    2    3
0 [  0    4    1    0  ]
1 [  0    0    0    1  ]
2 [  0    2    0    5  ]
3 [  0    0    0    0  ]
```

(0 indicates no edge; in weighted graphs, ∞ or a sentinel value is more typical.)

### 6.3 Storage Complexity

An adjacency matrix always allocates V × V cells, regardless of the number of edges:

| Component             | Space     |
|-----------------------|-----------|
| V × V matrix          | O(V²)     |
| **Total**             | **O(V²)** |

For V = 1000, the matrix requires 10⁶ cells — consuming approximately 8 MB for `double` entries. For V = 10,000, it would require 800 MB.

### 6.4 Key Operations

| Operation          | Time Complexity | Notes                              |
|--------------------|-----------------|-------------------------------------|
| Add edge (u, v)    | O(1)            | Direct array write: M[u][v] = w     |
| Check edge (u, v)  | O(1)            | Direct array read: M[u][v] ≠ 0      |
| Get edge weight    | O(1)            | Direct array read: M[u][v]          |
| Enumerate neighbours of u | O(V)   | Full row scan required              |

### 6.5 Project Implementation Note

This project does **not** implement an adjacency matrix class. The `Graph` class uses an **adjacency list** exclusively. The adjacency matrix is described here as a conceptual alternative for comparison and educational purposes, as required by the project's documentation requirements.

---

## 7. Comparison: Adjacency List vs. Adjacency Matrix

| Property                       | Adjacency List       | Adjacency Matrix     |
|--------------------------------|----------------------|----------------------|
| **Storage space**              | O(V + E)             | O(V²)                |
| **Add edge**                   | O(1) amortised       | O(1)                 |
| **Check edge existence**       | O(deg(u))            | O(1)                 |
| **Enumerate all neighbours**   | O(deg(u))            | O(V)                 |
| **Enumerate all edges**        | O(V + E)             | O(V²)                |
| **Memory for sparse graphs**   | ✅ Efficient          | ❌ Wasteful           |
| **Memory for dense graphs**    | ⚠️ Slight overhead   | ✅ Compact            |
| **Dijkstra's algorithm**       | O((V+E) log V)       | O(V²)                |
| **Implementation used**        | ✅ **This project**   | ❌ Not implemented    |

### 7.1 Why Adjacency Lists Are Preferred Here

Dijkstra's algorithm iterates over all neighbours of each settled vertex. With an adjacency list, this takes O(deg(u)) per vertex — proportional to the actual number of edges. With a matrix, it takes O(V) per vertex regardless of degree, leading to overall O(V²) for the algorithm.

For the typical use-case of this project (road-like, sparse graphs where E ≪ V²), adjacency lists provide significantly better asymptotic performance. The `std::vector<std::vector<Edge>>` representation is cache-friendly for sequential neighbour iteration and grows dynamically to accommodate any graph size.

---

## 8. Dijkstra's Shortest Path Algorithm

### 8.1 Overview

**Dijkstra's algorithm** (Dijkstra, 1959) is a **greedy** algorithm that solves the **Single-Source Shortest Path (SSSP)** problem for graphs with **non-negative edge weights**. Given:

- A weighted graph G = (V, E) with non-negative weights w(u, v) ≥ 0.
- A source vertex s ∈ V.

The algorithm computes the shortest-path distance d[v] from s to every vertex v ∈ V, and constructs a **shortest-path tree** via a predecessor (parent) array.

### 8.2 Core Invariant

At any point during execution, the set **S** of "settled" (finalised) vertices holds the property:

> For every vertex u ∈ S, d[u] is the true shortest-path distance from s to u.

This invariant is maintained by always extracting the unvisited vertex with the smallest tentative distance — implemented via a **binary min-heap priority queue**.

### 8.3 Why Dijkstra Does Not Support Negative-Weight Edges

Dijkstra's correctness relies on the **greedy choice property**: once a vertex u is settled with distance d[u], no future relaxation can produce a shorter path to u. This holds because all edge weights are non-negative, so any path through an unvisited vertex must be at least as long as d[u].

With a **negative-weight edge** (u, v, −k), relaxing v via u could later reduce d[u] itself — meaning the already-settled distance d[u] was incorrect. This would invalidate the invariant and corrupt the algorithm's output.

**Example of failure:**
```
Vertices: {0, 1, 2}
Edges: 0→1 (w=5), 0→2 (w=3), 2→1 (w=-4)
```
Dijkstra settles vertex 2 with d[2] = 3, then settles vertex 1 with d[1] = 5. But the true shortest path is 0 → 2 → 1 with cost 3 + (−4) = −1. Dijkstra computes the wrong answer.

**This implementation** rejects negative weights explicitly:
```cpp
// From src/Dijkstra.cpp
if (!std::isfinite(edge.weight) || edge.weight < 0.0) {
    throw std::invalid_argument(
        "Dijkstra: non-finite or negative edge weight ...");
}
```

For graphs with negative-weight edges, **Bellman-Ford** (O(VE)) or **SPFA** should be used instead.

### 8.4 Pseudocode

```
function Dijkstra(Graph G, source s):
    for each vertex v in G:
        d[v] ← ∞
        parent[v] ← -1
        visited[v] ← false

    d[s] ← 0
    PQ ← MinPriorityQueue()
    PQ.insert((0, s))

    while PQ is not empty:
        (currentDist, u) ← PQ.extractMin()

        if visited[u]:
            continue          // stale entry, skip
        visited[u] ← true

        for each edge (u, v, w) in G.neighbours(u):
            candidate ← d[u] + w
            if candidate < d[v]:
                d[v] ← candidate
                parent[v] ← u
                PQ.insert((candidate, v))

    return (d, parent)
```

---

## 9. Step-by-Step Algorithm Execution

### 9.1 Worked Example

Consider the following weighted directed graph with 5 vertices (0–4):

```
Vertices: {0, 1, 2, 3, 4}
Edges (directed, weighted):
  0 → 1  (w = 10)
  0 → 2  (w = 3)
  1 → 3  (w = 2)
  2 → 1  (w = 4)
  2 → 3  (w = 8)
  2 → 4  (w = 2)
  3 → 4  (w = 5)
  4 → 3  (w = 1)
```

**Goal:** Find shortest paths from source vertex **0** to all other vertices.

### 9.2 Initial State

| Vertex | d[v] | parent[v] | visited |
|--------|------|-----------|---------|
| 0      | 0    | −1        | false   |
| 1      | ∞    | −1        | false   |
| 2      | ∞    | −1        | false   |
| 3      | ∞    | −1        | false   |
| 4      | ∞    | −1        | false   |

Priority Queue (min-heap): `[(0, 0)]`

---

### 9.3 Iteration 1 — Extract vertex 0 (d = 0)

Mark 0 as visited. Examine neighbours of 0:

- **Edge 0→1 (w=10):** candidate = 0 + 10 = 10 < ∞ → update d[1] = 10, parent[1] = 0. Push (10, 1).
- **Edge 0→2 (w=3):** candidate = 0 + 3 = 3 < ∞ → update d[2] = 3, parent[2] = 0. Push (3, 2).

| Vertex | d[v] | parent[v] | visited |
|--------|------|-----------|---------|
| 0      | 0    | −1        | ✅ true  |
| 1      | 10   | 0         | false   |
| 2      | 3    | 0         | false   |
| 3      | ∞    | −1        | false   |
| 4      | ∞    | −1        | false   |

Priority Queue: `[(3, 2), (10, 1)]`

---

### 9.4 Iteration 2 — Extract vertex 2 (d = 3)

Mark 2 as visited. Examine neighbours of 2:

- **Edge 2→1 (w=4):** candidate = 3 + 4 = 7 < 10 → update d[1] = 7, parent[1] = 2. Push (7, 1).
- **Edge 2→3 (w=8):** candidate = 3 + 8 = 11 < ∞ → update d[3] = 11, parent[3] = 2. Push (11, 3).
- **Edge 2→4 (w=2):** candidate = 3 + 2 = 5 < ∞ → update d[4] = 5, parent[4] = 2. Push (5, 4).

| Vertex | d[v] | parent[v] | visited |
|--------|------|-----------|---------|
| 0      | 0    | −1        | ✅ true  |
| 1      | 7    | 2         | false   |
| 2      | 3    | 0         | ✅ true  |
| 3      | 11   | 2         | false   |
| 4      | 5    | 2         | false   |

Priority Queue: `[(5, 4), (7, 1), (10, 1-stale), (11, 3)]`

---

### 9.5 Iteration 3 — Extract vertex 4 (d = 5)

Mark 4 as visited. Examine neighbours of 4:

- **Edge 4→3 (w=1):** candidate = 5 + 1 = 6 < 11 → update d[3] = 6, parent[3] = 4. Push (6, 3).

| Vertex | d[v] | parent[v] | visited |
|--------|------|-----------|---------|
| 0      | 0    | −1        | ✅ true  |
| 1      | 7    | 2         | false   |
| 2      | 3    | 0         | ✅ true  |
| 3      | 6    | 4         | false   |
| 4      | 5    | 2         | ✅ true  |

Priority Queue: `[(6, 3), (7, 1), (10, 1-stale), (11, 3-stale)]`

---

### 9.6 Iteration 4 — Extract vertex 3 (d = 6)

Mark 3 as visited. Examine neighbours of 3:

- **Edge 3→4 (w=5):** candidate = 6 + 5 = 11 > 5. No update (vertex 4 already settled).

| Vertex | d[v] | parent[v] | visited |
|--------|------|-----------|---------|
| 3      | 6    | 4         | ✅ true  |

Priority Queue: `[(7, 1), (10, 1-stale), (11, 3-stale)]`

---

### 9.7 Iteration 5 — Extract vertex 1 (d = 7)

Mark 1 as visited. Examine neighbours of 1:

- **Edge 1→3 (w=2):** candidate = 7 + 2 = 9 > 6. No update (vertex 3 already settled).

| Vertex | d[v] | parent[v] | visited |
|--------|------|-----------|---------|
| 1      | 7    | 2         | ✅ true  |

Remaining queue entries (10, 1) and (11, 3) are stale — skipped when popped.

---

### 9.8 Final Results

| Vertex | Shortest Distance from 0 | Shortest Path               |
|--------|--------------------------|-----------------------------|
| 0      | 0                        | [0]                         |
| 1      | 7                        | [0 → 2 → 1]                 |
| 2      | 3                        | [0 → 2]                     |
| 3      | 6                        | [0 → 2 → 4 → 3]             |
| 4      | 5                        | [0 → 2 → 4]                 |

---

## 10. Priority Queue Explanation

### 10.1 Role in Dijkstra's Algorithm

The **priority queue** is the central data structure that enables Dijkstra's algorithm to always process the vertex with the **currently smallest tentative distance**. This greedy selection ensures that once a vertex is settled, its distance is final.

### 10.2 Implementation: Binary Min-Heap

This project uses `std::priority_queue` with `std::greater` to implement a **binary min-heap**:

```cpp
// From src/Dijkstra.cpp
using PQElement = std::pair<long long, int>;
std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
```

Each element in the priority queue is a pair `(distance, vertex_index)`. Since `std::pair` comparisons are lexicographic and `std::greater` reverses the order, the element with the **smallest distance** is always at the top.

### 10.3 Lazy Deletion Strategy

This implementation uses a **lazy deletion** (or "insert and skip") approach:

- When a vertex's distance is improved, a new `(newDist, v)` pair is **pushed** without removing the old entry.
- When an entry is popped, the `visited[]` array is checked. If the vertex is already settled, the entry is **discarded** in O(1):

```cpp
if (visited[u]) {
    continue; // Stale entry — skip
}
visited[u] = true;
```

This avoids the need for a `decrease-key` operation (which `std::priority_queue` does not support). The trade-off is that the heap may hold up to O(V + E) entries in the worst case.

### 10.4 Priority Queue Operations and Cost

| Operation       | Binary Heap Cost | Occurrences       | Total Cost  |
|-----------------|------------------|-------------------|-------------|
| `push` (insert) | O(log(V + E))    | At most E times   | O(E log V)  |
| `pop` (extract) | O(log(V + E))    | At most V + E times | O((V+E) log V) |
| `top` (peek)    | O(1)             | At most V + E times | O(V + E)  |

> Since E ≤ V² for simple graphs, log(V + E) = O(log V). All heap sizes are bounded by O(V + E).

---

## 11. Path Reconstruction

### 11.1 Predecessor Array

Dijkstra's algorithm maintains a **parent array** (predecessor array) `parent[v]` that records which vertex immediately precedes v on its current shortest path from the source:

```cpp
// From include/Dijkstra.h
std::vector<int> parent; // parent[v] = predecessor of v on shortest path
```

Initialisation:
- `parent[source] = −1` (source has no predecessor)
- `parent[v] = −1` for all v (unreachable by default)

Update during relaxation:
```cpp
// From src/Dijkstra.cpp
if (candidateDist < result.distances[v]) {
    result.distances[v] = candidateDist;
    result.parent[v] = u;   // <-- record predecessor
    pq.emplace(candidateDist, v);
}
```

### 11.2 Reconstruction Algorithm

The `PathReconstruction::reconstructPath` function (defined in [`src/PathReconstruction.cpp`](../src/PathReconstruction.cpp)) traces the path **backwards** from the destination to the source using the parent array, then reverses the result:

```cpp
// Simplified from src/PathReconstruction.cpp
std::vector<int> path;
int current = destination;

while (current != -1) {
    path.push_back(current);
    if (current == source) break;
    current = parent[current];
}

std::reverse(path.begin(), path.end()); // forward order: source → ... → destination
```

The function also includes **cycle detection** via a `visited` boolean array to guard against corrupted parent-pointer states.

### 11.3 Complexity

| Step                                  | Time Complexity |
|---------------------------------------|-----------------|
| Backward traversal (destination → source) | O(L) where L = path length ≤ V |
| `std::reverse`                        | O(L)            |
| **Total**                             | **O(V)**        |

### 11.4 Usage Example

Using the worked example from Section 9, to find the path from vertex 0 to vertex 3:

```
parent = [-1, 2, 0, 4, 2]

Trace from 3:
  3 → parent[3] = 4
  4 → parent[4] = 2
  2 → parent[2] = 0
  0 → parent[0] = -1 (source reached)

Reversed path: [0, 2, 4, 3]
Total distance: d[3] = 6
```

---

## 12. Time Complexity Analysis

Let **V** = number of vertices, **E** = number of (logical) edges.

### 12.1 Graph Construction

| Operation                | Time Complexity  | Implementation Detail                       |
|--------------------------|------------------|---------------------------------------------|
| `Graph(V)`               | O(V)             | Initialises V empty vectors                 |
| `addVertex()`            | O(1) amortised   | `emplace_back()` on outer vector            |
| `addEdge(u, v, w)`       | O(1) amortised   | `push_back()` on `adjacencyList_[u]`        |
| Inserting all E edges    | **O(E)** total   | E × O(1) amortised                          |

### 12.2 Dijkstra's Algorithm

#### Initialisation

- Allocating and filling `distances` and `parent` arrays: **O(V)**
- Validating all edges (pre-check loop): **O(V + E)**
- Inserting source into priority queue: **O(log 1) = O(1)**

#### Main Loop

| Sub-operation                        | Per-occurrence cost | Total occurrences | Total cost    |
|--------------------------------------|---------------------|-------------------|---------------|
| Extract-min from priority queue      | O(log(V+E)) = O(log V) | ≤ V + E         | O((V+E) log V) |
| Visited check                        | O(1)                | ≤ V + E           | O(V + E)      |
| Edge relaxation (examine neighbours) | O(1) per edge       | E total           | O(E)          |
| Push to priority queue               | O(log(V+E)) = O(log V) | ≤ E            | O(E log V)    |

#### Total Time Complexity of Dijkstra

$$\mathcal{O}((V + E) \log V)$$

This is derived as follows:
- The priority queue holds at most **O(V + E)** elements.
- Each pop and push costs **O(log(V + E)) = O(log V)**.
- Pops occur at most V + E times (V vertices, up to E extra stale entries).
- Pushes occur at most E times (one per relaxation).

For **connected graphs** where E ≥ V − 1, this simplifies to **O(E log V)**.  
For **sparse graphs** where E = O(V), the complexity is **O(V log V)**.

### 12.3 Path Reconstruction

| Step                              | Time Complexity |
|-----------------------------------|-----------------|
| `PathReconstruction::reconstructPath` | O(V)        |
| `std::reverse`                    | O(V)            |
| **Total**                         | **O(V)**        |

### 12.4 Overall Summary

| Phase                      | Time Complexity     |
|----------------------------|---------------------|
| Graph construction (V + E) | O(V + E)            |
| Dijkstra's algorithm       | O((V + E) log V)    |
| Path reconstruction        | O(V)                |
| **End-to-end**             | **O((V + E) log V)** |

---

## 13. Space Complexity Analysis

### 13.1 Graph Storage

| Structure                     | Space       | Description                                 |
|-------------------------------|-------------|---------------------------------------------|
| Outer vector (`adjacencyList_`) | O(V)      | One slot per vertex                         |
| All inner edge vectors (directed) | O(E)    | One entry per directed edge                 |
| All inner edge vectors (undirected) | O(2E) = O(E) | Two entries per logical edge         |
| **Adjacency list total**      | **O(V + E)** |                                             |

Compare with adjacency matrix: **O(V²)** — prohibitive for large sparse graphs.

### 13.2 Dijkstra's Algorithm Auxiliary Space

| Structure                 | Space       | Description                                            |
|---------------------------|-------------|--------------------------------------------------------|
| `distances` array         | O(V)        | One `long long` per vertex                             |
| `parent` array            | O(V)        | One `int` per vertex                                   |
| `visited` array           | O(V)        | One `bool` per vertex                                  |
| Priority queue (`pq`)     | O(V + E)    | At most V + E elements (lazy deletion, no decrease-key) |
| **Algorithm total**       | **O(V + E)** |                                                       |

### 13.3 Path Reconstruction Auxiliary Space

| Structure        | Space  | Description                                  |
|------------------|--------|----------------------------------------------|
| `path` vector    | O(V)   | At most V vertices on the reconstructed path |
| `visited` array  | O(V)   | Cycle-detection guard                        |
| **Total**        | **O(V)** |                                            |

### 13.4 Overall Space Complexity

| Component                       | Space        |
|---------------------------------|--------------|
| Graph storage (adjacency list)  | O(V + E)     |
| Dijkstra auxiliary structures   | O(V + E)     |
| Path reconstruction             | O(V)         |
| **Total**                       | **O(V + E)** |

---

## 14. Algorithm Assumptions and Limitations

### 14.1 Assumptions

1. **Non-negative edge weights:** All edge weights must be finite and ≥ 0. This is enforced in both `Graph::addEdge` and `Dijkstra::shortestPath` via `std::invalid_argument` exceptions.

2. **Whole-number weights only (implementation constraint):** The Dijkstra implementation converts edge weights from `double` to `long long` for distance arithmetic. Fractional weights (e.g., 1.5) are **rejected** with `std::invalid_argument`:
   ```cpp
   if (std::trunc(edge.weight) != edge.weight) {
       throw std::invalid_argument(
           "Dijkstra: edge weights must be whole numbers...");
   }
   ```
   This is a deliberate project constraint, not a theoretical requirement of Dijkstra's algorithm.

3. **Integer overflow protection:** Distances are stored as `long long`. Before computing `d[u] + w`, the implementation checks:
   ```cpp
   if (currentDist > Dijkstra::INF - weight) continue; // overflow guard
   ```

4. **Connected or disconnected graphs:** Unreachable vertices are supported — they retain `d[v] = Dijkstra::INF` and `parent[v] = −1`. Query methods (`hasPathTo`, `getPathTo`) handle this gracefully.

5. **Self-loops and parallel edges:** Both are permitted by the `Graph` class. Self-loops have no effect on shortest-path results (a self-loop on u with weight w ≥ 0 never improves d[u] = 0 from source u itself). Parallel edges are handled correctly — only the minimum-weight entry among parallel edges will be used for relaxation.

### 14.2 Limitations

1. **No support for negative-weight edges:** As explained in Section 8.3, Dijkstra's algorithm is incorrect for graphs with negative-weight edges. Use Bellman-Ford for such cases.

2. **No decrease-key operation:** `std::priority_queue` does not support key reduction. The lazy-deletion approach means stale entries accumulate. In graphs with many relaxations (dense graphs), this increases memory usage to O(V + E) in the queue.

3. **Fractional weights not supported:** Only integer-valued weights are accepted (see Assumption 2 above). This limits use cases where weights are inherently fractional (e.g., probabilities, fractional distances).

4. **Single-source only:** The current implementation computes shortest paths from one source vertex. All-pairs shortest paths require running Dijkstra V times: O(V(V + E) log V), or using Floyd-Warshall O(V³).

5. **No real-time graph mutation during queries:** The implementation does not support incremental updates to the graph while Dijkstra is in progress.

6. **No Fibonacci heap:** The theoretically optimal complexity of Dijkstra is O(E + V log V) using a Fibonacci heap (amortised O(1) `decrease-key`). This project uses a standard binary min-heap, giving O((V + E) log V). The difference matters only for very dense graphs.

---

## 15. References

The following references were used in preparing this documentation. All references are real, verifiable, and academically established.

---

**[1] Dijkstra, E. W. (1959).** "A note on two problems in connexion with graphs." *Numerische Mathematik*, 1(1), 269–271.  
The original paper introducing Dijkstra's algorithm for shortest paths in weighted graphs.

---

**[2] Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022).** *Introduction to Algorithms* (4th ed.). MIT Press.  
Chapter 22 (Elementary Graph Algorithms) and Chapter 24 (Single-Source Shortest Paths). The definitive reference for algorithm complexity analysis and graph data structures. The O((V + E) log V) analysis of Dijkstra with a binary heap is established in Section 24.3.

---

**[3] Sedgewick, R., & Wayne, K. (2011).** *Algorithms* (4th ed.). Addison-Wesley.  
Part 4 (Graphs) covers adjacency list and adjacency matrix representations, weighted digraphs, and shortest-path algorithms. Provides accessible treatment of priority-queue-based Dijkstra.

---

**[4] ISO/IEC 14882:2017.** *Programming Languages — C++* (C++17 Standard).  
Specifies the behaviour of `std::priority_queue`, `std::vector`, `std::pair`, and `std::greater` used in this implementation.

---

**[5] cppreference.com (2024).** `std::priority_queue` — C++ reference.  
Retrieved from https://en.cppreference.com/w/cpp/container/priority_queue  
Documents the interface, complexity guarantees, and behaviour of the binary heap container used in `Dijkstra.cpp`.

---

**[6] cppreference.com (2024).** `std::vector` — C++ reference.  
Retrieved from https://en.cppreference.com/w/cpp/container/vector  
Documents amortised O(1) `push_back` and O(1) `emplace_back`, the basis for the O(1) amortised `addEdge` and `addVertex` claims.

---

**[7] Skiena, S. S. (2008).** *The Algorithm Design Manual* (2nd ed.). Springer.  
Chapter 6 (Weighted Graph Algorithms) includes practical guidance on Dijkstra's algorithm, priority queues, and representation trade-offs between adjacency lists and matrices.

---

*Documentation prepared for GitHub Issue #5 — Complexity Analysis and Documentation.*  
*Branch: `bini` | Project: negaromanzir-lab/Implement-Graph-Data-Structure-with-Shortest-Path-Algorithms*
