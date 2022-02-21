#include <cstdint>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <set>
#include <deque>
#include <algorithm>


using namespace std;

//todo set this log(size_of_buckets)+log(sizeof(T))+1
const static int bits_to_ignore = 8; //Array of int, 4 bytes each. I want to fill buckets of 32 elements 2^5

template<typename T>
class IRU {

    vector<int> &frontier; //frontier
    int size_of_bucket;
    deque<set<int>> list_of_buckets;
    bool usingLastBucket = false;
    bool one_bucket_per_warp;  //never mix two non-full list_of_buckets to try to create a new full one

    int *secondary_array = nullptr; //todo associate each element of the secondary to each element of the frontier when reordering the frontier
    void (*filter_op)(int,
                      int) = nullptr; //todo apply this function to the secondary array each time two elements of the frontier are equals

    class MyHash {
        T *arr;
    public:
        explicit MyHash(T *arr) : arr(arr) {}

        std::size_t operator()(int i) const {
            return ((std::size_t) (arr + i)) >> bits_to_ignore;
        }
    };

    class MyKeyEquals {
        T *arr;
    public:
        explicit MyKeyEquals(T *arr) : arr(arr) {}

        bool operator()(int i, int j) const {
            return (((std::size_t) (arr + i)) >> bits_to_ignore) == (((std::size_t) (arr + j)) >> bits_to_ignore);
        }
    };

    //todo sort the set<int> by the most 32-8 significant bits
    unordered_map<int, set<int>, MyHash, MyKeyEquals> map_of_buckets;

public:
    IRU(T *targetArray, vector<int> &indicesArray, int sizeOfBucket, bool never_unite_buckets) :
            frontier(indicesArray),
            size_of_bucket(sizeOfBucket),
            map_of_buckets(200, MyHash(targetArray), MyKeyEquals(targetArray)),
            one_bucket_per_warp(never_unite_buckets) {}

    //
    bool load_iru(int &index) {
        if (usingLastBucket) {
            set<int> &currentBucket = list_of_buckets.back();
            if (currentBucket.empty()) {//this returns false everytime a warp completes a bucket
                list_of_buckets.pop_back();
                usingLastBucket = false;
                return false;
            }
            index = *currentBucket.begin();
            currentBucket.erase(currentBucket.begin());
            return true;
        } else {
            if (list_of_buckets.empty())
                return false;
            usingLastBucket = true;

            if (!one_bucket_per_warp) {
                //merges the biggest bucket with the little ones until it contains size_of_bucket elements
                set<int> &currentBucket = list_of_buckets.back();
                while (currentBucket.size() < size_of_bucket && currentBucket != list_of_buckets.front() &&
                       currentBucket.size() + list_of_buckets.front().size() <= size_of_bucket) {
                    currentBucket.insert(list_of_buckets.front().begin(), list_of_buckets.front().end());
                    list_of_buckets.pop_front();
                }
            }
            return load_iru(index);
        }


    }

    bool empty_frontier() {
        return list_of_buckets.empty();
    }

    // compute all the hashes of the frontier
    void compute_hashes() {
        for (int f : frontier) {
            set<int> &bucket = map_of_buckets[f];
            map_of_buckets[f].insert(f);

            if (bucket.size() >= size_of_bucket) {
                list_of_buckets.push_back(bucket);
                map_of_buckets.erase(f);
            }

        }
        cout << "number of partial buckets: " << map_of_buckets.size() << '\n';
        cout << "number of complete buckets: " << list_of_buckets.size() << '\n';

        for (auto&&[_, second] : map_of_buckets) {
            list_of_buckets.push_back(second);
        }
        frontier.clear();
        map_of_buckets.clear();
        sort(list_of_buckets.begin(), list_of_buckets.end(), [](const set<int> &a, const set<int> &b) {
            return (a.size() < b.size());
        });


    }

    void print_frontier() {
        for (const auto &buck: list_of_buckets) {
            for (auto index : buck) {
                cout << index << " ";
            }
            cout << endl;
        }
    }

};

/*int main(){
    int n = 200;
    vector<int> frontier(40);
    int distances[200];



    IRU<int> iru(distances,n, frontier, 32);

    for (int i = 0; i < 40; i++)
        frontier.push_back(rand()%200);

    iru.compute_hashes();
    //iru.print_frontier();

    while (!iru.empty_frontier()) {
        int pos;

        iru.print_frontier();
        bool loaded = iru.load_iru(pos);
        if (loaded) {
            cout << pos << endl;
        }
        cout << endl;
    }



    return 0;

}*/