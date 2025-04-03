cmake_minimum_required(VERSION 3.11)
project(correct)

set(PYTHON_SCRIPT_RUN "${CMAKE_SOURCE_DIR}/tests/end-to-end-tests/correct/run_tests.py")
file(GLOB TEST_FILES "${CMAKE_SOURCE_DIR}/tests/end-to-end-tests/correct/run_tests.py")

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_test(
        NAME ${TEST_NAME}
        COMMAND python3 ${PYTHON_SCRIPT_RUN} ${TEST_NAME}.pcl
    )

    set_tests_properties(
        ${TEST_NAME}
        PROPERTIES
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endforeach()