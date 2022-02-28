cmake_minimum_required(VERSION 3.19.2)
project(Irregular_Accesses_Reorder_Unit)

set(CMAKE_CXX_STANDARD 20)
add_executable(Irregular_Accesses_Reorder_Unit algorithms/DisjointSet.cpp algorithms/GraphGenerator.cpp algorithms/IRU.cpp main.cpp algorithms/BFS.cpp algorithms/BFS.h)
