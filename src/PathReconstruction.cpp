#include "PathReconstruction.h"

#include <algorithm>
#include <stdexcept>
#include <string>

std::vector<int> PathReconstruction::reconstructPath(int source,
                                                     int destination,
                                                     const std::vector<int>& parent) {
    const int n = static_cast<int>(parent.size());
    if (source < 0 || source >= n) {
        throw std::out_of_range("PathReconstruction: source vertex " + std::to_string(source) +
                                " is out of range [0, " + std::to_string(n) + ").");
    }
    if (destination < 0 || destination >= n) {
        throw std::out_of_range("PathReconstruction: destination vertex " + std::to_string(destination) +
                                " is out of range [0, " + std::to_string(n) + ").");
    }

    if (source == destination) {
        return {source};
    }

    if (parent[static_cast<std::size_t>(destination)] == -1) {
        return {}; // Destination is unreachable from source
    }

    std::vector<int> path;
    std::vector<bool> visited(static_cast<std::size_t>(n), false);
    int current = destination;

    while (current != -1) {
        if (visited[static_cast<std::size_t>(current)]) {
            // Cycle detected in parent pointers to prevent infinite looping
            return {};
        }
        visited[static_cast<std::size_t>(current)] = true;
        path.push_back(current);

        if (current == source) {
            break;
        }

        current = parent[static_cast<std::size_t>(current)];
    }

    if (path.empty() || path.back() != source) {
        return {}; // Unable to trace path back to source
    }

    std::reverse(path.begin(), path.end());
    return path;
}

bool PathReconstruction::hasPath(int destination,
                                 const std::vector<long long>& distances,
                                 long long inf) noexcept {
    if (destination < 0 || static_cast<std::size_t>(destination) >= distances.size()) {
        return false;
    }
    return distances[static_cast<std::size_t>(destination)] != inf;
}
