#include <iostream>
#include <unordered_set>
#include <vector>
#include "IRU.cpp"
#include "DisjointSet.cpp"
#include "GraphGenerator.cpp"

using namespace std;

vector<vector<Arc>> loadGraph();

int main() {

    int number_of_nodes = 10000;
    double density_of_arcs = 0.5;

    int distances[number_of_nodes]; //array to be coalesced

    vector<int> frontier;
    vector<vector<int>> neighbours = GraphGenerator::create_graph(number_of_nodes,density_of_arcs);

    int number_of_warps = 2;
    int threads_per_warp = 32;

    for(int i = 0;i < number_of_nodes;i++)
        distances[i] = -1;
    distances[0] = 0;

    frontier.push_back(0);
    IRU iru{distances, frontier, threads_per_warp};

    bool usingIRU = true;
    int distance = 0;

    while (frontier.empty()) {
        iru.compute_hashes();
        distance++;

        for (int warp = 0; warp < number_of_warps; warp++) {
            for (int thread = 0; thread < threads_per_warp; thread++) {
                int pos;
                int current_node;

                if(!usingIRU)
                {
                    pos = warp * threads_per_warp + thread;
                    current_node = frontier[pos];
                }
                else
                {
                    bool loaded = iru.load_iru(current_node);
                    if (!loaded)
                        continue;
                }

                if(distances[current_node] == -1)
                {
                    distances[current_node] = distance;
                    // termina la parte con coalescenza

                    iru.add_to_frontier(neighbours[current_node]);
                }
            }
        }
    }

    return 0;
}
