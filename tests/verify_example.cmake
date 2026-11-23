# Integration test for Issue #6.
#
# Runs the batch console application with the shipped sample input and compares
# its standard output with the shipped expected output.  CRLF differences are
# normalised so the same test works on Windows and Unix-like systems.

foreach(required CLI_EXECUTABLE EXAMPLE_INPUT EXPECTED_OUTPUT)
    if(NOT DEFINED ${required})
        message(FATAL_ERROR "verify_example.cmake: missing -D${required}=...")
    endif()
endforeach()

execute_process(
    COMMAND "${CLI_EXECUTABLE}"
    INPUT_FILE "${EXAMPLE_INPUT}"
    OUTPUT_VARIABLE actual_output
    ERROR_VARIABLE actual_errors
    RESULT_VARIABLE exit_code
)

if(NOT exit_code EQUAL 0)
    message(FATAL_ERROR
        "Batch console exited with code ${exit_code}.\n${actual_errors}")
endif()

if(NOT actual_errors STREQUAL "")
    message(FATAL_ERROR
        "Batch console produced unexpected diagnostics:\n${actual_errors}")
endif()

file(READ "${EXPECTED_OUTPUT}" expected_output)
string(REPLACE "\r\n" "\n" expected_output "${expected_output}")
string(REPLACE "\r\n" "\n" actual_output "${actual_output}")

if(NOT actual_output STREQUAL expected_output)
    message(FATAL_ERROR
        "Sample output does not match examples/expected-output.txt.\n"
        "--- expected ---\n${expected_output}\n"
        "--- actual ---\n${actual_output}")
endif()

message(STATUS "Sample input produced the expected shortest-path output.")
