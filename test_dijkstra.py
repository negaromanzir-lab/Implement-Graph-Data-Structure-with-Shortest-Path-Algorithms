import math
import unittest

from dijkstra import dijkstra


class DijkstraTests(unittest.TestCase):
    def test_finds_shortest_distances_and_keeps_unreachable_vertices(self):
        graph = {
            "A": [("B", 4), ("C", 1)],
            "B": [("D", 1)],
            "C": [("B", 2), ("D", 8)],
            "D": [],
            "isolated": [],
        }

        distances = dijkstra(graph, "A")

        self.assertEqual(distances["A"], 0)
        self.assertEqual(distances["B"], 3)
        self.assertEqual(distances["C"], 1)
        self.assertEqual(distances["D"], 4)
        self.assertTrue(math.isinf(distances["isolated"]))

    def test_includes_neighbor_only_vertices_and_handles_zero_weight(self):
        distances = dijkstra({"start": [("middle", 0)]}, "start")

        self.assertIn("middle", distances)
        self.assertEqual(distances["middle"], 0)

    def test_missing_source_is_an_isolated_vertex(self):
        distances = dijkstra({"known": []}, "source")

        self.assertEqual(distances["source"], 0)
        self.assertTrue(math.isinf(distances["known"]))

    def test_rejects_negative_or_non_finite_weights(self):
        for weight in (-1, math.inf, math.nan):
            with self.subTest(weight=weight):
                with self.assertRaises(ValueError):
                    dijkstra({"start": [("end", weight)]}, "start")

    def test_equal_priority_vertices_need_not_be_orderable(self):
        first = object()
        second = object()
        end = object()
        graph = {
            "start": [(first, 1), (second, 1)],
            first: [(end, 1)],
            second: [(end, 1)],
        }

        self.assertEqual(dijkstra(graph, "start")[end], 2)


if __name__ == "__main__":
    unittest.main()
