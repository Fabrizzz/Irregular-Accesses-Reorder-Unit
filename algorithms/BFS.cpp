#include "BFS.h"

using namespace std;

double
BfsBenchmark(const vector<vector<int>> &neighbours, int startingNode, bool never_unite_buckets, int hashMapSize) {
    int threads_per_warp = 32;
    int number_of_warps = 2;
    int number_of_nodes = neighbours.size();
    int *distances = new int[number_of_nodes]();
    vector<int> frontier;
    int memoryLocationsAccessed = 0;
    int warpsAccesses = 0;

    for (int i = 0; i < number_of_nodes; i++)
        distances[i] = -1;

    frontier.push_back(startingNode);

    cout << "HashMapSize: " << hashMapSize << endl;
    IRU<int> iru(reinterpret_cast<int *>(distances), frontier, threads_per_warp, never_unite_buckets, hashMapSize);

    int distance = -1;
    // << "Starting BFS" << endl;
    iru.compute_hashes();

    do {
        int current_index = 0;
        distance++;
        for (int warp = 0; !iru.empty_frontier(); warp = warp == number_of_warps - 1 ? 0 : warp + 1) {
            warpsAccesses++;
            set<std::size_t> memoryLocations;

            for (int thread = 0; thread < threads_per_warp; thread++) {
                bool loaded = true;
                loaded = iru.load_iru(current_index);
                if (!loaded) {
                    if (thread == 0) {
                        warp--;
                        warpsAccesses--;
                    }
                    break;
                }
                memoryLocations.insert(((std::size_t) (distances + current_index)) >> bits_to_ignore);
                //cout << "current index: "<< current_index<<endl;
                //iru.print_frontier();
                if (distances[current_index] == -1) {
                    distances[current_index] = distance;
                    //here terminates the optimized block
                    auto &current_neighbours = const_cast<vector<int> &>(neighbours[current_index]);
                    frontier.insert(frontier.end(), current_neighbours.begin(), current_neighbours.end());
                }
            }
            memoryLocationsAccessed += memoryLocations.size();

        }
        //cout << "The " << distance << " level of BFS completed" << endl;
        iru.compute_hashes();

        //iru.print_frontier();
    } while (!iru.empty_frontier());

    for (int i = 1; i < number_of_nodes; i++) {
        if (distances[i] == -1) {
            cout << "Graph is not connected" << endl;
            break;
        }
    }

    delete[](distances);
    cout << "MemAccesses " << memoryLocationsAccessed << endl;
    cout << "WarpsAccesses " << warpsAccesses << endl;
    return (double) memoryLocationsAccessed / warpsAccesses;
}

double BfsBenchmarkWithoutIru(const vector<vector<int>> &neighbours, int startingNode) {
    int threads_per_warp = 32;
    int number_of_warps = 2;
    int number_of_nodes = neighbours.size();
    int *distances = new int[number_of_nodes]();
    vector<int> frontier;
    vector<int> nextFrontier;
    int memoryLocationsAccessed = 0;
    int warpsAccesses = 0;

    for (int i = 0; i < number_of_nodes; i++)
        distances[i] = -1;

    frontier.push_back(startingNode);
    int distance = -1;
    cout << "Starting BFS" << endl;

    do {
        int current_index = 0;
        distance++;
        for (int warp = 0; current_index < frontier.size(); warp = warp == number_of_warps - 1 ? 0 : warp + 1) {
            warpsAccesses++;
            set<std::size_t> memoryLocations;

            for (int thread = 0; thread < threads_per_warp; thread++, current_index++) {
                if (current_index >= frontier.size()) {
                    if (thread == 0) {
                        warp--;
                        warpsAccesses--;
                    }
                    break;
                }
                int nodePosition = frontier[current_index];
                memoryLocations.insert(((std::size_t) (distances + nodePosition)) >> bits_to_ignore);
                //cout << "current index: "<< nodePosition<<endl;
                //iru.print_frontier();
                if (distances[nodePosition] == -1) {
                    distances[nodePosition] = distance;
                    //here terminates the optimized block
                    auto &current_neighbours = const_cast<vector<int> &>(neighbours[nodePosition]);

                    nextFrontier.insert(nextFrontier.end(), current_neighbours.begin(), current_neighbours.end());
                }
            }
            memoryLocationsAccessed += memoryLocations.size();

        }
        //cout << "The " << distance << " level of BFS completed" << endl;
        frontier.swap(nextFrontier);
        nextFrontier.clear();
    } while (!frontier.empty());

    for (int i = 1; i < number_of_nodes; i++) {
        if (distances[i] == -1) {
            cout << "Graph is not connected" << endl;
            break;
        }
    }

    delete[](distances);
    cout << "MemAccesses " << memoryLocationsAccessed << endl;
    cout << "WarpsAccesses " << warpsAccesses << endl;
    return (double) memoryLocationsAccessed / warpsAccesses;
}