cmake_minimum_required(VERSION 3.10)
project(PascalCompiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(PascalCompiler plc.cpp src/Token.cpp src/Token.h src/Symbol.h src/SymbolTable.cpp src/SymbolTable.h src/Scanner.cpp src/Scanner.h src/Administration.cpp src/Administration.h)