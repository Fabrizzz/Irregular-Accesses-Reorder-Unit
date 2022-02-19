#include <cstdint>
#include <vector>
#include "Arc.h"
#include <unordered_map>

using namespace std;

template <typename T>
class IRU {
    T& target_array;
    vector<int>& indices_array; //frontier
    int size_of_bucket;

    int* secondary_array = nullptr;
    void (*filter_op)(int,int) = nullptr;


    class MyHashFunction {
    public:
        size_t operator()(const pair<T&, int>& p) const
        {
            return (&(p.first[p.second])) >> 5;
        }
    };

    unordered_map<pair<T, int>, vector<int>, MyHashFunction> um;

public:
    IRU(T& targetArray, vector<int> &indicesArray, int sizeOfBucket) : target_array(targetArray),
                                                                            indices_array(indicesArray),
                                                                            size_of_bucket(sizeOfBucket) {}

    //
    bool load_iru (Arc &index){ //value loaded from the frontier
        return true;
    }

    // compute all the hashes of the frontier
    void compute_hashes(){
        for(int f : indices_array)
        {
            um.at({target_array,f}).push_back(f);
        }
    }

};
int main(){
    vector<int> frontier = { 1, 2, 3, 4, 0 };
    int distances[] = { 1, 2, 3, 4, 5 };

    IRU iru(distances, frontier, 32);

    iru.compute_hashes();

    return 0;

}