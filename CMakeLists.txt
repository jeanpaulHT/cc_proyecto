cmake_minimum_required(VERSION 3.14)
project(proyecto_eda)

set(CMAKE_CXX_STANDARD 14)

add_executable(proyecto_eda src/main.cpp src/rtree.h src/point.h src/routeNode.h)