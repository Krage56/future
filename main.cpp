#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <future>
#include <chrono>
#include <list>
using namespace  std;
using const_iter = vector<long long>::const_iterator;
size_t countOnes(long long n){
    size_t count = 0;
    while(n){
        ++count;
        n = n & (n - 1);
    }
    return count;
}

[[nodiscard]]size_t vectorOneBitsCount(const_iter cbeg, const_iter cend){
    size_t overall_count = 0;
    for_each(cbeg, cend, [&overall_count](long long item){overall_count += countOnes(item);});
    return overall_count;
}

void fullVector(vector<long long>& v){
    srand(time(NULL));
    const long long lim = pow(10,5);
    for_each(v.begin(), v.end(), [lim](long long &item){item = rand() % lim;});
}

//void printOrder(size_t threads, )


void accumulate(const vector<long long>&v, size_t threads){
    size_t piece_cap = v.size() / threads;
    size_t remainder = v.size() % threads;
    auto beg = v.cbegin();
    list<future<size_t>> l_futures;
    chrono::time_point start = chrono::steady_clock::now();
    for(size_t i = 0; i < threads; ++i){
        l_futures.emplace_back(async(vectorOneBitsCount, beg, beg + ((remainder? 1: 0) + piece_cap)));
        beg += ((remainder? 1: 0) + piece_cap);
        if(remainder){
            --remainder;
        }
    }
    size_t overall_count = 0;
    for_each(l_futures.begin(), l_futures.end(), [&overall_count](auto& f){overall_count += f.get();});
    chrono::time_point end = chrono::steady_clock::now();
    chrono::duration<double> dur = end - start;
    cout << dur.count() << endl;
    //cout << overall_count << endl;
}
int main() {
    vector<long long> random_valued(pow(10,6));
    fullVector(random_valued);
//    size_t count = vectorOneBitsCount(random_valued.cbegin(), random_valued.cend());
//    cout << count << endl;
    const int threads = 10;
    for(int i = 1; i <= threads; ++i){
        cout << i << " ";
        accumulate(random_valued, i);
    }
    //accumulate(random_valued, 5);
    return 0;
}
