cmake_minimum_required(VERSION 3.14)
project(GraphShortestPath LANGUAGES CXX)

# The project targets C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Reusable graph library (Issue #1). The console application in src/main.cpp is
# added by a later issue and is intentionally not part of this target.
add_library(graph STATIC src/Graph.cpp)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Self-contained Graph tests.
enable_testing()
add_executable(test_graph tests/test_graph.cpp)
target_link_libraries(test_graph PRIVATE graph)
add_test(NAME graph_tests COMMAND test_graph)
