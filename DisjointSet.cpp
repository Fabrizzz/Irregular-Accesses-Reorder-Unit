#include <unordered_map>
#include <iostream>

using namespace std;

class DisjointSet
{
private:
    std::unordered_map<int, int> parent;

    // stores the depth of trees
    std::unordered_map<int, int> rank;

    // perform MakeSet operation
public:
    void makeSet(int universe[], int size)
    {
        // create `n` disjoint sets (one for each item)
        for (int i = 0; i < size; i++)
        {
            parent.insert_or_assign(universe[i], universe[i]);
            rank.insert_or_assign(universe[i], 0);
        }
    }

    // Find the root of the set in which element `k` belongs
    int Find(int k)
    {
        int root = parent.at(k);
        if (root != k)
        {
            root = Find(root);
            parent.insert_or_assign(k, root);
        }

        return root;
    }

    // Perform Union of two subsets
    void Union(int a, int b)
    {
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
        }
        else if (rankx < ranky) {
            parent.insert_or_assign(x, y);
        }
        else {
            parent.insert_or_assign(x, y);
            rank.insert_or_assign(y, ranky + 1);
        }
    }

    static void printSets(int universe[], int size, DisjointSet& ds)
    {
        for (int i = 0; i < size; i++)
            cout << universe[i] << " " << ds.Find(universe[i]) << endl;
        cout << endl;
    }
/*
private:
    int main()
    {
        int universe[] = { 1, 2, 3, 4, 5 };
        int sizeUniverse = sizeof(universe)/sizeof(int);

        // initialize `DisjointSet` class
        DisjointSet ds{};

        // create a singleton set for each element of the universe
        ds.makeSet(universe, sizeUniverse);
        DisjointSet::printSets(universe, sizeUniverse, ds);

        ds.Union(4, 3);        // 4 and 3 are in the same set
        DisjointSet::printSets(universe, sizeUniverse, ds);

        ds.Union(2, 1);        // 1 and 2 are in the same set
        DisjointSet::printSets(universe, sizeUniverse, ds);

        ds.Union(1, 3);        // 1, 2, 3, 4 are in the same set
        DisjointSet::printSets(universe, sizeUniverse, ds);
        return 0;
    }*/
};


