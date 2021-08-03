#include <bits/stdc++.h>
#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <vector.hpp>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>

using namespace std;

constexpr static int C = 10;

template<class T>
using uvector = uwr::static_vector<T, C>;
template<class T>
using bvector = boost::container::static_vector<T, C>;

template<class Container>
void print(const Container& c) {
    for (const auto& x : c)
        cout << x << " ";
    cout << endl;
}

int main() {
    using T = test_type;
    uwr::vector<T> v { 1 };

    ON;
    
    OFF;

    return 0;
}
