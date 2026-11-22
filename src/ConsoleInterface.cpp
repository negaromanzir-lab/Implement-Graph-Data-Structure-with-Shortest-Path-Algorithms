#include "ConsoleInterface.h"

#include "Dijkstra.h"
#include "Graph.h"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

class TokenReader {
public:
    explicit TokenReader(std::istream& input) : input_(input) {}

    bool next(std::string& token) {
        while (!(lineStream_ >> token)) {
            std::string line;
            if (!std::getline(input_, line)) {
                return false;
            }
            const std::size_t comment = line.find('#');
            if (comment != std::string::npos) {
                line.erase(comment);
            }
            lineStream_.clear();
            lineStream_.str(line);
        }
        return true;
    }

    std::string require(const char* field) {
        std::string token;
        if (!next(token)) {
            throw std::runtime_error(std::string("missing ") + field + ".");
        }
        return token;
    }

private:
    std::istream& input_;
    std::istringstream lineStream_;
};

int parseInt(const std::string& token, const char* field) {
    std::size_t parsed = 0;
    long long value = 0;
    try {
        value = std::stoll(token, &parsed);
    } catch (const std::exception&) {
        throw std::runtime_error(std::string("invalid ") + field + ": " + token + ".");
    }
    if (parsed != token.size() || value < std::numeric_limits<int>::min() ||
        value > std::numeric_limits<int>::max()) {
        throw std::runtime_error(std::string("invalid ") + field + ": " + token + ".");
    }
    return static_cast<int>(value);
}

double parseWeight(const std::string& token) {
    std::size_t parsed = 0;
    double weight = 0.0;
    try {
        weight = std::stod(token, &parsed);
    } catch (const std::exception&) {
        throw std::runtime_error("invalid edge weight: " + token + ".");
    }
    if (parsed != token.size()) {
        throw std::runtime_error("invalid edge weight: " + token + ".");
    }
    return weight;
}

} // namespace

int runConsole(std::istream& input, std::ostream& output, std::ostream& errors) {
    try {
        TokenReader reader(input);
        const int vertexCount = parseInt(reader.require("vertex count"), "vertex count");
        const int edgeCount = parseInt(reader.require("edge count"), "edge count");
        const int directedValue = parseInt(reader.require("directed flag"), "directed flag");

        if (vertexCount < 0 || edgeCount < 0) {
            throw std::runtime_error("vertex and edge counts must be non-negative.");
        }
        if (directedValue != 0 && directedValue != 1) {
            throw std::runtime_error("directed flag must be 0 or 1.");
        }

        Graph graph(vertexCount, directedValue == 1);
        for (int i = 0; i < edgeCount; ++i) {
            const int from = parseInt(reader.require("edge source"), "edge source");
            const int to = parseInt(reader.require("edge destination"), "edge destination");
            const double weight = parseWeight(reader.require("edge weight"));
            graph.addEdge(from, to, weight);
        }

        const int source = parseInt(reader.require("source vertex"), "source vertex");
        if (!graph.isValidVertex(source)) {
            throw std::out_of_range("source vertex is out of range.");
        }
        std::string extra;
        if (reader.next(extra)) {
            throw std::runtime_error("unexpected extra input: " + extra + ".");
        }

        const DijkstraResult result = Dijkstra::shortestPath(graph, source);
        output << "Shortest paths from source vertex " << source << ":\n";
        for (int vertex = 0; vertex < vertexCount; ++vertex) {
            output << "\nVertex " << vertex << ": Distance = ";
            if (!result.hasPathTo(vertex)) {
                output << "INF, Path = []\n";
                continue;
            }

            output << result.getDistanceTo(vertex) << ", Path = [";
            const std::vector<int> path = result.getPathTo(vertex);
            for (std::size_t i = 0; i < path.size(); ++i) {
                if (i != 0) {
                    output << " -> ";
                }
                output << path[i];
            }
            output << "]\n";
        }
        return 0;
    } catch (const std::exception& error) {
        errors << "Error: " << error.what() << '\n';
        return 1;
    }
}
