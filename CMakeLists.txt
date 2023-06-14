cmake_minimum_required(VERSION 3.19.2)
project(Irregular_Accesses_Reorder_Unit)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
add_executable(Irregular_Accesses_Reorder_Unit algorithms/DisjointSet.cpp algorithms/GraphGenerator.cpp algorithms/IRU.cpp main.cpp algorithms/BFS.cpp algorithms/BFS.h)

FetchContent_Declare(
        graph1
        URL https://suitesparse-collection-website.herokuapp.com/MM/Newman/cond-mat-2005.tar.gz
)
FetchContent_Declare(
        graph2
        URL https://suitesparse-collection-website.herokuapp.com/MM/Belcastro/human_gene1.tar.gz
)
FetchContent_Declare(
        graph3
        URL https://suitesparse-collection-website.herokuapp.com/MM/INPRO/msdoor.tar.gz
)
FetchContent_Declare(
        graph4
        URL https://www.cc.gatech.edu/dimacs10/archive/data/delaunay/delaunay_n19.graph.bz2
        DOWNLOAD_NO_EXTRACT true
)
FetchContent_Declare(
        graph5
        URL https://www.cc.gatech.edu/dimacs10/archive/data/kronecker/kron_g500-simple-logn18.graph.bz2
        DOWNLOAD_NO_EXTRACT true
)

FetchContent_GetProperties(graph1 graph2 graph3 graph4 graph5)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/graphs)

if(NOT graph1_POPULATED)
  FetchContent_Populate(graph1)
endif()

if(EXISTS ${graph1_SOURCE_DIR}/cond-mat-2005.mtx AND NOT EXISTS ${CMAKE_BINARY_DIR}/graphs/cond-mat-2005.mtx)
    file(RENAME ${graph1_SOURCE_DIR}/cond-mat-2005.mtx  ${CMAKE_BINARY_DIR}/graphs/cond-mat-2005.mtx)
endif()

if(NOT graph2_POPULATED)
  FetchContent_Populate(graph2)
endif()

if(EXISTS ${graph2_SOURCE_DIR}/human_gene1.mtx AND NOT EXISTS ${CMAKE_BINARY_DIR}/graphs/human_gene1.mtx)
    file(RENAME ${graph2_SOURCE_DIR}/human_gene1.mtx ${CMAKE_BINARY_DIR}/graphs/human_gene1.mtx)
endif()

if(NOT graph3_POPULATED)
  FetchContent_Populate(graph3)
endif()

if(EXISTS ${graph3_SOURCE_DIR}/msdoor.mtx AND NOT EXISTS ${CMAKE_BINARY_DIR}/graphs/msdoor.mtx)
    file(RENAME ${graph3_SOURCE_DIR}/msdoor.mtx ${CMAKE_BINARY_DIR}/graphs/msdoor.mtx)
endif()

if(NOT graph4_POPULATED)
  FetchContent_Populate(graph4)
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/graphs/delaunay_n19.graph)
    execute_process(
        COMMAND bzip2 -dk ${graph4_SOURCE_DIR}/delaunay_n19.graph.bz2
      )
      file(RENAME ${graph4_SOURCE_DIR}/delaunay_n19.graph ${CMAKE_BINARY_DIR}/graphs/delaunay_n19.graph)
endif()

if(NOT graph5_POPULATED)
  FetchContent_Populate(graph5)
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/graphs/kron_g500-simple-logn18.graph)
      execute_process(
          COMMAND bzip2 -dk ${graph5_SOURCE_DIR}/kron_g500-simple-logn18.graph.bz2
        )
        file(RENAME ${graph5_SOURCE_DIR}/kron_g500-simple-logn18.graph ${CMAKE_BINARY_DIR}/graphs/kron_g500-simple-logn18.graph)
  endif()
