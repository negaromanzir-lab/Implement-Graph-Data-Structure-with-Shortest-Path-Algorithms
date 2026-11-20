"""Single-source shortest distances for graphs with nonnegative edge weights.

The algorithm initializes every known vertex to infinity and the source to
zero, then repeatedly visits the closest unsettled vertex from a min-heap.
Each outgoing edge is relaxed when it offers a shorter route. Unreachable
vertices remain at infinity.

Dijkstra's algorithm requires nonnegative, finite edge weights. With an
adjacency-list graph and a binary heap it runs in O((V + E) log V). For
negative-weight edges, use an algorithm such as Bellman-Ford instead. An
undirected edge must be listed in both vertices' adjacency lists.
"""

from heapq import heappop, heappush
from itertools import count
from math import isfinite, inf
from numbers import Real
from typing import Dict, Hashable, Iterable, Mapping, Tuple, TypeVar


Vertex = TypeVar("Vertex", bound=Hashable)


def dijkstra(
    graph: Mapping[Vertex, Iterable[Tuple[Vertex, Real]]],
    source: Vertex,
) -> Dict[Vertex, float]:
    """Return shortest distances from ``source`` to every graph vertex.

    The graph maps each vertex to an iterable of ``(neighbor, weight)`` edges.
    Vertices mentioned only as neighbors are included in the result. A missing
    source is treated as an isolated vertex. Invalid weights raise ValueError.
    """
    adjacency = {}
    vertices = set(graph)
    for vertex, edges in graph.items():
        adjacency[vertex] = []
        for neighbor, weight in edges:
            if (
                not isinstance(weight, Real)
                or not isfinite(float(weight))
                or weight < 0
            ):
                raise ValueError("edge weights must be finite, nonnegative real numbers")
            adjacency[vertex].append((neighbor, float(weight)))
            vertices.add(neighbor)

    vertices.add(source)
    distances = {vertex: inf for vertex in vertices}
    distances[source] = 0.0
    sequence = count()
    queue = [(0.0, next(sequence), source)]

    while queue:
        distance, _, vertex = heappop(queue)
        if distance != distances[vertex]:
            continue

        for neighbor, weight in adjacency.get(vertex, ()):
            candidate = distance + weight
            if candidate < distances[neighbor]:
                distances[neighbor] = candidate
                heappush(queue, (candidate, next(sequence), neighbor))

    return distances
