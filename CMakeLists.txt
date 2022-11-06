cmake_minimum_required(VERSION 3.9.1)
set(CMAKE_CXX_STANDARD 20)
project(Acalc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES src/main.cpp src/lexer/lexer.cpp src/parser/parser.cpp src/graph/engine.cpp)

add_executable(Acalc ${SOURCES})
TARGET_LINK_LIBRARIES(Acalc -lSDL2)
