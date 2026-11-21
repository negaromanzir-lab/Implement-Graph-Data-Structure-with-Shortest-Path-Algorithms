#include "Graph.h"

#include <cmath>
#include <stdexcept>
#include <string>

namespace {

/**
 * @brief Build a descriptive out-of-range message for a bad vertex index.
 */
std::string invalidVertexMessage(const char* role, int vertex, int vertexCount) {
    return "Graph: " + std::string(role) + " vertex " + std::to_string(vertex) +
           " is out of range [0, " + std::to_string(vertexCount) + ").";
}

} // namespace

Graph::Graph(int vertexCount, bool directed)
    : adjacencyList_(static_cast<std::size_t>(vertexCount < 0 ? 0 : vertexCount)),
      directed_(directed),
      edgeCount_(0) {
    if (vertexCount < 0) {
        throw std::invalid_argument("Graph: vertex count cannot be negative (got " +
                                    std::to_string(vertexCount) + ").");
    }
}

bool Graph::isDirected() const noexcept {
    return directed_;
}

int Graph::vertexCount() const noexcept {
    return static_cast<int>(adjacencyList_.size());
}

int Graph::edgeCount() const noexcept {
    return edgeCount_;
}

int Graph::addVertex() {
    adjacencyList_.emplace_back();
    return static_cast<int>(adjacencyList_.size()) - 1;
}

void Graph::addEdge(int from, int to, double weight) {
    // Validate the graph configuration before mutating any adjacency list.
    if (!isValidVertex(from)) {
        throw std::out_of_range(invalidVertexMessage("source", from, vertexCount()));
    }
    if (!isValidVertex(to)) {
        throw std::out_of_range(invalidVertexMessage("destination", to, vertexCount()));
    }
    if (!std::isfinite(weight) || weight < 0.0) {
        throw std::invalid_argument("Graph: edge weight must be finite and non-negative (got " +
                                    std::to_string(weight) + ").");
    }

    adjacencyList_[static_cast<std::size_t>(from)].push_back(Edge{to, weight});

    // Mirror the edge for undirected graphs. A self-loop is stored only once.
    if (!directed_ && from != to) {
        adjacencyList_[static_cast<std::size_t>(to)].push_back(Edge{from, weight});
    }

    ++edgeCount_;
}

bool Graph::hasEdge(int from, int to) const {
    if (!isValidVertex(from)) {
        throw std::out_of_range(invalidVertexMessage("source", from, vertexCount()));
    }
    if (!isValidVertex(to)) {
        throw std::out_of_range(invalidVertexMessage("destination", to, vertexCount()));
    }

    for (const Edge& edge : adjacencyList_[static_cast<std::size_t>(from)]) {
        if (edge.to == to) {
            return true;
        }
    }
    return false;
}

double Graph::getEdgeWeight(int from, int to) const {
    if (!isValidVertex(from)) {
        throw std::out_of_range(invalidVertexMessage("source", from, vertexCount()));
    }
    if (!isValidVertex(to)) {
        throw std::out_of_range(invalidVertexMessage("destination", to, vertexCount()));
    }

    for (const Edge& edge : adjacencyList_[static_cast<std::size_t>(from)]) {
        if (edge.to == to) {
            return edge.weight;
        }
    }
    throw std::runtime_error("Graph: no edge exists from vertex " + std::to_string(from) +
                             " to vertex " + std::to_string(to) + ".");
}

const std::vector<Graph::Edge>& Graph::getNeighbors(int vertex) const {
    if (!isValidVertex(vertex)) {
        throw std::out_of_range(invalidVertexMessage("source", vertex, vertexCount()));
    }
    return adjacencyList_[static_cast<std::size_t>(vertex)];
}

bool Graph::isValidVertex(int vertex) const noexcept {
    return vertex >= 0 && static_cast<std::size_t>(vertex) < adjacencyList_.size();
}
