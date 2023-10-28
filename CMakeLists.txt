cmake_minimum_required(VERSION 3.7...3.27)

set(CMAKE_CXX_STANDARD 20)

project(numb LANGUAGES CXX)

add_executable(
  numb
  src/ast.cpp
  src/numbspec.cpp
  src/token.cpp
  src/typechecker.cpp
  src/lexer.cpp
  src/parser.cpp
  src/main.cpp)
