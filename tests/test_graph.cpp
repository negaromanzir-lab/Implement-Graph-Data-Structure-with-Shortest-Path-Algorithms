#include "Graph.h"

#include <cstdio>
#include <exception>
#include <limits>
#include <stdexcept>
#include <string>

/**
 * @file test_graph.cpp
 * @brief Self-contained tests for the Graph data structure (Issue #1).
 *
 * The tests use a tiny assertion helper instead of an external framework so the
 * suite can be built with a plain C++17 compiler. Run the executable directly
 * or through CTest.
 */

namespace {

int testsRun = 0;
int testsFailed = 0;

/// Record a boolean check.
void check(bool condition, const std::string& name) {
    ++testsRun;
    if (condition) {
        std::printf("[PASS] %s\n", name.c_str());
    } else {
        ++testsFailed;
        std::printf("[FAIL] %s\n", name.c_str());
    }
}

/// Record a check that @p callable throws the expected exception type.
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

void testEmptyAndSizedConstruction() {
    Graph empty;
    check(empty.vertexCount() == 0, "default graph has 0 vertices");
    check(empty.edgeCount() == 0, "default graph has 0 edges");
    check(empty.isDirected(), "default graph is directed");

    Graph sized(5, true);
    check(sized.vertexCount() == 5, "sized graph reports 5 vertices");
    check(sized.edgeCount() == 0, "sized graph starts with 0 edges");
    check(sized.isDirected(), "sized graph is directed");

    Graph undirected(3, false);
    check(!undirected.isDirected(), "undirected flag is reported");

    expectThrow<std::invalid_argument>(
        [] { Graph bad(-1, true); }, "negative vertex count throws invalid_argument");
}

void testAddVertex() {
    Graph graph(2, true);
    const int v2 = graph.addVertex();
    const int v3 = graph.addVertex();
    check(v2 == 2 && v3 == 3, "addVertex returns consecutive indices");
    check(graph.vertexCount() == 4, "addVertex grows the vertex count");

    graph.addEdge(v3, v2, 2.5);
    check(graph.hasEdge(v3, v2), "edges can use vertices created by addVertex");
}

void testDirectedEdges() {
    Graph graph(4, true);
    graph.addEdge(0, 1, 4.0);
    graph.addEdge(0, 2, 1.5);
    graph.addEdge(1, 3, 2.0);

    check(graph.edgeCount() == 3, "directed edge count is tracked");
    check(graph.hasEdge(0, 1), "directed edge 0->1 exists");
    check(!graph.hasEdge(1, 0), "directed edge is not mirrored");

    const std::vector<Graph::Edge>& neighbors = graph.getNeighbors(0);
    check(neighbors.size() == 2, "vertex 0 has two outgoing edges");
    check(neighbors[0].to == 1 && neighbors[0].weight == 4.0, "edge 0->1 weight is 4.0");
    check(neighbors[1].to == 2 && neighbors[1].weight == 1.5, "edge 0->2 weight is 1.5");
    check(graph.getNeighbors(3).empty(), "isolated vertex has no neighbors");
    check(graph.getEdgeWeight(1, 3) == 2.0, "getEdgeWeight returns the stored weight");
}

void testUndirectedEdges() {
    Graph graph(3, false);
    graph.addEdge(0, 1, 7.0);

    check(graph.edgeCount() == 1, "undirected edge is counted once");
    check(graph.hasEdge(0, 1), "undirected edge 0-1 exists forward");
    check(graph.hasEdge(1, 0), "undirected edge 0-1 exists backward");
    check(graph.getEdgeWeight(1, 0) == 7.0, "reverse direction reports the same weight");
    check(graph.getNeighbors(0).size() == 1, "vertex 0 lists vertex 1 as neighbor");
    check(graph.getNeighbors(1).size() == 1, "vertex 1 lists vertex 0 as neighbor");

    // Self-loops must not be duplicated in an undirected graph.
    graph.addEdge(2, 2, 3.0);
    check(graph.getNeighbors(2).size() == 1, "undirected self-loop is stored once");
    check(graph.hasEdge(2, 2), "undirected self-loop is retrievable");
}

void testZeroWeightEdges() {
    Graph graph(2, true);
    graph.addEdge(0, 1, 0.0);

    check(graph.edgeCount() == 1, "zero-weight edge is accepted");
    check(graph.hasEdge(0, 1), "zero-weight edge exists");
    check(graph.getEdgeWeight(0, 1) == 0.0, "zero-weight edge reports weight 0.0");
}

void testParallelEdges() {
    Graph graph(2, true);
    graph.addEdge(0, 1, 1.0);
    graph.addEdge(0, 1, 5.0);

    check(graph.edgeCount() == 2, "parallel edges are both stored");
    check(graph.getNeighbors(0).size() == 2, "parallel edges produce two neighbors");
}

void testInvalidVertices() {
    Graph graph(3, true);

    expectThrow<std::out_of_range>(
        [&] { graph.addEdge(-1, 0, 1.0); }, "addEdge rejects negative source vertex");
    expectThrow<std::out_of_range>(
        [&] { graph.addEdge(0, 3, 1.0); }, "addEdge rejects out-of-range destination");
    expectThrow<std::out_of_range>(
        [&] { graph.getNeighbors(3); }, "getNeighbors rejects invalid vertex");
    expectThrow<std::out_of_range>(
        [&] { graph.hasEdge(0, -2); }, "hasEdge rejects invalid vertex");
    expectThrow<std::out_of_range>(
        [&] { graph.getEdgeWeight(1, 9); }, "getEdgeWeight rejects invalid vertex");

    // A rejected insertion must leave the graph untouched.
    check(graph.edgeCount() == 0, "rejected addEdge does not change edge count");
}

void testNegativeWeights() {
    Graph directed(2, true);
    expectThrow<std::invalid_argument>(
        [&] { directed.addEdge(0, 1, -0.5); }, "directed negative weight throws invalid_argument");

    Graph undirected(2, false);
    expectThrow<std::invalid_argument>(
        [&] { undirected.addEdge(0, 1, -1.0); }, "undirected negative weight throws invalid_argument");
    expectThrow<std::invalid_argument>(
        [&] { directed.addEdge(0, 1, std::numeric_limits<double>::quiet_NaN()); },
        "NaN edge weight throws invalid_argument");
    expectThrow<std::invalid_argument>(
        [&] { directed.addEdge(0, 1, std::numeric_limits<double>::infinity()); },
        "infinite edge weight throws invalid_argument");
    check(directed.edgeCount() == 0, "rejected weights leave graph unchanged");
}

void testMissingEdgeWeight() {
    Graph graph(2, true);
    expectThrow<std::runtime_error>(
        [&] { graph.getEdgeWeight(0, 1); }, "getEdgeWeight on missing edge throws runtime_error");
}

} // namespace

int main() {
    testEmptyAndSizedConstruction();
    testAddVertex();
    testDirectedEdges();
    testUndirectedEdges();
    testZeroWeightEdges();
    testParallelEdges();
    testInvalidVertices();
    testNegativeWeights();
    testMissingEdgeWeight();

    std::printf("\n%d checks run, %d failed\n", testsRun, testsFailed);
    return testsFailed == 0 ? 0 : 1;
}
