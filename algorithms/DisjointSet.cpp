#include <unordered_map>
#include <iostream>
#include <vector>

using namespace std;

class DisjointSet {
private:
    std::unordered_map<int, int> parent;

    // stores the depth of trees
    std::unordered_map<int, int> rank;

    // perform MakeSet operation
public:
    void makeSet(const vector<int> &universe) {
        // create `n` disjoint sets (one for each item)
        for (const int &i: universe) {
            parent.insert_or_assign(i, i);
            rank.insert_or_assign(i, 0);
        }
    }

    // Find the root of the set in which element `k` belongs
    int Find(int k) {
        int root = parent.at(k);
        if (root != k) {
            root = Find(root);
            parent.insert_or_assign(k, root);
        }

        return root;
    }

    // Perform Union of two subsets
    void Union(int a, int b) {
        // Find the root of the sets in which elements
        // `x` and `y` belongs
        int x = Find(a);
        int y = Find(b);

        // if `x` and `y` are present in the same set
        if (x == y) {
            return;
        }

        int rankx = rank.at(x);
        int ranky = rank.at(y);
        if (rankx > ranky) {
            parent.insert_or_assign(y, x);
        } else if (rankx < ranky) {
            parent.insert_or_assign(x, y);
        } else {
            parent.insert_or_assign(x, y);
            rank.insert_or_assign(y, ranky + 1);
        }
    }

    static void printSets(const vector<int> &universe, DisjointSet &ds) {
        for (int i: universe)
            cout << i << " " << ds.Find(i) << endl;
        cout << endl;
    }

};


