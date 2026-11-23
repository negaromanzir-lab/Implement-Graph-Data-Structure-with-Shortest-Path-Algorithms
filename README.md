# Implement Graph Data Structure with Shortest Path Algorithms

A modular C++17 project that implements a weighted graph and Dijkstra's single-source shortest path algorithm, with path reconstruction, two console interfaces, a self-contained test system, and a CMake build.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Problem Statement](#problem-statement)
- [Objectives](#objectives)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Project Architecture](#project-architecture)
- [Directory Structure](#directory-structure)
- [Installation Instructions](#installation-instructions)
- [Compilation Instructions](#compilation-instructions)
- [Execution Instructions](#execution-instructions)
- [Example Input and Output](#example-input-and-output)
- [Testing Instructions](#testing-instructions)
- [Algorithm Explanation](#algorithm-explanation)
- [Complexity Analysis](#complexity-analysis)
- [Limitations](#limitations)
- [Team Contributions](#team-contributions)
- [Final Submission Documents](#final-submission-documents)
- [References](#references)

---

## Project Overview

This repository contains the integrated deliverable for the university mini-project *Implement a Graph Data Structure with Shortest Path Algorithms*. It combines four independently developed modules into one buildable application:

1. **Graph** — a weighted graph stored as an adjacency list, supporting directed and undirected graphs.
2. **Dijkstra** — single-source shortest paths using a binary min-priority queue and 64-bit distances.
3. **Path Reconstruction** — recovery of the actual vertex sequence from predecessor pointers.
4. **Console Interface** — an interactive prompt-driven application and a batch interface for scripted input.

A fifth module, the **testing system**, verifies the graph, algorithm, interface, and the shipped example through CTest. No external libraries are required.

---

## Problem Statement

Many real-world systems (road networks, computer networks, logistics routes) can be modelled as weighted graphs. The practical problem is: **given a starting vertex, find the cheapest route to every other vertex and report both its total cost and the exact sequence of vertices**.

A correct solution must also handle the difficult cases that appear in real data: unreachable vertices, zero-weight edges, cycles, directed versus undirected links, large distances, and invalid user input. The project must be modular so each part can be tested independently, and it must build cleanly with the C++17 standard.

---

## Objectives

- Implement a weighted graph using an adjacency list with validation of vertices and weights.
- Implement Dijkstra's algorithm with a binary min-heap and overflow-safe distance arithmetic.
- Reconstruct shortest paths from a predecessor array, including cycle protection.
- Provide both an interactive console and a batch console that share the same algorithm core.
- Build a self-contained test suite and register it with CTest.
- Produce a clean, documented repository ready for final university submission.

---

## Features

| Area | Feature |
|------|---------|
| Graph | Adjacency-list storage, `O(V + E)` space, directed and undirected modes, dynamic vertex insertion |
| Graph | Rejects negative, non-finite, out-of-range vertex indices and invalid weights with exceptions |
| Algorithm | Dijkstra with `std::priority_queue` and lazy deletion of stale entries |
| Algorithm | `long long` distances with an explicit overflow guard; finite, whole-number weights only |
| Paths | Predecessor-based reconstruction with unreachable-path and corrupt-parent handling |
| Interfaces | Interactive prompt-driven app and token-based batch app using the same `Graph`/`Dijkstra` core |
| Testing | Independent graph, Dijkstra, and console suites plus an end-to-end sample-output check |
| Build | CMake 3.14+, C++17 enforced, warnings enabled on GCC/Clang/MSVC, no third-party dependencies |

---

## Technologies Used

- **Language:** C++17 (`-std=c++17`, no compiler extensions)
- **Build system:** CMake 3.14 or newer
- **Test runner:** CTest (bundled with CMake); tests use a small in-repo assertion helper instead of GoogleTest
- **Compilers:** GCC 7+, Clang 5+, or MSVC 2017+
- **Standard library:** `std::vector`, `std::priority_queue`, `std::pair`, exceptions, iostreams
- **Version control / collaboration:** Git and GitHub (issues, branches, pull requests)
- **Documentation:** Markdown

---

## Project Architecture

The code is layered so that interfaces depend on algorithms, and algorithms depend only on the graph — never the other way around.

```
┌────────────────────────────── Interfaces ──────────────────────────────┐
│  src/main.cpp (interactive)     src/cli_main.cpp + ConsoleInterface.cpp │
│  prompts + validation           token input, formatted results          │
└──────────────────┬──────────────────────────────┬──────────────────────┘
                   │                              │
                   └──────────┬───────────────────┘
                              ▼
┌────────────────────────── Algorithm layer ─────────────────────────────┐
│  Dijkstra::shortestPath()  →  DijkstraResult (distances + parent)       │
│  DijkstraResult::getPathTo()  →  PathReconstruction::reconstructPath()  │
└──────────────────────────────┬───────────────────────────────────────────┘
                               ▼
┌─────────────────────────── Data layer ──────────────────────────────────┐
│  Graph: vector<vector<Edge>> adjacency lists, validation, edge counts   │
└──────────────────────────────────────────────────────────────────────────┘
```

The `graph` static library owns the data and algorithm layers. Both executables and all test executables link against it. `ConsoleInterface.cpp` is compiled into the batch application and the console test binary, so the tested code is exactly the code that runs.

---

## Directory Structure

```
.
├── CMakeLists.txt                  # C++17 build, warnings, targets, CTest registration
├── README.md                       # This document
├── .gitignore                      # Build outputs, binaries, IDE/OS files
├── include/
│   ├── Graph.h                     # Graph class and Edge definition
│   ├── Dijkstra.h                  # Dijkstra solver and DijkstraResult
│   ├── PathReconstruction.h        # Predecessor-chain path reconstruction
│   └── ConsoleInterface.h          # Batch console entry point
├── src/
│   ├── main.cpp                    # Interactive console application
│   ├── cli_main.cpp                # Batch console application entry point
│   ├── Graph.cpp                   # Graph implementation
│   ├── Dijkstra.cpp                # Dijkstra implementation
│   ├── PathReconstruction.cpp      # Path reconstruction implementation
│   └── ConsoleInterface.cpp        # Token parser, validation, result formatting
├── tests/
│   ├── test_graph.cpp              # Graph unit tests
│   ├── test_dijkstra.cpp           # Dijkstra and path reconstruction unit tests
│   ├── test_console.cpp            # Console input-validation tests
│   └── verify_example.cmake        # CTest end-to-end sample comparison
├── examples/
│   ├── input.txt                   # Sample batch input
│   └── expected-output.txt         # Verified expected output
├── docs/
│   ├── final-submission.md         # Final report, presentation outline, contribution record
│   ├── testing-results.md          # Test case report and execution summary
│   ├── technical-documentation.md  # Full technical report (Issue #5)
│   ├── algorithm-analysis.md       # Algorithm design and edge cases (Issue #2)
│   └── complexity-analysis.md      # Complexity derivations (Issue #2)
└── .vscode/
    └── settings.json               # Editor CMake source-directory setting
```

---

## Installation Instructions

**Prerequisites**

- A C++17-compatible compiler: GCC ≥ 7, Clang ≥ 5, or MSVC ≥ 2017
- CMake ≥ 3.14 (CTest is included with CMake)
- Git (only needed to obtain the source)

There are **no third-party library dependencies** to install.

```bash
git clone https://github.com/negaromanzir-lab/Implement-Graph-Data-Structure-with-Shortest-Path-Algorithms.git
cd Implement-Graph-Data-Structure-with-Shortest-Path-Algorithms
```

On Windows with MSYS2, the verification environment used `C:\msys64\ucrt64\bin` (GCC 14.2) and CMake 3.22.1; any equivalent C++17 toolchain works.

---

## Compilation Instructions

From the repository root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

For a multi-configuration generator (for example Visual Studio):

```bash
cmake -S . -B build
cmake --build build --config Release
```

CMake enables `-Wall -Wextra -Wpedantic` (or `/W4 /permissive-` on MSVC) automatically. A clean rebuild is:

```bash
cmake --build build --target clean   # or delete the build/ directory
cmake --build build
```

**Build outputs**

| Executable | Purpose |
|------------|---------|
| `GraphShortestPath` | Interactive console application (`src/main.cpp`) |
| `GraphShortestPathCLI` | Batch console application (`src/cli_main.cpp`) |
| `test_graph`, `test_dijkstra`, `test_console` | Test executables registered with CTest |

With single-config generators the binaries are in `build/`; multi-config generators place them in `build/Release/` (or the selected configuration folder).

---

## Execution Instructions

### Interactive mode

```bash
./build/GraphShortestPath          # Linux/macOS
.\build\GraphShortestPath.exe      # Windows cmd/PowerShell
```

The program asks for:

1. Number of vertices (≥ 1)
2. Whether the graph is directed (`y` or `n`)
3. Number of edges (≥ 0)
4. For each edge: source vertex, destination vertex, and weight — **one value per line**
5. Source and destination vertices for the query
6. Whether to perform another calculation

Prompt-driven mode is designed for a terminal. For scripts, files, or automated checks, use the batch interface below.

### Batch mode (sample input)

```bash
./build/GraphShortestPathCLI < examples/input.txt     # Linux/macOS / cmd
Get-Content examples\input.txt | .\build\GraphShortestPathCLI.exe   # PowerShell
```

The token format is:

```
<vertex_count> <edge_count> <directed: 1|0>
<from> <to> <weight>          # repeated edge_count times
<source_vertex>
```

Blank lines are skipped and `#` starts a comment, so the shipped input file is self-documenting. Unreachable vertices are printed as `Distance = INF, Path = []`. Invalid input produces `Error: ...` on standard error and a non-zero exit status.

---

## Example Input and Output

`examples/input.txt`:

```text
# Graph specification:
# Format:
# <num_vertices> <num_edges> <is_directed (1/0)>
# <from> <to> <weight>
# <source_vertex>

6 9 1
0 1 4
0 2 2
1 2 1
1 3 5
2 3 8
2 4 10
3 4 2
3 5 6
4 5 3
0
```

`examples/expected-output.txt` (verified by the `example_output` CTest case):

```text
Shortest paths from source vertex 0:

Vertex 0: Distance = 0, Path = [0]

Vertex 1: Distance = 4, Path = [0 -> 1]

Vertex 2: Distance = 2, Path = [0 -> 2]

Vertex 3: Distance = 9, Path = [0 -> 1 -> 3]

Vertex 4: Distance = 11, Path = [0 -> 1 -> 3 -> 4]

Vertex 5: Distance = 14, Path = [0 -> 1 -> 3 -> 4 -> 5]
```

For example, the cheapest route from `0` to `5` is `0 -> 1 -> 3 -> 4 -> 5` with cost `4 + 5 + 2 + 3 = 14`; the seemingly direct alternative through vertex `2` costs more.

---

## Testing Instructions

Run every registered test from the build directory:

```bash
ctest --test-dir build --output-on-failure
```

Or run the suites individually:

```bash
./build/test_graph          # graph construction, edges, validation
./build/test_dijkstra       # shortest paths, path reconstruction, overflow, invalid input
./build/test_console        # token parsing and invalid-input handling
```

| CTest name | What it checks |
|------------|----------------|
| `graph_tests` | Directed/undirected insertion, zero weight, parallel edges, invalid vertices and weights |
| `dijkstra_tests` | Direct paths, alternative routes, disconnected vertices, zero weights, cycles, large/overflow weights, invalid sources, parent reconstruction |
| `console_tests` | Valid/commented input, disconnected output, seven malformed-input scenarios |
| `example_output` | Runs the batch CLI on `examples/input.txt` and compares its output with `examples/expected-output.txt` |

Tests are self-contained: no GoogleTest or other download is needed. Full results are recorded in [`docs/testing-results.md`](docs/testing-results.md).

---

## Algorithm Explanation

### Representation

Each vertex `u` owns a vector of `Edge { to, weight }` entries. This uses `O(V + E)` memory and lets Dijkstra examine only the actual neighbours of a settled vertex. Undirected edges are stored in both directions but counted once.

### Dijkstra's method

```
dist[v] = INF for all v;  dist[source] = 0
parent[v] = -1 for all v
push (0, source) into a min-priority queue

while queue is not empty:
    (d, u) = pop minimum
    if u was already settled: continue        // stale entry, lazy deletion
    mark u as settled
    for each edge (u -> v, w):
        if d + w would overflow: skip        // overflow guard
        if d + w < dist[v]:
            dist[v] = d + w
            parent[v] = u
            push (dist[v], v)
```

Because all weights are non-negative, the first time a vertex is popped from the min-heap its distance is final. `std::priority_queue` has no `decrease-key`, so improved distances are simply pushed again and old entries are skipped when popped.

### Path reconstruction

`DijkstraResult::getPathTo(v)` follows `parent[v]` back to the source, detects cycles or corrupt parent indices (returning an empty path), and reverses the collected chain to produce `[source, ..., v]`. `source == destination` returns `[source]`; unreachable targets return `[]`.

---

## Complexity Analysis

Let `V` be the number of vertices and `E` the number of logical edges.

| Operation | Time |
|-----------|------|
| Construct graph and insert all edges | `O(V + E)` |
| `addVertex()` / `addEdge(u, v, w)` | `O(1)` amortised |
| `hasEdge(u, v)` / `getEdgeWeight(u, v)` | `O(deg(u))` |
| Dijkstra with binary min-heap | `O((V + E) log V)` |
| Reconstruct one path | `O(V)` |
| **End-to-end query** | **`O((V + E) log V)`** |

| Structure | Space |
|-----------|-------|
| Adjacency list | `O(V + E)` |
| Distances, parent, visited arrays | `O(V)` |
| Priority queue with lazy deletion | `O(V + E)` |
| Reconstructed path | `O(V)` |
| **Overall** | **`O(V + E)`** |

For comparison, an adjacency matrix uses `O(V²)` space and Dijkstra with a matrix scan takes `O(V²)` time. Detailed derivations are in [`docs/complexity-analysis.md`](docs/complexity-analysis.md) and [`docs/technical-documentation.md`](docs/technical-documentation.md).

---

## Limitations

- **No negative weights.** Dijkstra's correctness requires non-negative weights; negative edges are rejected. Use Bellman–Ford for such graphs.
- **Whole-number weights only.** `double` input is validated but converted to `long long`; fractional weights are rejected with `std::invalid_argument`.
- **Single-source only.** All-pairs shortest paths require running the algorithm once per source.
- **No decrease-key.** Lazy deletion can hold up to `O(V + E)` stale heap entries; a Fibonacci heap would reduce the theoretical bound but is not used here.
- **No dynamic graph updates during a query.** Modify the graph, then recompute.
- **Interactive mode needs one value per line.** Use the batch interface for files and automation.

---

## Team Contributions

Contributions are recorded from the repository history and merged issues; no branches were merged or pushed as part of Issue #6.

| Contributor (git author / GitHub identity) | Contribution |
|--------------------------------------------|--------------|
| Negaro Manzir (`negaromanzir-lab` / `Negaro Manzir`) | Project setup, Graph API and implementation, CMake configuration and graph tests, folder restructure, Dijkstra integration (Issues #1, #2, #9, #10) |
| Kidist (`kidtes-27`) | Edge-weight validation and console interface (`ConsoleInterface.*`) with its tests (Issues #3, #4, #12) |
| Bini (`Bini` / `biniyaminseid2181-collab`) | Technical, algorithm, complexity and testing documentation; README technical content (Issue #5) |
| Genet (`genet` / `Beni-Geni`) | Input validation and error handling in the interactive console (Issue #4, PR #11) |
| Issue #6 integration work | CMake target unification, batch CLI entry point, end-to-end sample test, `.gitignore`, README expansion, final report and verification |

---

## Final Submission Documents

- [`docs/final-submission.md`](docs/final-submission.md) — final report, integration checklist, presentation outline, contribution record and known limitations.
- [`docs/testing-results.md`](docs/testing-results.md) — test case report and execution summary.
- [`docs/technical-documentation.md`](docs/technical-documentation.md) — complete technical report.
- [`docs/algorithm-analysis.md`](docs/algorithm-analysis.md) and [`docs/complexity-analysis.md`](docs/complexity-analysis.md) — algorithm design and complexity.

---

## References

1. Dijkstra, E. W. (1959). "A note on two problems in connexion with graphs." *Numerische Mathematik*, 1(1), 269–271.
2. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to Algorithms* (4th ed.). MIT Press. Chapters 22 and 24.
3. Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley. Part 4.
4. Skiena, S. S. (2008). *The Algorithm Design Manual* (2nd ed.). Springer. Chapter 6.
5. ISO/IEC 14882:2017. *Programming Languages — C++* (C++17 Standard).
6. cppreference.com. `std::priority_queue` — <https://en.cppreference.com/w/cpp/container/priority_queue>
7. cppreference.com. `std::vector` — <https://en.cppreference.com/w/cpp/container/vector>
8. CMake documentation. `cmake`, `ctest` — <https://cmake.org/cmake/help/latest/>

---

*Prepared for GitHub Issue #6 — Integration, README and Final Submission.*
