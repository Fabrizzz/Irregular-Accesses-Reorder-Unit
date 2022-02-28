#include <cstdint>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <set>
#include <deque>
#include <algorithm>


using namespace std;

const static int bits_to_ignore = 8; //Array of int, 4 bytes each. I want to fill buckets of 32 elements 2^5
//1 + log2(size_of_buckets) + log2(sizeof(int));

template<typename T>
class IRU {

    const int number_of_buckets;
    vector<int> &frontier; //frontier
    int size_of_bucket;
    deque<set<int>> list_of_buckets;
    bool usingLastBucket = false;
    bool one_bucket_per_warp;  //never mix two non-full list_of_buckets to try to create a new full one


    class MyHash {
        T *arr;
    public:
        explicit MyHash(T *arr) : arr(arr) {}
        std::size_t operator()(int i) const {
            return ((std::size_t) (arr + i)) >> bits_to_ignore;
        }
    };

    vector<set<int>> map_of_buckets;
    MyHash hashingFunction;

public:
    IRU(T *targetArray, vector<int> &indicesArray, int sizeOfBucket, bool never_unite_buckets, int number_of_buckets) :
            frontier(indicesArray),
            size_of_bucket(sizeOfBucket),
            map_of_buckets(number_of_buckets),
            one_bucket_per_warp(never_unite_buckets),
            hashingFunction(targetArray),
            number_of_buckets(number_of_buckets) {}

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
        for (int f: frontier) {
            int hashValue = hashingFunction.operator()(f) % number_of_buckets;
            set<int> &bucket = map_of_buckets[hashValue];
            bucket.insert(f);

            if (bucket.size() >= size_of_bucket) {
                list_of_buckets.template emplace_back(bucket);
                bucket.clear();
            }

        }
        //cout << "number of complete buckets: " << list_of_buckets.size() << '\n';

        for (auto &second: map_of_buckets) {
            if (!second.empty()) {
                list_of_buckets.emplace_back(second);
                second.clear();
            }
        }
        frontier.clear();
        sort(list_of_buckets.begin(), list_of_buckets.end(), [](const set<int> &a, const set<int> &b) {
            return (a.size() < b.size());
        });


    }

    void print_frontier() {
        for (const auto &buck: list_of_buckets) {
            for (auto index: buck) {
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