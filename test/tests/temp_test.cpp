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

    // Vector1 v1 { 1, 2, 3, 4, 5 };
    // Vector2 v2 { 1, 2, 3, 4, 5 };
    // const test_type val = 13;
    // int pos = 0;

    ON;
    
    uvector<int> v1(5);
    print(v1);

    bvector<int> v2(5);
    print(v2);

    vector<int> v3(5);
    print(v3);

    // auto s = sizeof(test_type);
    // int n = 5;
    // char t[n * s];
    // std::uninitialized_default_construct_n((test_type*)t, n);

    OFF;

    return 0;
}
