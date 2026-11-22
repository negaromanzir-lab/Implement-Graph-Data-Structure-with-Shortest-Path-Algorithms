#include "Dijkstra.h"

#include <cmath>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

/**
 * @brief Build an out-of-range error message for a vertex index.
 */
std::string vertexOutOfRangeMessage(const char* role, int vertex, int vertexCount) {
    return "Dijkstra: " + std::string(role) + " vertex " + std::to_string(vertex) +
           " is out of range [0, " + std::to_string(vertexCount) + ").";
}

} // namespace

bool DijkstraResult::hasPathTo(int destination) const {
    const int count = static_cast<int>(distances.size());
    if (destination < 0 || destination >= count) {
        throw std::out_of_range(vertexOutOfRangeMessage("destination", destination, count));
    }
    return distances[static_cast<std::size_t>(destination)] != Dijkstra::INF;
}

long long DijkstraResult::getDistanceTo(int destination) const {
    const int count = static_cast<int>(distances.size());
    if (destination < 0 || destination >= count) {
        throw std::out_of_range(vertexOutOfRangeMessage("destination", destination, count));
    }
    return distances[static_cast<std::size_t>(destination)];
}

std::vector<int> DijkstraResult::getPathTo(int destination) const {
    const int count = static_cast<int>(distances.size());
    if (destination < 0 || destination >= count) {
        throw std::out_of_range(vertexOutOfRangeMessage("destination", destination, count));
    }
    return PathReconstruction::reconstructPath(source, destination, parent);
}

DijkstraResult Dijkstra::shortestPath(const Graph& graph, int source) {
    const int numVertices = graph.vertexCount();

    // 1. Validate source vertex
    if (!graph.isValidVertex(source)) {
        throw std::out_of_range(vertexOutOfRangeMessage("source", source, numVertices));
    }

    // 2. Validate all graph edge weights upfront: reject negative weights
    for (int u = 0; u < numVertices; ++u) {
        for (const auto& edge : graph.getNeighbors(u)) {
            if (std::isnan(edge.weight) || edge.weight < 0.0) {
                throw std::invalid_argument(
                    "Dijkstra: negative or invalid edge weight (" + std::to_string(edge.weight) +
                    ") detected on edge from vertex " + std::to_string(u) + " to vertex " +
                    std::to_string(edge.to) + ".");
            }
        }
    }

    // 3. Initialize distance and parent arrays
    DijkstraResult result;
    result.source = source;
    result.distances.assign(static_cast<std::size_t>(numVertices), Dijkstra::INF);
    result.parent.assign(static_cast<std::size_t>(numVertices), -1);

    result.distances[static_cast<std::size_t>(source)] = 0;

    // Min-priority queue storing pairs of (distance, vertex_index)
    using PQElement = std::pair<long long, int>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;

    pq.emplace(0LL, source);
    std::vector<bool> visited(static_cast<std::size_t>(numVertices), false);

    // 4. Dijkstra main loop using binary min-heap
    while (!pq.empty()) {
        const auto [currentDist, u] = pq.top();
        pq.pop();

        // If vertex u was already finalized at its shortest distance, skip
        if (visited[static_cast<std::size_t>(u)]) {
            continue;
        }
        visited[static_cast<std::size_t>(u)] = true;

        for (const auto& edge : graph.getNeighbors(u)) {
            const int v = edge.to;
            const double rawWeight = edge.weight;

            if (rawWeight < 0.0) {
                throw std::invalid_argument(
                    "Dijkstra: negative edge weight detected during traversal (" +
                    std::to_string(rawWeight) + ").");
            }

            // Convert edge weight to long long, capping if it exceeds INF
            long long weight = 0;
            if (rawWeight >= static_cast<double>(Dijkstra::INF)) {
                weight = Dijkstra::INF;
            } else {
                weight = static_cast<long long>(rawWeight);
            }

            // Integer overflow protection:
            // Ensure currentDist + weight does not overflow Dijkstra::INF (LLONG_MAX).
            if (currentDist > Dijkstra::INF - weight) {
                // Sum exceeds maximum representable distance; cannot improve target
                continue;
            }

            const long long candidateDist = currentDist + weight;
            if (candidateDist < result.distances[static_cast<std::size_t>(v)]) {
                result.distances[static_cast<std::size_t>(v)] = candidateDist;
                result.parent[static_cast<std::size_t>(v)] = u;
                pq.emplace(candidateDist, v);
            }
        }
    }

    return result;
}
