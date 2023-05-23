cmake_minimum_required(VERSION 3.25)
project(tdd_book)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "tests/*.cpp")
add_library(Tests STATIC ${SOURCES})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Tests)