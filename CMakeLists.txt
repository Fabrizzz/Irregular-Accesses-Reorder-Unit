cmake_minimum_required(VERSION 3.21)
project(Irregular_Accesses_Reorder_Unit)

set(CMAKE_CXX_STANDARD 20)
add_executable(Irregular_Accesses_Reorder_Unit DisjointSet.cpp IRU.cpp)

#add_executable(Irregular_Accesses_Reorder_Unit main.cpp IRU.cpp Arc.h Union_Find.cpp)
