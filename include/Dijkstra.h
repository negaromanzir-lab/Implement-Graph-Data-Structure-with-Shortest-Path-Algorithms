#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Graph.h"
#include "PathReconstruction.h"

#include <limits>
#include <vector>

/**
 * @file Dijkstra.h
 * @brief Dijkstra's Single-Source Shortest Path Algorithm in C++17.
 *
 * Implements GitHub Issue #2: Dijkstra's Algorithm.
 *
 * Features:
 * - Uses the Graph class from Issue #1 (adjacency list representation).
 * - Employs a binary min-priority queue (std::priority_queue with std::greater).
 * - Computes shortest distances from a specified source vertex using 64-bit integer
 *   arithmetic (long long) for distance calculations.
 * - Protects against integer overflow during path relaxation.
 * - Tracks predecessor vertices via a parent array for complete path reconstruction.
 * - Fully supports disconnected graphs, marking unreachable vertices with Dijkstra::INF.
 * - Supports zero-weight edges and cyclic graphs.
 * - Rejects negative-weight edges by throwing std::invalid_argument.
 * - Provides modular, self-contained path reconstruction without console I/O dependencies.
 *
 * Time Complexity:  O((V + E) log V) where V = vertex count, E = edge count.
 * Space Complexity: O(V + E) for min-priority queue and distance/parent arrays.
 */

/**
 * @brief Container holding the output of Dijkstra's algorithm.
 */
struct DijkstraResult {
    int source = -1;                    ///< The source vertex index.
    std::vector<long long> distances;   ///< Shortest distances from source to each vertex.
    std::vector<int> parent;            ///< Predecessor array for path reconstruction.

    /**
     * @brief Check whether a path exists from source to @p destination.
     *
     * @param destination Target vertex index.
     * @return true if @p destination is reachable from @p source, false otherwise.
     * @throws std::out_of_range if @p destination is not a valid vertex index.
     */
    bool hasPathTo(int destination) const;

    /**
     * @brief Retrieve the shortest distance from source to @p destination.
     *
     * @param destination Target vertex index.
     * @return The shortest distance, or Dijkstra::INF if unreachable.
     * @throws std::out_of_range if @p destination is not a valid vertex index.
     */
    long long getDistanceTo(int destination) const;

    /**
     * @brief Reconstruct the sequence of vertices on the shortest path from source to @p destination.
     *
     * @param destination Target vertex index.
     * @return std::vector<int> Path vertices [source, ..., destination]. Empty if unreachable.
     * @throws std::out_of_range if @p destination is not a valid vertex index.
     */
    std::vector<int> getPathTo(int destination) const;
};

/**
 * @brief Single-source shortest path solver using Dijkstra's algorithm.
 */
class Dijkstra {
public:
    /// Sentinel value representing infinity for unreachable vertices.
    static constexpr long long INF = std::numeric_limits<long long>::max();

    /**
     * @brief Compute shortest paths from @p source to all vertices in @p graph.
     *
     * @param graph  The input graph (directed or undirected).
     * @param source The starting vertex index.
     * @return DijkstraResult containing distance array, parent array, and query helpers.
     *
     * @throws std::out_of_range     if @p source is not in [0, graph.vertexCount()).
     * @throws std::invalid_argument if any negative-weight edge is found in @p graph.
     */
    static DijkstraResult shortestPath(const Graph& graph, int source);

    /**
     * @brief Alias for shortestPath.
     */
    static DijkstraResult shortestPaths(const Graph& graph, int source) {
        return shortestPath(graph, source);
    }

    /**
     * @brief Convenience helper: reconstructs shortest path directly from @p source to @p destination.
     *
     * @throws std::out_of_range if @p source or @p destination is invalid.
     */
    static std::vector<int> findShortestPath(const Graph& graph, int source, int destination) {
        return shortestPath(graph, source).getPathTo(destination);
    }

    /**
     * @brief Convenience helper: computes shortest distance directly from @p source to @p destination.
     *
     * @throws std::out_of_range if @p source or @p destination is invalid.
     */
    static long long findShortestDistance(const Graph& graph, int source, int destination) {
        return shortestPath(graph, source).getDistanceTo(destination);
    }
};

/**
 * @brief Free function wrapper for Dijkstra::shortestPath.
 */
inline DijkstraResult dijkstra(const Graph& graph, int source) {
    return Dijkstra::shortestPath(graph, source);
}

#endif // DIJKSTRA_H
