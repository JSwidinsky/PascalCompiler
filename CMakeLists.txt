cmake_minimum_required(VERSION 3.10.2)
project(PascalCompiler)

include_directories(include)

file(GLOB SOURCES "src/*.cpp" "plc.cpp")

add_executable(PascalCompiler ${SOURCES})