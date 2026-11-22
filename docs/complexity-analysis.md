# Complexity Analysis: Dijkstra's Algorithm (Issue #2)

## 1. Time Complexity

Let $V$ denote the number of vertices and $E$ denote the number of edges in the graph.

### 1.1 Priority Queue Operations
- **Initial insertion**: The source vertex is pushed into the min-priority queue: $O(\log V)$.
- **Extract-Min (pop)**: Each vertex is settled at most once. While duplicate entries can exist in the priority queue due to relaxations, the total number of items pushed is bounded by $O(V + E)$. Each extraction takes $O(\log V)$. Thus, extraction operations across the entire algorithm cost:
  $$O(V \log V)$$
- **Relaxation (push)**: Every edge $(u, v) \in E$ is examined when vertex $u$ is settled. If a relaxation succeeds, a new entry $(d[v], v)$ is pushed into the min-heap. There are at most $E$ successful relaxations. Each insertion into the binary min-heap requires $O(\log V)$ time:
  $$O(E \log V)$$

### 1.2 Total Time Complexity
Combining all operations:
$$\mathcal{O}((V + E) \log V)$$

For connected graphs where $E \ge V - 1$, this simplifies to $\mathcal{O}(E \log V)$. In sparse graphs ($E = \mathcal{O}(V)$), the running time is $\mathcal{O}(V \log V)$.

### 1.3 Path Reconstruction Complexity
Reconstructing a path from $s$ to $t$ using `PathReconstruction::reconstructPath`:
- Traverses predecessor pointers backwards from $t$ to $s$.
- Number of hops $L \le V$.
- Time Complexity: $\mathcal{O}(L) = \mathcal{O}(V)$ in the worst case.

---

## 2. Space Complexity

### 2.1 Auxiliary Data Structures
- **Distance array (`distances`)**: Stores one `long long` per vertex $\implies \mathcal{O}(V)$.
- **Parent array (`parent`)**: Stores one `int` per vertex $\implies \mathcal{O}(V)$.
- **Visited array (`visited`)**: Stores one `bool` per vertex $\implies \mathcal{O}(V)$.
- **Priority Queue (`pq`)**: Stores at most $\mathcal{O}(V + E)$ elements simultaneously in the worst case.

### 2.2 Total Space Complexity
$$\mathcal{O}(V + E)$$
The space complexity is linear with respect to the input size of the graph.
