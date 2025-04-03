cmake_minimum_required(VERSION 3.11)
project(tests)

enable_testing()
add_subdirectory(end-to-end-tests)
message(STATUS "Tests subdirectory added.")
