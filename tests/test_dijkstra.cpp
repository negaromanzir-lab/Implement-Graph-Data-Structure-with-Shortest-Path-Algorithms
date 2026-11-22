#include "Dijkstra.h"
#include "Graph.h"
#include "PathReconstruction.h"

#include <cstdio>
#include <exception>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @file test_dijkstra.cpp
 * @brief Comprehensive test suite for Dijkstra's shortest path algorithm (Issue #2).
 *
 * Covers all required test scenarios:
 * 1. Direct shortest path
 * 2. Multiple alternative routes
 * 3. Shortest path through intermediate vertices
 * 4. Disconnected vertices
 * 5. Zero-weight edges
 * 6. Cyclic graphs
 * 7. Invalid source vertices
 * 8. Large edge weights (using long long and protecting against integer overflow)
 *
 * Plus path reconstruction tests, out-of-range queries, and negative weight handling.
 */

namespace {

int testsRun = 0;
int testsFailed = 0;

void check(bool condition, const std::string& name) {
    ++testsRun;
    if (condition) {
        std::printf("[PASS] %s\n", name.c_str());
    } else {
        ++testsFailed;
        std::printf("[FAIL] %s\n", name.c_str());
    }
}

template <typename ExceptionType, typename Callable>
void expectThrow(Callable&& callable, const std::string& name) {
    ++testsRun;
    try {
        callable();
        ++testsFailed;
        std::printf("[FAIL] %s (no exception thrown)\n", name.c_str());
    } catch (const ExceptionType&) {
        std::printf("[PASS] %s\n", name.c_str());
    } catch (...) {
        ++testsFailed;
        std::printf("[FAIL] %s (unexpected exception type)\n", name.c_str());
    }
}

// 1. Direct Shortest Path
void testDirectShortestPath() {
    Graph graph(3, true);
    graph.addEdge(0, 1, 5.0);
    graph.addEdge(0, 2, 15.0);
    graph.addEdge(1, 2, 8.0);

    const auto result = Dijkstra::shortestPath(graph, 0);

    check(result.getDistanceTo(0) == 0LL, "direct: distance to source is 0");
    check(result.getDistanceTo(1) == 5LL, "direct: shortest distance 0->1 is 5");
    check(result.hasPathTo(1), "direct: path exists to vertex 1");

    const std::vector<int> expectedPath01 = {0, 1};
    check(result.getPathTo(1) == expectedPath01, "direct: path 0->1 is [0, 1]");

    const std::vector<int> expectedSourcePath = {0};
    check(result.getPathTo(0) == expectedSourcePath, "direct: path to source is [0]");
}

// 2. Multiple Alternative Routes
void testMultipleAlternativeRoutes() {
    // 0 -> 1 -> 3 : cost 4 + 6 = 10
    // 0 -> 2 -> 3 : cost 2 + 5 = 7  (optimal)
    // 0 -> 4 -> 3 : cost 8 + 1 = 9
    Graph graph(5, true);
    graph.addEdge(0, 1, 4.0);
    graph.addEdge(1, 3, 6.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(2, 3, 5.0);
    graph.addEdge(0, 4, 8.0);
    graph.addEdge(4, 3, 1.0);

    const auto result = Dijkstra::shortestPath(graph, 0);

    check(result.getDistanceTo(3) == 7LL, "alternative routes: picks shortest cost 7");
    const std::vector<int> expectedPath = {0, 2, 3};
    check(result.getPathTo(3) == expectedPath, "alternative routes: path is [0, 2, 3]");
    check(result.parent[3] == 2, "alternative routes: parent of 3 is 2");
    check(result.parent[2] == 0, "alternative routes: parent of 2 is 0");
}

// 3. Shortest Path Through Intermediate Vertices
void testIntermediateVertices() {
    // Direct edge 0 -> 3 has cost 50, but path 0 -> 1 -> 2 -> 3 has cost 5 + 7 + 8 = 20.
    Graph graph(4, true);
    graph.addEdge(0, 3, 50.0);
    graph.addEdge(0, 1, 5.0);
    graph.addEdge(1, 2, 7.0);
    graph.addEdge(2, 3, 8.0);

    const auto result = Dijkstra::shortestPath(graph, 0);

    check(result.getDistanceTo(3) == 20LL,
          "intermediate: indirect route (20) preferred over expensive direct edge (50)");
    const std::vector<int> expectedPath = {0, 1, 2, 3};
    check(result.getPathTo(3) == expectedPath, "intermediate: path is [0, 1, 2, 3]");
}

// 4. Disconnected Vertices
void testDisconnectedVertices() {
    // Graph with 5 vertices.
    // Component 1: 0 -> 1 (3.0) -> 2 (4.0)
    // Component 2: 3 -> 4 (10.0)
    Graph graph(5, true);
    graph.addEdge(0, 1, 3.0);
    graph.addEdge(1, 2, 4.0);
    graph.addEdge(3, 4, 10.0);

    const auto result = Dijkstra::shortestPath(graph, 0);

    check(result.getDistanceTo(0) == 0LL, "disconnected: component 1 source distance 0");
    check(result.getDistanceTo(1) == 3LL, "disconnected: component 1 distance to 1 is 3");
    check(result.getDistanceTo(2) == 7LL, "disconnected: component 1 distance to 2 is 7");

    check(result.getDistanceTo(3) == Dijkstra::INF, "disconnected: vertex 3 is unreachable (INF)");
    check(result.getDistanceTo(4) == Dijkstra::INF, "disconnected: vertex 4 is unreachable (INF)");
    check(!result.hasPathTo(3), "disconnected: hasPathTo(3) is false");
    check(!result.hasPathTo(4), "disconnected: hasPathTo(4) is false");

    check(result.parent[3] == -1, "disconnected: unreachable vertex 3 has parent -1");
    check(result.parent[4] == -1, "disconnected: unreachable vertex 4 has parent -1");
    check(result.getPathTo(3).empty(), "disconnected: path to unreachable vertex 3 is empty");
    check(result.getPathTo(4).empty(), "disconnected: path to unreachable vertex 4 is empty");
}

// 5. Zero-Weight Edges
void testZeroWeightEdges() {
    // 0 -(0)-> 1 -(5)-> 2 -(0)-> 3
    Graph graph(4, true);
    graph.addEdge(0, 1, 0.0);
    graph.addEdge(1, 2, 5.0);
    graph.addEdge(2, 3, 0.0);

    const auto result = Dijkstra::shortestPath(graph, 0);

    check(result.getDistanceTo(0) == 0LL, "zero-weight: source distance 0");
    check(result.getDistanceTo(1) == 0LL, "zero-weight: edge 0->1 weight 0 gives distance 0");
    check(result.getDistanceTo(2) == 5LL, "zero-weight: edge 1->2 weight 5 gives distance 5");
    check(result.getDistanceTo(3) == 5LL, "zero-weight: edge 2->3 weight 0 keeps distance 5");

    const std::vector<int> expectedPath01 = {0, 1};
    check(result.getPathTo(1) == expectedPath01, "zero-weight: path 0->1 is [0, 1]");

    const std::vector<int> expectedPath03 = {0, 1, 2, 3};
    check(result.getPathTo(3) == expectedPath03, "zero-weight: path 0->3 is [0, 1, 2, 3]");

    // Zero-weight cycle does not cause infinite loop
    Graph cycleGraph(3, true);
    cycleGraph.addEdge(0, 1, 0.0);
    cycleGraph.addEdge(1, 2, 0.0);
    cycleGraph.addEdge(2, 0, 0.0);
    const auto cycleResult = Dijkstra::shortestPath(cycleGraph, 0);
    check(cycleResult.getDistanceTo(2) == 0LL, "zero-weight cycle terminates correctly");
}

// 6. Cyclic Graphs
void testCyclicGraphs() {
    // Directed graph with cycle: 0 -> 1 -> 2 -> 0, and 2 -> 3
    Graph directed(4, true);
    directed.addEdge(0, 1, 2.0);
    directed.addEdge(1, 2, 3.0);
    directed.addEdge(2, 0, 4.0);
    directed.addEdge(2, 3, 1.0);

    const auto dirResult = Dijkstra::shortestPath(directed, 0);
    check(dirResult.getDistanceTo(3) == 6LL, "cyclic directed: shortest distance to 3 is 6");
    const std::vector<int> expectedDir = {0, 1, 2, 3};
    check(dirResult.getPathTo(3) == expectedDir, "cyclic directed: path is [0, 1, 2, 3]");

    // Undirected graph (inherent 2-cycles)
    // 0 - 1 (cost 2)
    // 1 - 2 (cost 3)
    // 0 - 2 (cost 10)
    Graph undirected(3, false);
    undirected.addEdge(0, 1, 2.0);
    undirected.addEdge(1, 2, 3.0);
    undirected.addEdge(0, 2, 10.0);

    const auto undirResult = Dijkstra::shortestPath(undirected, 0);
    check(undirResult.getDistanceTo(2) == 5LL, "cyclic undirected: shortest distance to 2 is 5");
    const std::vector<int> expectedUndir = {0, 1, 2};
    check(undirResult.getPathTo(2) == expectedUndir, "cyclic undirected: path is [0, 1, 2]");
}

// 7. Invalid Source Vertices and Out-of-Bounds Queries
void testInvalidSourceAndQueries() {
    Graph graph(3, true);
    graph.addEdge(0, 1, 1.0);

    expectThrow<std::out_of_range>([&] { Dijkstra::shortestPath(graph, -1); },
                                   "invalid source: negative index throws out_of_range");
    expectThrow<std::out_of_range>([&] { Dijkstra::shortestPath(graph, 3); },
                                   "invalid source: index >= vertexCount throws out_of_range");

    Graph emptyGraph(0, true);
    expectThrow<std::out_of_range>([&] { Dijkstra::shortestPath(emptyGraph, 0); },
                                   "invalid source: empty graph throws out_of_range");

    const auto result = Dijkstra::shortestPath(graph, 0);
    expectThrow<std::out_of_range>([&] { result.getDistanceTo(-1); },
                                   "queries: getDistanceTo with negative index throws");
    expectThrow<std::out_of_range>([&] { result.getDistanceTo(5); },
                                   "queries: getDistanceTo with out-of-range index throws");
    expectThrow<std::out_of_range>([&] { result.hasPathTo(-1); },
                                   "queries: hasPathTo with negative index throws");
    expectThrow<std::out_of_range>([&] { result.hasPathTo(5); },
                                   "queries: hasPathTo with out-of-range index throws");
    expectThrow<std::out_of_range>([&] { result.getPathTo(-1); },
                                   "queries: getPathTo with negative index throws");
    expectThrow<std::out_of_range>([&] { result.getPathTo(5); },
                                   "queries: getPathTo with out-of-range index throws");
}

// 8. Large Edge Weights and Integer Overflow Protection
void testLargeEdgeWeightsAndOverflow() {
    // 64-bit distances: 3 billion + 4 billion = 7 billion (exceeds 32-bit int)
    const double w1 = 3000000000.0;
    const double w2 = 4000000000.0;

    Graph graph(3, true);
    graph.addEdge(0, 1, w1);
    graph.addEdge(1, 2, w2);

    const auto result = Dijkstra::shortestPath(graph, 0);
    const long long expectedDist = 7000000000LL;
    check(result.getDistanceTo(2) == expectedDist,
          "large weights: 3B + 4B = 7B calculated correctly in 64-bit integer");
    const std::vector<int> expectedPath = {0, 1, 2};
    check(result.getPathTo(2) == expectedPath, "large weights: path reconstructed correctly");

    // Overflow protection: edge weight that would cause long long overflow
    // dist[0] = 0 -> 1 (INF - 50) -> 2 (100)
    // Adding (INF - 50) + 100 would overflow LLONG_MAX.
    // Dijkstra must detect overflow, not wrap around to negative numbers,
    // and keep vertex 2 unreachable.
    Graph overflowGraph(3, true);
    const double nearMax = static_cast<double>(Dijkstra::INF - 50LL);
    overflowGraph.addEdge(0, 1, nearMax);
    overflowGraph.addEdge(1, 2, 100.0);

    const auto ovResult = Dijkstra::shortestPath(overflowGraph, 0);
    check(ovResult.getDistanceTo(1) == static_cast<long long>(nearMax),
          "overflow protection: vertex 1 reaches near-max distance");
    check(ovResult.getDistanceTo(2) == Dijkstra::INF,
          "overflow protection: vertex 2 candidate overflow rejected, remains INF");
    check(!ovResult.hasPathTo(2), "overflow protection: hasPathTo(2) is false");
}

// 9. Negative Edge Weight Rejection
void testNegativeEdgeWeightRejection() {
    // Graph::addEdge already rejects negative weights.
    // Test that Graph rejects it properly.
    Graph graph(2, true);
    expectThrow<std::invalid_argument>([&] { graph.addEdge(0, 1, -1.0); },
                                       "negative weights: Graph::addEdge rejects negative weight");
}

// 10. Path Reconstruction Utility Tests
void testPathReconstructionStandalone() {
    // Valid parent array: 0 -> 1 -> 2 -> 3
    const std::vector<int> parent = {-1, 0, 1, 2};

    const std::vector<int> path03 = PathReconstruction::reconstructPath(0, 3, parent);
    const std::vector<int> expected03 = {0, 1, 2, 3};
    check(path03 == expected03, "reconstruction: 0->3 is [0, 1, 2, 3]");

    const std::vector<int> path00 = PathReconstruction::reconstructPath(0, 0, parent);
    const std::vector<int> expected00 = {0};
    check(path00 == expected00, "reconstruction: source==destination returns [source]");

    // Unreachable vertex
    const std::vector<int> parentWithUnreachable = {-1, 0, -1};
    const std::vector<int> path02 = PathReconstruction::reconstructPath(0, 2, parentWithUnreachable);
    check(path02.empty(), "reconstruction: unreachable target returns empty path");

    // Out of range check
    expectThrow<std::out_of_range>([&] { PathReconstruction::reconstructPath(-1, 0, parent); },
                                   "reconstruction: negative source throws out_of_range");
    expectThrow<std::out_of_range>([&] { PathReconstruction::reconstructPath(0, 10, parent); },
                                   "reconstruction: destination >= size throws out_of_range");
}

} // namespace

int main() {
    std::printf("====================================================\n");
    std::printf("Running Dijkstra Algorithm Test Suite (Issue #2)...\n");
    std::printf("====================================================\n");

    testDirectShortestPath();
    testMultipleAlternativeRoutes();
    testIntermediateVertices();
    testDisconnectedVertices();
    testZeroWeightEdges();
    testCyclicGraphs();
    testInvalidSourceAndQueries();
    testLargeEdgeWeightsAndOverflow();
    testNegativeEdgeWeightRejection();
    testPathReconstructionStandalone();

    std::printf("====================================================\n");
    std::printf("Test Summary: %d checks run, %d failed\n", testsRun, testsFailed);
    std::printf("====================================================\n");

    return testsFailed == 0 ? 0 : 1;
}
