cmake_minimum_required(VERSION 3.14)

project(GraphShortestPath
    VERSION 1.0.0
    DESCRIPTION "Weighted graph and Dijkstra shortest paths in C++17"
    LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Language standard: C++17 with no compiler-specific extensions.
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Enable compiler warnings for every target in the project.
# ---------------------------------------------------------------------------
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# Integrated modules: Graph, Dijkstra, path reconstruction and console I/O.
# Headers are listed as sources so IDE generators show the complete project.
# ---------------------------------------------------------------------------
set(GRAPH_HEADERS
    include/ConsoleInterface.h
    include/Dijkstra.h
    include/Graph.h
    include/PathReconstruction.h
)

set(GRAPH_SOURCES
    src/Graph.cpp
    src/Dijkstra.cpp
    src/PathReconstruction.cpp
)

add_library(graph STATIC ${GRAPH_SOURCES} ${GRAPH_HEADERS})
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(graph PUBLIC cxx_std_17)

# ---------------------------------------------------------------------------
# Applications.
# ---------------------------------------------------------------------------

# Interactive, prompt-driven console application (Issue #4).
add_executable(GraphShortestPath src/main.cpp)
target_link_libraries(GraphShortestPath PRIVATE graph)

# Batch console application: reads the token format used by examples/input.txt.
# It reuses the tested ConsoleInterface module without duplicating its logic.
add_executable(GraphShortestPathCLI src/cli_main.cpp src/ConsoleInterface.cpp)
target_link_libraries(GraphShortestPathCLI PRIVATE graph)

# ---------------------------------------------------------------------------
# Testing system: three unit suites plus an end-to-end example check.
# ---------------------------------------------------------------------------
enable_testing()

add_executable(test_graph tests/test_graph.cpp)
target_link_libraries(test_graph PRIVATE graph)
add_test(NAME graph_tests COMMAND test_graph)

add_executable(test_dijkstra tests/test_dijkstra.cpp)
target_link_libraries(test_dijkstra PRIVATE graph)
add_test(NAME dijkstra_tests COMMAND test_dijkstra)

add_executable(test_console tests/test_console.cpp src/ConsoleInterface.cpp)
target_link_libraries(test_console PRIVATE graph)
add_test(NAME console_tests COMMAND test_console)

add_test(
    NAME example_output
    COMMAND ${CMAKE_COMMAND}
        -DCLI_EXECUTABLE=$<TARGET_FILE:GraphShortestPathCLI>
        -DEXAMPLE_INPUT=${CMAKE_CURRENT_SOURCE_DIR}/examples/input.txt
        -DEXPECTED_OUTPUT=${CMAKE_CURRENT_SOURCE_DIR}/examples/expected-output.txt
        -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/verify_example.cmake
)
