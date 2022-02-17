cmake_minimum_required(VERSION 3.20)
project(turoial_mylanguage)

set(CMAKE_CXX_STANDARD 14)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})

file(GLOB SRC_FILES
        "src/*.h"
        "src/*.cpp"
        )

add_executable(turoial_mylanguage ${SRC_FILES} src/CallExprAST.cpp src/CallExprAST.h)

#add_executable(turoial_mylanguage main.cpp src/AST.cpp src/AST.h src/parser.cpp src/parser.h src/lexer.cpp src/lexer.h)
