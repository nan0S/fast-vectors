#include <bits/stdc++.h>
#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <vector.hpp>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>
#include <rvector.h>

using namespace std;

constexpr static int C = 10;

using boost_gf = boost::container::growth_factor_100;
using boost_options = boost::container::vector_options_t<boost::container::growth_factor<boost_gf>>;
template<class T>
using boost_vector = boost::container::vector<T, boost::container::new_allocator<T>, boost_options>; 

template<class Container>
void print(const Container& c) {
    for (const auto& x : c)
        cout << x << " ";
    cout << endl;
}

template<class Vector>
void f(const std::string& name) {
    cout << endl << name << ":" << endl;
    test_type::start_recording();

    {
        Vector v1(100);
        Vector v2(100);
        
        v2 = v1;
    }

    test_type::print_stats();
    cout << endl;
}

#define RUN(v) f<v>(#v)

using T = std::string;
// using Vector = rvector<T>;
using Vector = uwr::vector<T>;
// using Vector = std::vector<T>;
// using Vector = boost_vector<T>;
using uwr_vector = uwr::vector<T>;
using rvector_t = rvector<T>;

constexpr long ps = 4096;

long npages(long x) {
    return x * sizeof(T) / ps;
}

template<class V>
void show_capacity() {
    int last_cap = -1;
    V v;
    for (int i = 300; i < 2048; ++i) {
        v.emplace_back();
        if (last_cap != v.capacity()) {
            cout << v.capacity() << " ";
            last_cap = v.capacity();
        }
    }
    cout << endl;
}

int main() {
    show_capacity<uwr_vector>();
    show_capacity<rvector_t>();

    return 0;
}
