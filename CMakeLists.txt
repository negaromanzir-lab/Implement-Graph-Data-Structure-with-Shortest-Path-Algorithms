cmake_minimum_required(VERSION 3.14)
project(GraphShortestPath LANGUAGES CXX)

# The project targets C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Reusable graph and shortest path library (Issues #1 and #2).
# The console application in src/main.cpp is added by a later issue and is
# intentionally not part of this target.
add_library(graph STATIC
    src/Graph.cpp
    src/Dijkstra.cpp
    src/PathReconstruction.cpp
)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable testing for CTest.
enable_testing()

# Self-contained Graph tests (Issue #1).
add_executable(test_graph tests/test_graph.cpp)
target_link_libraries(test_graph PRIVATE graph)
add_test(NAME graph_tests COMMAND test_graph)

# Self-contained Dijkstra algorithm tests (Issue #2).
add_executable(test_dijkstra tests/test_dijkstra.cpp)
target_link_libraries(test_dijkstra PRIVATE graph)
add_test(NAME dijkstra_tests COMMAND test_dijkstra)
