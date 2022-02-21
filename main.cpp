#include "IRU.cpp"
#include "GraphGenerator.cpp"

using namespace std;


int main() {

    int number_of_nodes = 1000; //todo take this 4 parameters from stdin
    double density_of_arcs = 0.01;
    int number_of_warps = 2;
    int threads_per_warp = 32;

    //todo can i use a vector instead?
    int *distances = new int[number_of_nodes]();

    vector<int> frontier;
    vector<vector<int>> neighbours = GraphGenerator::create_random_graph(number_of_nodes, density_of_arcs);

    for (int i = 0; i < number_of_nodes; i++)
        distances[i] = -1;

    frontier.push_back(0);
    IRU<int> iru(reinterpret_cast<int *>(distances), frontier, threads_per_warp, false);

    int distance = -1;
    cout << "Starting BFS" << endl;
    iru.compute_hashes();

    do {

        distance++;

        for (int warp = 0; !iru.empty_frontier(); warp = warp == number_of_warps - 1 ? 0 : warp + 1) {
            for (int thread = 0; thread < threads_per_warp; thread++) {
                int current_index;
                bool loaded = iru.load_iru(current_index);
                //todo test BFS without the IRU to see the difference
                if (!loaded) {
                    if (thread == 0)
                        warp--;
                    break;
                }

                //cout << "current index: "<< current_index<<endl;
                //iru.print_frontier();
                if (distances[current_index] == -1) {
                    distances[current_index] = distance;
                    //here terminates the optimized block
                    vector<int> &current_neighbours = neighbours[current_index];
                    for (int i : current_neighbours) {

                        if (distances[i] == -1)
                            frontier.push_back(i);
                    }
                    //todo should i add all the neighbours to the frontier to avoid accessing the distances vector uncoalesced?
                    //frontier.insert(frontier.end(),current_neighbours.begin(),current_neighbours.end());
                }
            }
        }
        cout << "The " << distance << " level of BFS completed" << endl;
        iru.compute_hashes();
        //iru.print_frontier();
    } while (!iru.empty_frontier());

    for (int i = 0; i < number_of_nodes; i++) {
        if (distances[i] == -1) {
            cout << "Something is wrong, at least a Node wasn't visited";
            delete[](distances);
            return 1;
        }
    }

    delete[](distances);
    return 0;
}
