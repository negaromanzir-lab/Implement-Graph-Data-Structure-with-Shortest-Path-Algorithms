/**
 * @file cli_main.cpp
 * @brief Entry point for the batch (non-interactive) console interface.
 *
 * The interactive application lives in src/main.cpp.  This thin wrapper feeds
 * standard input to the same ConsoleInterface module that is exercised by the
 * console unit tests, so the files in examples/ can be used directly:
 *
 *     GraphShortestPathCLI < examples/input.txt
 */

#include "ConsoleInterface.h"

#include <iostream>

int main() {
    return runConsole(std::cin, std::cout, std::cerr);
}
