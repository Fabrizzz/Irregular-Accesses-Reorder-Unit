#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include "DisjointSet.cpp"

using namespace std;

class GraphGenerator {
public:

    static vector<vector<int>> load_graph_matrix_market(const string &path) {
        ifstream newfile;
        newfile.open(path,
                     ios::in); //open a file to perform read operation using file object
        if (newfile.is_open()) { //checking whether the file is open
            string tp;
            while (getline(newfile, tp)) {
                if (!tp.starts_with('%'))
                    break;
            }
            int n;
            stringstream stream(tp);
            stream >> n;
            vector<vector<int>> graph(n + 1);

            while (getline(newfile, tp)) {
                stringstream stream(tp);
                int firstNode;
                int secondNode;
                stream >> firstNode;
                stream >> secondNode;
                graph[firstNode].push_back(secondNode);
                graph[secondNode].push_back(firstNode);
            }
            newfile.close(); //close the file object.
            return graph;
        }
        cout << "FILE NOT FOUND: " << path << endl;
        exit(1);
    }

    static vector<vector<int>> load_graph_Metis(const string &path) {
        ifstream newfile;
        newfile.open(path,
                     ios::in); //open a file to perform read operation using file object
        if (newfile.is_open()) { //checking whether the file is open
            string tp;
            int n;
            newfile >> n;
            vector<vector<int>> graph;
            graph.reserve(n + 1);
            graph.emplace_back();//because graph is 1 indexed;
            getline(newfile, tp);

            while (getline(newfile, tp)) {
                stringstream stream(tp);
                std::vector<int> values(
                        (std::istream_iterator<int>(stream)), // begin
                        (std::istream_iterator<int>()));
                graph.push_back(values);
            }
            newfile.close(); //close the file object.
            return graph;
        }
        cout << "FILE NOT FOUND: " << path << endl;
        exit(1);

    }

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

};
