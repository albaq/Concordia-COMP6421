#cmake_minimum_required(VERSION 3.13)
#project(SimpleCompiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(SimpleCompiler main.cpp Token.cpp Token.h CharacterScanner.h InMemoryScanner.cpp InMemoryScanner.h Lexer.cpp Lexer.h)
