#include <vector>
#include "IRU.cpp"

double BfsBenchmark(const vector<vector<int>> &neighbours, int startingNode, bool never_unite_buckets, int hashMapSize);

double BfsBenchmarkWithoutIru(const vector<vector<int>> &neighbours, int startingNode);