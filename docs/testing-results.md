# Testing and Debugging Results — Final Verification (Issue #6)

## Environment and commands

| Item | Value |
|------|-------|
| Standard | C++17 (`CMAKE_CXX_STANDARD 17`, extensions off) |
| Build tool | CMake 3.22.1 with Ninja generator |
| Compiler | GCC 14.2.0 (MSYS2 UCRT64) |
| Build type | Release |
| Warning flags | `-Wall -Wextra -Wpedantic` (GCC); `/W4 /permissive-` on MSVC |
| External test libraries | None — in-repo assertion helpers are used |

Commands executed from the repository root:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

The build was performed from a fresh, deleted build directory and completed with **0 compiler errors and 0 compiler warnings**. No missing dependencies were encountered: the project uses only the C++17 standard library and tools bundled with CMake.

## Automated results

```text
100% tests passed, 0 tests failed out of 4

Start 1: graph_tests     .......... Passed
Start 2: dijkstra_tests  .......... Passed
Start 3: console_tests   .......... Passed
Start 4: example_output  .......... Passed

Total Test time (real) ≈ 0.14 sec
```

| Test executable | Checks run | Failed |
|-----------------|-----------:|-------:|
| `test_graph` | 44 | 0 |
| `test_dijkstra` | 57 | 0 |
| `test_console` | 19 | 0 |
| **Unit checks total** | **120** | **0** |
| `example_output` (CTest integration case) | 1 | 0 |

## Test case report

| ID | Description | Input | Expected result | Actual result | Status |
|---|---|---|---|---|---|
| G-01 | Default, sized, and expanded graph initialization | Default graph; `Graph(5)`; append two vertices | Counts are 0, 5, then 4; new indices are 2 and 3 | As expected | Pass |
| G-02 | Directed edge insertion | Add `0 -> 1 (4)`, `0 -> 2 (1.5)` | Edges appear only in outgoing adjacency lists; edge count is 2 | As expected | Pass |
| G-03 | Undirected edge insertion | Add `0 -- 1 (7)` and self-loop `2 -- 2 (3)` | Reverse edge is present; logical edge count is 2; self-loop stored once | As expected | Pass |
| G-04 | Zero-weight edge | Add `0 -> 1 (0)` | Edge exists and has weight 0 | As expected | Pass |
| G-05 | Negative and non-finite edge rejection | Add weights `-1`, `NaN`, and `+infinity` | Each throws `invalid_argument`; graph remains unchanged | As expected | Pass |
| G-06 | Invalid graph vertices | Use negative and out-of-range indices in edge/query calls | Each invalid vertex call throws `out_of_range`; rejected insertion changes no edge count | As expected | Pass |
| D-01 | Direct shortest path | `0 -> 1 (5)`, `0 -> 2 (15)`, `1 -> 2 (8)`; source 0 | Distance to 1 is 5; path is `[0, 1]`; source distance is 0 | As expected | Pass |
| D-02 | Competing direct and indirect routes / parent tracking | Routes to 3 cost 10, 7, and 9 | Distance to 3 is 7; path `[0, 2, 3]`; `parent[3] = 2` | As expected | Pass |
| D-03 | Disconnected vertices | Source component `0 -> 1 -> 2`; separate `3 -> 4` | Vertices 3 and 4 have `INF`, parent `-1`, and empty paths | As expected | Pass |
| D-04 | Cyclic graphs and zero-weight cycles | Directed and undirected cycles; directed zero-weight cycle | Algorithm terminates with shortest distances and valid paths | As expected | Pass |
| D-05 | Large distances and overflow | `3B + 4B`; separately `6e18 + 4e18` | Distances 7B and 6e18 are preserved; overflowing sum remains unreachable without wrapping | As expected | Pass |
| D-06 | Invalid source and result queries | Source `-1`, source `vertexCount`, and invalid destination queries | Each throws `out_of_range` | As expected | Pass |
| D-07 | Invalid distance weights | Fractional edge `1.5`; edge at `LLONG_MAX` boundary | Fractional weight throws `invalid_argument`; out-of-range weight throws `overflow_error` | As expected | Pass |
| P-01 | Path reconstruction and source equals destination | Parent chain `0 -> 1 -> 2 -> 3`; reconstruct `0 -> 0` | Reconstructs `[0, 1, 2, 3]`; same-vertex path is `[0]`; unreachable path is empty | As expected | Pass |
| P-02 | Corrupt parent chain safety | Out-of-range parent and cyclic parent pointers | Returns an empty path; no invalid memory access or infinite loop | As expected | Pass |
| C-01 | Valid console input and comments | Four-vertex directed graph with a comment line; source 0 | Exits 0 and prints vertex 3 distance 6 with path `[0 -> 2 -> 1 -> 3]` | As expected | Pass |
| C-02 | Disconnected console output | `3 1 1`, edge `0 1 2`, source 0 | Exits 0 and prints vertex 2 as `INF`, path `[]` | As expected | Pass |
| C-03 | Invalid edge vertex | Edge destination 2 in a two-vertex graph | Exits nonzero and prints an error diagnostic | As expected | Pass |
| C-04 | Invalid directed flag | Header flag 2 | Exits nonzero and prints an error diagnostic | As expected | Pass |
| C-05 | Negative edge weight | Edge weight `-1` | Exits nonzero and prints an error diagnostic | As expected | Pass |
| C-06 | Invalid source | Source index 2 in a two-vertex graph | Exits nonzero and prints an error diagnostic | As expected | Pass |
| C-07 | Malformed, incomplete, or extra input | Non-numeric weight, missing edge field, or trailing token | Exits nonzero and prints an error diagnostic | As expected | Pass |
| I-01 | Shipped example, end to end | `examples/input.txt` via `GraphShortestPathCLI` | Output equals `examples/expected-output.txt` exactly (after CRLF normalization) | Exact match | Pass |

## Additional final-verification checks

### Shortest-path correctness

The sample graph was checked independently by hand and by the application:

| Destination | Expected distance | Actual distance | Expected path | Actual path |
|---|---:|---:|---|---|
| 0 | 0 | 0 | `0` | `0` |
| 1 | 4 | 4 | `0 -> 1` | `0 -> 1` |
| 2 | 2 | 2 | `0 -> 2` | `0 -> 2` |
| 3 | 9 | 9 | `0 -> 1 -> 3` | `0 -> 1 -> 3` |
| 4 | 11 | 11 | `0 -> 1 -> 3 -> 4` | `0 -> 1 -> 3 -> 4` |
| 5 | 14 | 14 | `0 -> 1 -> 3 -> 4 -> 5` | `0 -> 1 -> 3 -> 4 -> 5` |

The interactive application was also driven with a scripted session (one value per prompt). It reported `Source: 0 Destination: 5 Shortest Path: 0 -> 1 -> 3 -> 4 -> 5 Total Distance: 14` and exited with status 0.

### Invalid input handling (batch interface)

| Case | Exit status | Diagnostic |
|---|---:|---|
| Out-of-range edge destination | 1 | `Error: Graph: destination vertex 2 is out of range [0, 2).` |
| Directed flag other than 0/1 | 1 | `Error: directed flag must be 0 or 1.` |
| Negative edge weight | 1 | `Error: Graph: edge weight must be finite and non-negative (got -1.000000).` |
| Out-of-range source | 1 | `Error: source vertex is out of range.` |
| Non-numeric weight | 1 | `Error: invalid edge weight: nope.` |
| Extra trailing token | 1 | `Error: unexpected extra input: extra.` |
| Missing edge field | 1 | `Error: missing source vertex.` |
| Fractional weight rejected by Dijkstra | 1 | `Error: Dijkstra: edge weights must be whole numbers for 64-bit distance calculations.` |

All invalid cases failed cleanly with a diagnostic on standard error; none produced a partial or misleading result.

### Documentation verification

- README commands were executed in the verification environment (configure, build, run both executables, CTest).
- The README example input/output was compared with the files in `examples/`; the `example_output` CTest case guards this against future drift.
- The `.gitignore` excludes build directories and generated binaries, so a clean checkout contains only sources, tests, examples, and documentation.

## Conclusion

All 120 unit checks, the end-to-end example check, the interactive smoke test, and all eight manual invalid-input cases pass. The clean C++17 build produces no errors or warnings and has no external dependencies. The repository is ready for final submission; known algorithmic limitations are documented in the README and in `docs/final-submission.md`.
