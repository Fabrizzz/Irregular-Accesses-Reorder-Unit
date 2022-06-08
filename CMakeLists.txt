cmake_minimum_required(VERSION 3.19.2)
project(Irregular_Accesses_Reorder_Unit)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
add_executable(Irregular_Accesses_Reorder_Unit algorithms/DisjointSet.cpp algorithms/GraphGenerator.cpp algorithms/IRU.cpp main.cpp algorithms/BFS.cpp algorithms/BFS.h)
FetchContent_Declare(
        graph1
        URL https://suitesparse-collection-website.herokuapp.com/MM/Newman/cond-mat-2005.tar.gz
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
)
FetchContent_Declare(
        graph2
        URL https://suitesparse-collection-website.herokuapp.com/MM/Belcastro/human_gene1.tar.gz
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
)
FetchContent_Declare(
        graph3
        URL https://suitesparse-collection-website.herokuapp.com/MM/INPRO/msdoor.tar.gz
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
)
FetchContent_Declare(
        graph4
        URL https://www.cc.gatech.edu/dimacs10/archive/data/delaunay/delaunay_n19.graph.bz2
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        DOWNLOAD_NO_EXTRACT true
)
FetchContent_Declare(
        graph5
        URL https://www.cc.gatech.edu/dimacs10/archive/data/kronecker/kron_g500-simple-logn18.graph.bz2
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        DOWNLOAD_NO_EXTRACT true
)

FetchContent_MakeAvailable(graph1 graph2 graph3 graph4 graph5)

execute_process(
COMMAND bzip2 -dk _deps/graph4-src/delaunay_n19.graph.bz2
COMMAND bzip2 -dk _deps/graph5-src/kron_g500-simple-logn18.graph.bz2
)

file(MAKE_DIRECTORY graphs)
file(RENAME _deps/graph1-src/cond-mat-2005.mtx graphs/cond-mat-2005.mtx)
file(RENAME _deps/graph2-src/human_gene1.mtx graphs/human_gene1.mtx)
file(RENAME _deps/graph3-src/msdoor.mtx graphs/msdoor.mtx)
file(RENAME _deps/graph4-src/delaunay_n19.graph graphs/delaunay_n19.graph)
file(RENAME _deps/graph5-src/kron_g500-simple-logn18.graph graphs/kron_g500-simple-logn18.graph)
