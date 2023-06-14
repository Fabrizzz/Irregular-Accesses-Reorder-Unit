# Irregular Accesses Reorder Unit (IRU) CPU Implementation

This project implements the Irregular Accesses Reorder Unit (IRU),
originally designed for GPU computing, as a CPU implementation. The IRU algorithm 
aims to optimize memory coalescing and performance for graph-based workloads on 
GPGPU architectures. The CPU implementation of the IRU algorithm allows developers 
to explore the algorithm's effectiveness and performance on CPU platforms.
You can find all the detail about the original work on this paper:
[An Irregular Accesses Reorder Unit for Efficient Execution of Irregular Computations on GPGPU](https://arxiv.org/abs/2007.07131)

## Features

- Implements the BFS (Breadth-First Search) algorithm with the IRU optimization.
- Provides a reference implementation of the BFS algorithm without the IRU optimization 
- for performance comparison.
- Utilizes the Disjoint-Set data structure for union-find operations.
- Includes the GraphGenerator class for loading graph data from various file formats or 
- generating random graphs.
- Supports loading graph data from Matrix Market or Metis file formats.
- Offers the ability to create random graphs with a specified number of nodes and density.
- Computes and prints average uncoalesced accesses per warp, demonstrating the algorithm's
- performance.

## Prerequisites

To compile and run the project, ensure that you have the following installed:

- C++ compiler with C++20 support
- CMake 3.19.2 or higher
- Bzip2

## Dependencies

This project has the following dependencies, which are automatically fetched and included during the CMake build process:

- [SuiteSparse Matrix Collection (cond-mat-2005)](https://sparse.tamu.edu/Newman/cond-mat-2005)
- [SuiteSparse Matrix Collection (human_gene1)](https://sparse.tamu.edu/Belcastro/human_gene1)
- [SuiteSparse Matrix Collection (msdoor)](https://sparse.tamu.edu/INPRO/msdoor)
- [DIMACS10 Graph Collection (delaunay_n19)](https://www.cc.gatech.edu/dimacs10/archive/delaunay.shtml)
- [DIMACS10 Graph Collection (kron_g500-simple-logn18)](https://www.cc.gatech.edu/dimacs10/archive/kronecker.shtml)

## Build Instructions

To build the project, follow these steps:

1. Make sure you have CMake version 3.19.2 or higher installed on your system.
2. Create a build directory and navigate to it: `mkdir build && cd build`.
3. Run CMake to generate the build files: `cmake ..`.
4. Build the project: `cmake --build .`.

## Usage

After building the project, you can run the simulation using the generated executable. 

`./Irregular_Accesses_Reorder_Unit`


The simulation performs BFS with the IRU algorithm on different graph datasets. The average uncoalesced accesses per warp is printed for each configuration.

The main entry point of the simulation is the `main.cpp` file. In this file, you can modify the graph datasets and starting nodes to test different scenarios. The `testBench` function is used to perform the BFS benchmark with the IRU algorithm and print the results.

```cpp
int main() {
    // Create or load graph data
    vector<vector<int>> neighbours = GraphGenerator::create_random_graph(10000, 0.01);
    testBench("RandomGeneratedGraph", neighbours, 0);

    // Load graph data from Matrix Market file
    neighbours = GraphGenerator::load_graph_matrix_market("./graphs/cond-mat-2005.mtx");
    testBench("cond-mat-2005", neighbours, 1);

    // Load graph data from Matrix Market file
    neighbours = GraphGenerator::load_graph_matrix_market("./graphs/human_gene1.mtx");
    testBench("human_gene1", neighbours, 25);

    // Load graph data from Matrix Market file
    neighbours = GraphGenerator::load_graph_matrix_market("./graphs/msdoor.mtx");
    testBench("msdoor", neighbours, 1);

    // Load graph data from Metis file
    neighbours = GraphGenerator::load_graph_Metis("./graphs/delaunay_n19.graph");
    testBench("delaunay_n19", neighbours, 1);

    // Load graph data from Metis file
    neighbours = GraphGenerator::load_graph_Metis("./graphs/kron_g500-simple-logn18.graph");
    testBench("kron_g500-simple-logn18", neighbours, 1);

    return 0;
}
```
Please note that this implementation is intended for CPU execution, 
emulating the behavior of the IRU algorithm designed for GPGPU architectures. 
The original implementation of the IRU
for GPGPU can be found in the paper IRU: 
[An Irregular Accesses Reorder Unit for Efficient Execution of Irregular Computations on GPGPU](https://arxiv.org/abs/2007.07131).
