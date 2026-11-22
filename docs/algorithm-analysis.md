# Algorithm Analysis: Dijkstra's Algorithm (Issue #2)

## 1. Overview
Dijkstra's algorithm is a greedy algorithm that solves the Single-Source Shortest Path (SSSP) problem for weighted graphs with non-negative edge weights. Given a source vertex $s$, the algorithm computes the minimum distance from $s$ to all other vertices in the graph and constructs a shortest-path tree via predecessor pointers.

## 2. Design and Data Structures

### 2.1 Graph Representation
The algorithm utilizes the `Graph` class implemented in Issue #1:
- Vertices are indexed from `0` to `V - 1`.
- Outgoing edges are stored in an adjacency list `std::vector<std::vector<Edge>>`, enabling $O(\text{deg}(u))$ lookup for vertex $u$'s neighbors.
- Both directed and undirected graphs are supported seamlessly.

### 2.2 Min-Priority Queue
To achieve the optimal $O((V + E) \log V)$ time complexity:
- A binary min-heap (`std::priority_queue` with `std::greater`) is used.
- Priority queue elements store pairs `(distance, vertex_index)`.
- When an edge relaxation yields a shorter distance to neighbor $v$, the new distance is pushed onto the min-heap.
- A `visited` (settled) boolean array ensures that stale heap entries are skipped in $O(1)$ time upon popping.

### 2.3 Distance Calculations and Overflow Protection
- **64-bit integer arithmetic**: Distances are maintained as `long long` integers (`std::vector<long long> distances`), preventing overflow for large edge weights that exceed 32-bit integer limits ($2 \times 10^9$).
- **Overflow Detection**: Before evaluating a candidate distance `currentDist + weight`, the algorithm verifies:
  ```cpp
  if (currentDist > Dijkstra::INF - weight) {
      continue; // Overflow prevented; distance cannot improve
  }
  ```
  This check guarantees that integer additions never trigger undefined behavior or wrap to negative numbers.

### 2.4 Predecessor Tracking (Parent Array)
- A parent array (`std::vector<int> parent`) tracks the predecessor vertex on the shortest path.
- `parent[source]` is initialized to `-1`.
- For all unreachable vertices, `parent[v]` remains `-1`.
- Whenever an edge $(u, v)$ is relaxed, `parent[v]` is updated to $u$.

### 2.5 Edge Case Handling
1. **Disconnected Graphs**: Vertices unreachable from the source retain a distance of `Dijkstra::INF` (`std::numeric_limits<long long>::max()`) and parent `-1`. Query methods cleanly report `hasPathTo(v) == false` and return an empty path.
2. **Zero-Weight Edges**: Edges with weight `0` are processed without issues; because candidate distances are never strictly less than already settled zero-distance nodes, no infinite loops can occur.
3. **Negative-Weight Edges**: The algorithm verifies all edge weights and rejects negative weights by throwing `std::invalid_argument`.
4. **Cyclic Graphs**: Cycles with non-negative weights cannot reduce shortest distances; each vertex is settled at most once.
5. **Path Reconstruction**: Backtracking from destination to source via `parent` pointers incorporates cycle detection to safeguard against corrupted states.
