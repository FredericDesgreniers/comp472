cmake_minimum_required(VERSION 3.8)
project(comp472_2)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp GameMemory.cpp GameMemory.h BoardRenderer.cpp BoardRenderer.h WindowsBoardRenderer.cpp WindowsBoardRenderer.h vec2.h)
add_executable(comp472_2 ${SOURCE_FILES})