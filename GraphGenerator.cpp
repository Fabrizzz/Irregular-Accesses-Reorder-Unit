#include <vector>
#include "DisjointSet.cpp"

using namespace std;

class GraphGenerator {
public:
    //Erdős–Rényi model
    static vector<vector<int>> create_random_graph(int n, double density) {//density [0,1]
        vector<vector<int>> graph(n);

        vector<int> universe(n);

        for (int i = 0; i < n; i++) {
            universe[i] = i;
        }

        DisjointSet ds = DisjointSet();
        ds.makeSet(universe);

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                //todo change with a better rand function
                if ((double) rand() / (RAND_MAX) < density) {
                    graph[i].push_back(j);
                    graph[j].push_back(i);
                    ds.Union(i, j);
                }
            }
        }

        //connect components
        for (int i = 0; i < n - 1; i++) {
            if (ds.Find(i) != ds.Find(i + 1)) {
                graph[i].push_back(i + 1);
                graph[i + 1].push_back(i);
                ds.Union(i, i + 1);
            }
        }

        return graph;
    }

    static void print(const vector<vector<int>> &graph_0) {

        for (int i = 0; i < graph_0.size(); i++) {
            cout << i << " -> ";
            for (int j : graph_0[i])
                cout << j << " ";
            cout << endl;
        }
    }
};

/*int main (){
    create_random_graph(10000, 0.0);

    return 0;
}*/