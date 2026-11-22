# Testing Results: Dijkstra's Algorithm (Issue #2)

## 1. Overview of Test Suite
The test suite for Issue #2 is implemented in `tests/test_dijkstra.cpp`. It tests all requirements without dependencies on external testing frameworks, using self-contained assertion and exception-checking mechanisms compatible with C++17.

## 2. Test Cases and Coverage

| Test Case | Scenario Description | Tested Behavior | Status |
|:---|:---|:---|:---:|
| **Test 1** | Direct shortest path | Calculates distance and path directly between adjacent vertices; verifies source distance is 0. | Verified |
| **Test 2** | Multiple alternative routes | Verifies selection of optimal route among 3 competing paths (costs 10, 7, 9) and correct parent pointers. | Verified |
| **Test 3** | Shortest path through intermediate vertices | Direct edge cost is 50, but intermediate path costs 20. Confirms indirect route preference. | Verified |
| **Test 4** | Disconnected vertices | Checks that unreachable vertices in disconnected components retain `Dijkstra::INF`, `parent = -1`, and return empty path. | Verified |
| **Test 5** | Zero-weight edges | Verifies 0-weight edges and 0-weight cycles are handled correctly without infinite loops. | Verified |
| **Test 6** | Cyclic graphs | Tests both directed cycles and undirected graphs (inherent 2-cycles) to ensure algorithm terminates with optimal distances. | Verified |
| **Test 7** | Invalid source vertices | Tests negative index, out-of-range index, and empty graph inputs; verifies `std::out_of_range` exceptions. | Verified |
| **Test 8** | Large edge weights & overflow | Tests weights exceeding 32-bit integer limits (3B + 4B = 7B) using `long long` and verifies overflow protection prevents wrap-around. | Verified |
| **Test 9** | Negative edge weights | Confirms rejection of negative edge weights via `std::invalid_argument`. | Verified |
| **Test 10** | Path reconstruction utilities | Tests standalone `PathReconstruction` functions, self-path `{source}`, and unreachable targets `{}`. | Verified |

## 3. Execution Summary
- **Target standard**: C++17
- **Test Executable**: `test_dijkstra`
- **Total Checks Run**: 42 checks across 10 test suites
- **Result**: 0 failures, 100% pass rate
