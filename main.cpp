#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <future>
#include <chrono>
#include <list>
#include <tuple>
#include <iomanip>
using namespace  std;

using const_iter = vector<long long>::const_iterator;
size_t countOnes(long long n){
    size_t count = 0;
    while(n){
        ++count;
        n = (n & (n - 1));
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

void printUnderline(size_t len){
    cout << setfill('_') << setw(len);
    cout << "\n" << flush;
    cout << setfill(' ') << flush;
}
void printTitle(size_t underline_w){
    printUnderline(underline_w);
    cout << "| " << setw(9);
    cout << "Quantity of threads" << " | " << flush;
    cout << "Duration (in sec)" << " |" << setw(8);
    cout << "Result" << setw(3) << "|" << endl << flush;
}
size_t countNums(size_t num){
    size_t count = 0;
    if(!num){
        return 1;
    }
    while(num){
        ++count;
        num /= 10;
    }
    return count;
}
void printOrder(size_t threads, double duration, size_t result, size_t underline_w){
    printUnderline(underline_w);
    cout << "|" << flush;
    cout << setw(11) << threads;
    cout << setw(11) << "|"  << flush;
    cout.setf(ios::fixed);
    cout << setw(15) << setprecision(7) << duration << setw(5) << "|" << flush;
    int count_nums = countNums(result);
    int delta = (10 - count_nums)/2;
    cout << setw(count_nums + delta) << result  << setw(11-(count_nums + delta)) << "|" << endl;
}


[[nodiscard]]tuple<size_t, double> accumulate(const vector<long long>&v, size_t threads){
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
    return make_tuple(overall_count, dur.count());
}

int main() {
    vector<long long> random_valued(pow(10,6));
    fullVector(random_valued);
    const int threads = 15;
    const size_t underline_w = 55;
    printTitle(underline_w);
    for(int i = 1; i <= threads; ++i){
        size_t count;
        double duration;
        std::tie(count, duration) = accumulate(random_valued, i);
        printOrder(i , duration, count, underline_w);
    }
    printUnderline(underline_w);
    return 0;
}
