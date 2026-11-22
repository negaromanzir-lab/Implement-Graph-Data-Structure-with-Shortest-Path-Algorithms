# Testing and Debugging Results (Issue #4)

The self-contained C++17 test suites are `tests/test_graph.cpp`,
`tests/test_dijkstra.cpp`, and `tests/test_console.cpp`. GoogleTest is not
required.

## Test Case Report

| ID | Description | Input | Expected result | Actual result | Status |
|---|---|---|---|---|---|
| G-01 | Default, sized, and expanded graph initialization | Default graph; `Graph(5)`; append two vertices | Counts are 0, 5, then 4; new indices are 2 and 3 | Not run: C++ compiler/build tools unavailable | Not run |
| G-02 | Directed edge insertion | Add `0 -> 1 (4)`, `0 -> 2 (1.5)` | Edges appear only in outgoing adjacency lists; edge count is 2 | Not run: C++ compiler/build tools unavailable | Not run |
| G-03 | Undirected edge insertion | Add `0 -- 1 (7)` and self-loop `2 -- 2 (3)` | Reverse edge is present; logical edge count is 2; self-loop stored once | Not run: C++ compiler/build tools unavailable | Not run |
| G-04 | Zero-weight edge | Add `0 -> 1 (0)` | Edge exists and has weight 0 | Not run: C++ compiler/build tools unavailable | Not run |
| G-05 | Negative and non-finite edge rejection | Add weights `-1`, `NaN`, and `+infinity` | Each throws `invalid_argument`; graph remains unchanged | Not run: C++ compiler/build tools unavailable | Not run |
| G-06 | Invalid graph vertices | Use negative and out-of-range indices in edge/query calls | Each invalid vertex call throws `out_of_range`; rejected insertion changes no edge count | Not run: C++ compiler/build tools unavailable | Not run |
| D-01 | Direct shortest path | `0 -> 1 (5)`, `0 -> 2 (15)`, `1 -> 2 (8)`; source 0 | Distance to 1 is 5; path is `[0, 1]`; source distance is 0 | Not run: C++ compiler/build tools unavailable | Not run |
| D-02 | Competing direct and indirect routes / parent tracking | Routes to 3 cost 10, 7, and 9 | Distance to 3 is 7; path `[0, 2, 3]`; `parent[3] = 2` | Not run: C++ compiler/build tools unavailable | Not run |
| D-03 | Disconnected vertices | Source component `0 -> 1 -> 2`; separate `3 -> 4` | Vertices 3 and 4 have `INF`, parent `-1`, and empty paths | Not run: C++ compiler/build tools unavailable | Not run |
| D-04 | Cyclic graphs and zero-weight cycles | Directed and undirected cycles; directed zero-weight cycle | Algorithm terminates with shortest distances and valid paths | Not run: C++ compiler/build tools unavailable | Not run |
| D-05 | Large distances and overflow | `3B + 4B`; separately `6e18 + 4e18` | Distances 7B and 6e18 are preserved; overflowing sum remains unreachable without wrapping | Not run: C++ compiler/build tools unavailable | Not run |
| D-06 | Invalid source and result queries | Source `-1`, source `vertexCount`, and invalid destination queries | Each throws `out_of_range` | Not run: C++ compiler/build tools unavailable | Not run |
| D-07 | Invalid distance weights | Fractional edge `1.5`; edge at `LLONG_MAX` boundary | Fractional weight throws `invalid_argument`; out-of-range weight throws `overflow_error`, never casts unsafely | Not run: C++ compiler/build tools unavailable | Not run |
| P-01 | Path reconstruction and source equals destination | Parent chain `0 -> 1 -> 2 -> 3`; reconstruct `0 -> 0` | Reconstructs `[0, 1, 2, 3]`; same-vertex path is `[0]`; unreachable path is empty | Not run: C++ compiler/build tools unavailable | Not run |
| P-02 | Corrupt parent chain safety | Out-of-range parent and cyclic parent pointers | Returns an empty path; no invalid memory access or infinite loop | Not run: C++ compiler/build tools unavailable | Not run |
| C-01 | Valid console input and comments | Four-vertex directed graph with a comment line; source 0 | Exits 0 and prints vertex 3 distance 6 with path `[0 -> 2 -> 1 -> 3]` | Not run: C++ compiler/build tools unavailable | Not run |
| C-02 | Disconnected console output | `3 1 1`, edge `0 1 2`, source 0 | Exits 0 and prints vertex 2 as `INF`, path `[]` | Not run: C++ compiler/build tools unavailable | Not run |
| C-03 | Invalid edge vertex | Edge destination 2 in a two-vertex graph | Exits nonzero and prints an error diagnostic | Not run: C++ compiler/build tools unavailable | Not run |
| C-04 | Invalid directed flag | Header flag 2 | Exits nonzero and prints an error diagnostic | Not run: C++ compiler/build tools unavailable | Not run |
| C-05 | Negative edge weight | Edge weight `-1` | Exits nonzero and prints an error diagnostic | Not run: C++ compiler/build tools unavailable | Not run |
| C-06 | Invalid source | Source index 2 in a two-vertex graph | Exits nonzero and prints an error diagnostic | Not run: C++ compiler/build tools unavailable | Not run |
| C-07 | Malformed, incomplete, or extra input | Non-numeric weight, missing edge field, or trailing token | Exits nonzero and prints an error diagnostic | Not run: C++ compiler/build tools unavailable | Not run |

## Execution Summary

- **Requested standard/warnings:** C++17, `-Wall -Wextra -Wpedantic` (enabled in CMake for non-MSVC compilers).
- **Build/test command:** `cmake -S . -B build -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic"`, then `cmake --build build` and `ctest --test-dir build --output-on-failure`.
- **Result:** Not executed. The initial build attempt failed because `cmake` is not installed, and no C++ compiler was found. A user-scoped LLVM installation was unavailable; machine-wide installation was declined.
- **Pass/fail totals:** No tests were executed; no test is reported as passed or failed.
