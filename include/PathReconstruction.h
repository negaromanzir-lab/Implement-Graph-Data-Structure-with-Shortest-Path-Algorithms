#ifndef PATH_RECONSTRUCTION_H
#define PATH_RECONSTRUCTION_H

#include <limits>
#include <vector>

/**
 * @file PathReconstruction.h
 * @brief Utilities for reconstructing shortest paths from predecessor (parent) arrays.
 *
 * This module is part of Issue #2 (Dijkstra's Algorithm). It provides modular,
 * decoupled functions to reconstruct the sequence of vertices forming the shortest
 * path from a source vertex to any reachable destination.
 */

class PathReconstruction {
public:
    /**
     * @brief Reconstructs the shortest path from @p source to @p destination.
     *
     * Traces backwards from @p destination to @p source using the @p parent array.
     *
     * @param source      The starting vertex index.
     * @param destination The target vertex index.
     * @param parent      Predecessor array where parent[v] is the vertex immediately
     *                    preceding v on the shortest path. Unreachable vertices or
     *                    the source vertex typically have parent value -1.
     *
     * @return std::vector<int> Sequence of vertices from @p source to @p destination inclusive.
     *         Returns an empty vector if @p destination is unreachable from @p source.
     *         If @p source == @p destination, returns {source}.
     *
     * @throws std::out_of_range if @p source or @p destination is not in [0, parent.size()).
     */
    static std::vector<int> reconstructPath(int source, int destination, const std::vector<int>& parent);

    /**
     * @brief Checks whether a path exists to @p destination based on distance.
     *
     * @param destination Target vertex index.
     * @param distances   Array of computed shortest distances.
     * @param inf         The sentinel value representing unreachable vertices
     *                    (defaults to std::numeric_limits<long long>::max()).
     *
     * @return true if destination is reachable and within bounds, false otherwise.
     */
    static bool hasPath(int destination,
                        const std::vector<long long>& distances,
                        long long inf = std::numeric_limits<long long>::max()) noexcept;
};

/**
 * @brief Free function wrapper for PathReconstruction::reconstructPath.
 */
inline std::vector<int> reconstructPath(int source, int destination, const std::vector<int>& parent) {
    return PathReconstruction::reconstructPath(source, destination, parent);
}

#endif // PATH_RECONSTRUCTION_H
