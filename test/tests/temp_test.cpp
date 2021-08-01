#include <bits/stdc++.h>
#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
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
    using Vector1 = boost::container::static_vector<T, C>;
    using Vector2 = uwr::static_vector<T, C>;
    
    const T val = 3;
    Vector2 v {3, 1, 2, 3, 2, 3, 2};

    ON;
    
    erase_if(v, [&](const auto& x){ return x == val; });

    OFF;
 
    cout << v << endl;

    return 0;
}
