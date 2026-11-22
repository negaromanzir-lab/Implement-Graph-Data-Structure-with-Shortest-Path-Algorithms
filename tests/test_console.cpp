#include "ConsoleInterface.h"

#include <cstdio>
#include <sstream>
#include <string>

namespace {

int testsRun = 0;
int testsFailed = 0;

void check(bool condition, const char* name) {
    ++testsRun;
    if (condition) {
        std::printf("[PASS] %s\n", name);
    } else {
        ++testsFailed;
        std::printf("[FAIL] %s\n", name);
    }
}

void testValidInputAndCommentLines() {
    std::istringstream input(
        "# graph header\n"
        "4 4 1\n"
        "0 1 4\n"
        "0 2 2\n"
        "2 1 1\n"
        "1 3 3\n"
        "0\n");
    std::ostringstream output;
    std::ostringstream errors;

    const int status = runConsole(input, output, errors);
    check(status == 0, "valid input exits successfully");
    check(errors.str().empty(), "valid input produces no errors");
    check(output.str().find("Vertex 3: Distance = 6, Path = [0 -> 2 -> 1 -> 3]") !=
              std::string::npos,
          "console prints the computed shortest distance and path");
}

void testDisconnectedInput() {
    std::istringstream input("3 1 1\n0 1 2\n0\n");
    std::ostringstream output;
    std::ostringstream errors;

    check(runConsole(input, output, errors) == 0, "disconnected input exits successfully");
    check(output.str().find("Vertex 2: Distance = INF, Path = []") != std::string::npos,
          "console marks unreachable vertices");
}

void testInvalidInputs() {
    const char* invalidInputs[] = {
        "2 1 1\n0 2 1\n0\n",
        "2 1 2\n0 1 1\n0\n",
        "2 1 1\n0 1 -1\n0\n",
        "2 1 1\n0 1 1\n2\n",
        "2 1 1\n0 1 nope\n0\n",
        "2 0 1\n0\nextra\n",
        "2 1 1\n0 1\n0\n"};

    for (std::size_t i = 0; i < sizeof(invalidInputs) / sizeof(invalidInputs[0]); ++i) {
        std::istringstream input(invalidInputs[i]);
        std::ostringstream output;
        std::ostringstream errors;
        const int status = runConsole(input, output, errors);
        check(status != 0, "invalid input exits with failure");
        check(!errors.str().empty(), "invalid input reports a diagnostic");
    }
}

} // namespace

int main() {
    testValidInputAndCommentLines();
    testDisconnectedInput();
    testInvalidInputs();

    std::printf("\n%d checks run, %d failed\n", testsRun, testsFailed);
    return testsFailed == 0 ? 0 : 1;
}
