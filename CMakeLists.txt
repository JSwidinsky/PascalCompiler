cmake_minimum_required(VERSION 2.8.12.2)
project(PascalCompiler)

SET(G++_COVERAGE_COMPILER_FLAGS "-std=c++11" "-Wall" "-g")

include_directories(include)

file(GLOB SOURCES "src/*.cpp" "plc.cpp")

add_definitions(${G++_COVERAGE_COMPILER_FLAGS})
add_executable(PascalCompiler ${SOURCES})