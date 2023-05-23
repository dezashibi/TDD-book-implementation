cmake_minimum_required(VERSION 3.25)
project(Tests)

file(GLOB_RECURSE SOURCES "tests_*.cpp")
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
