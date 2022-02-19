#include <vector>
#include "DisjointSet.cpp"

using namespace std;

class GraphGenerator{
public:
    
    static vector<vector<int>> create_graph(int n, double density){
        vector<vector<int>> graph{};

        for (int i = 0; i < n; i++) {
            graph.push_back({});
        }
        int universe[n];
        for (int i = 0; i < n; i++) {
            universe[i] = i;
        }
        DisjointSet ds = DisjointSet();
        ds.makeSet(universe, n);
        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                if ((double) rand() / (RAND_MAX) < density){
                    graph[universe[i]].push_back(universe[j]);
                    graph[universe[j]].push_back(universe[i]);
                    ds.Union(i,j);
                }
            }
        }
    
        for (int i = 0; i < n-1; i++) {
            if (ds.Find(universe[i]) != ds.Find(universe[i+1])){
                //cout << "sus" << endl;
                graph[universe[i]].push_back(universe[i+1]);
                graph[universe[i+1]].push_back(universe[i]);
                ds.Union(universe[i],universe[i+1]);
            }
        }

        return graph;
    }

    void print(int size, vector<vector<int>> graph_0){

        for(int i = 0;i < size;i++) {
            cout << i << " -> ";
            for (int j = 0; j < graph_0[i].size(); j++)
                cout << graph_0[i][j] << " ";
            cout << endl;
        }
    }
};
/*
int main (){
    GraphGenerator gg = {};

    vector<vector<int>> graph = gg.create_graph(10000, 0.5);

    return 0;
}*/