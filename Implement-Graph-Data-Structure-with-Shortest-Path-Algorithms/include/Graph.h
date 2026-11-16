#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

/**
 * @file Graph.h
 * @brief Weighted graph stored as an adjacency list.
 *
 * The graph keeps one adjacency list per vertex. Every list entry stores the
 * destination vertex and the weight of the edge. Weights must be non-negative
 * because the project's shortest-path layer is built on Dijkstra's algorithm.
 *
 * Both directed and undirected graphs are supported. For an undirected graph an
 * edge inserted as addEdge(u, v, w) is mirrored as addEdge(v, u, w), so callers
 * always observe symmetric adjacency lists.
 */

class Graph {
public:
    /**
     * @brief One outgoing adjacency entry.
     */
    struct Edge {
        int to;        ///< Destination vertex index.
        double weight; ///< Non-negative edge weight.
    };

    /**
     * @brief Construct a graph with a fixed number of vertices.
     *
     * @param vertexCount Initial number of vertices (must be >= 0).
     * @param directed    true for a directed graph, false for an undirected one.
     *
     * @throws std::invalid_argument if @p vertexCount is negative.
     */
    explicit Graph(int vertexCount = 0, bool directed = true);

    /** @brief Default copy constructor (adjacency lists are value-copied). */
    Graph(const Graph&) = default;

    /** @brief Default copy assignment. */
    Graph& operator=(const Graph&) = default;

    /** @brief Default move constructor. */
    Graph(Graph&&) noexcept = default;

    /** @brief Default move assignment. */
    Graph& operator=(Graph&&) noexcept = default;

    /** @brief Default destructor. */
    ~Graph() = default;

    /**
     * @return true if the graph is directed, false if it is undirected.
     */
    bool isDirected() const noexcept;

    /**
     * @return Number of vertices currently stored in the graph.
     */
    int vertexCount() const noexcept;

    /**
     * @return Number of logical edges. An undirected edge is counted once even
     *         though it is stored twice in the adjacency lists.
     */
    int edgeCount() const noexcept;

    /**
     * @brief Append a new isolated vertex.
     *
     * @return Index of the newly created vertex.
     */
    int addVertex();

    /**
     * @brief Insert a weighted edge.
     *
     * For an undirected graph the reverse edge is inserted automatically, so the
     * adjacency relationship is always symmetric. Zero-weight edges are allowed.
     * Parallel edges (the same ordered pair inserted more than once) and
     * self-loops are allowed.
     *
     * @param from   Source vertex index.
     * @param to     Destination vertex index.
     * @param weight Edge weight (must be >= 0).
     *
     * @throws std::out_of_range      if @p from or @p to is not a valid vertex.
     * @throws std::invalid_argument  if @p weight is negative.
     */
    void addEdge(int from, int to, double weight);

    /**
     * @brief Check whether a direct edge from @p from to @p to exists.
     *
     * @throws std::out_of_range if either vertex index is invalid.
     */
    bool hasEdge(int from, int to) const;

    /**
     * @brief Return the weight of the first edge from @p from to @p to.
     *
     * @throws std::out_of_range   if either vertex index is invalid.
     * @throws std::runtime_error  if no such edge exists.
     */
    double getEdgeWeight(int from, int to) const;

    /**
     * @brief Return the outgoing edges of a vertex.
     *
     * For an undirected graph this is the full neighbor list of the vertex.
     *
     * @throws std::out_of_range if @p vertex is not a valid vertex.
     */
    const std::vector<Edge>& getNeighbors(int vertex) const;

    /**
     * @brief Check whether an index refers to an existing vertex.
     */
    bool isValidVertex(int vertex) const noexcept;

private:
    std::vector<std::vector<Edge>> adjacencyList_; ///< One list per vertex.
    bool directed_;                                ///< true = directed graph.
    int edgeCount_;                                ///< Logical edge count.
};

#endif // GRAPH_H
