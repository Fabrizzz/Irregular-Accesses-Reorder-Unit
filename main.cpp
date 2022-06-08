#include "algorithms/BFS.h"

#include "algorithms/GraphGenerator.cpp"


using namespace std;

void testBench(string path, vector<vector<int>> neighbours, int startFrom) {
    int hashMapDimensioning[] = {32, 100, 500};
    cout << "Testing of " << path << endl;
    cout << "BFS without IRU:" << endl;
    cout << "Average Uncoalesced Accesses per warp: " << BfsBenchmarkWithoutIru(neighbours, startFrom) << endl;
    for (int val : hashMapDimensioning) {
        int size = neighbours.size() / val;
        cout << "Using IRU with bucket Union Optimization:" << endl;
        cout << "Average Uncoalesced Accesses per warp: " << BfsBenchmark(neighbours, startFrom, false, size) << endl;
        cout << "Using IRU without Optimization:" << endl;
        cout << "Average Uncoalesced Accesses per warp: " << BfsBenchmark(neighbours, startFrom, true, size) << endl;
    }
    cout << endl;


}

int main() {
    vector<vector<int>> neighbours = GraphGenerator::create_random_graph(10000, 0.01);
    testBench("RandomGeneratedGraph", neighbours, 0);
    //https://sparse.tamu.edu/Newman/cond-mat-2005
    neighbours = GraphGenerator::load_graph_matrix_market("./graphs/cond-mat-2005.mtx");
    testBench("cond-mat-2005", neighbours, 1);
    //https://sparse.tamu.edu/Belcastro/human_gene1
    neighbours = GraphGenerator::load_graph_matrix_market("./graphs/human_gene1.mtx");
    testBench("human_gene1", neighbours, 25);
    //https://sparse.tamu.edu/INPRO/msdoor
    neighbours = GraphGenerator::load_graph_matrix_market("./graphs/msdoor.mtx");
    testBench("msdoor", neighbours, 1);
    //https://www.cc.gatech.edu/dimacs10/archive/delaunay.shtml
    neighbours = GraphGenerator::load_graph_Metis("./graphs/delaunay_n19.graph");
    testBench("delaunay_n19", neighbours, 1);
    //https://www.cc.gatech.edu/dimacs10/archive/kronecker.shtml
    neighbours = GraphGenerator::load_graph_Metis("./graphs/kron_g500-simple-logn18.graph");
    testBench("kron_g500-simple-logn18", neighbours, 1);

    return 0;
}

