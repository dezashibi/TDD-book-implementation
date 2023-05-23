cmake_minimum_required(VERSION 3.25)
project(Tests)

file(GLOB_RECURSE SOURCES "chapter*.cpp" "test*.cpp")
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
