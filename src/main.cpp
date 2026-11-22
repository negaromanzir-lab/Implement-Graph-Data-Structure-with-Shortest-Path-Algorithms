#include <iostream>
#include <limits>
#include <string>

#include "../include/Graph.h"
#include "../include/Dijkstra.h"

// Helper to read an integer with validation.
static int readInt(const std::string &prompt, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max()) {
    int value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Error] Invalid number. Please enter an integer.\n";
            continue;
        }
        if (value < min || value > max) {
            std::cout << "[Error] Value out of range (" << min << ".." << max << ").\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

// Helper to read a double (weight) with validation.
static double readDouble(const std::string &prompt, double min = 0.0) {
    double value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Error] Invalid number. Please enter a numeric value.\n";
            continue;
        }
        if (value < min) {
            std::cout << "[Error] Weight cannot be negative.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

static bool readYesNo(const std::string &prompt) {
    std::string answer;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, answer);
        if (answer.empty()) continue;
        char c = std::tolower(answer[0]);
        if (c == 'y') return true;
        if (c == 'n') return false;
        std::cout << "[Error] Please answer 'y' or 'n'.\n";
    }
}

static void displayResult(int source, int dest, const DijkstraResult &res) {
    if (!res.hasPathTo(dest)) {
        std::cout << "[Info] Destination vertex " << dest << " is unreachable from source " << source << ".\n";
        return;
    }
    std::vector<int> path = res.getPathTo(dest);
    std::cout << "================================ SHORTEST PATH RESULT\n";
    std::cout << "Source: " << source << " Destination: " << dest << " Shortest Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i + 1 < path.size()) std::cout << " -> ";
    }
    std::cout << " Total Distance: " << res.getDistanceTo(dest) << "\n";
}

int main() {
    std::cout << "=== Graph Shortest Path Calculator (C++17) ===\n";
    while (true) {
        // ----- Graph construction -----
        int vertexCount = readInt("Enter number of vertices: ", 1);
        bool directed = readYesNo("Is the graph directed? (y/n): ");
        Graph graph(vertexCount, directed);

        int edgeCount = readInt("Enter number of edges: ", 0);
        for (int i = 0; i < edgeCount; ++i) {
            std::cout << "--- Edge " << i + 1 << " of " << edgeCount << " ---\n";
            int from = readInt("  Source vertex: ", 0, vertexCount - 1);
            int to   = readInt("  Destination vertex: ", 0, vertexCount - 1);
            double weight = readDouble("  Edge weight (non‑negative): ");
            try {
                graph.addEdge(from, to, weight);
            } catch (const std::exception &e) {
                std::cout << "[Error] " << e.what() << " Please re‑enter this edge.\n";
                --i; // repeat this iteration
            }
        }

        // ----- Shortest‑path query -----
        int source = readInt("Select starting vertex: ", 0, vertexCount - 1);
        int destination = readInt("Select destination vertex: ", 0, vertexCount - 1);
        DijkstraResult result;
        try {
            result = Dijkstra::shortestPath(graph, source);
        } catch (const std::exception &e) {
            std::cout << "[Error] Failed to compute shortest paths: " << e.what() << "\n";
            // allow user to restart whole process
            if (!readYesNo("Do you want to try again? (y/n): ")) break;
            continue;
        }
        displayResult(source, destination, result);

        // ----- Repeat? -----
        if (!readYesNo("Perform another calculation? (y/n): ")) break;
        std::cout << "\n"; // spacing
    }
    std::cout << "Good bye!\n";
    return 0;
}
