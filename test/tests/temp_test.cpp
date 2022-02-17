#include <bits/stdc++.h>

#include <boost/container/vector.hpp>
#include <boost/container/static_vector.hpp>

#include "uwr/vector.hpp"
#include "uwr/static_vector.hpp"
#include "uwr/allocator/malloc_allocator.hpp"

#include "test_type/test_type.hpp"
#include "utils/value.hpp"

using namespace std;

using boost_gf = boost::container::growth_factor_100;
using boost_options = boost::container::vector_options_t<boost::container::growth_factor<boost_gf>>;
template<class T>
using boost_vector = boost::container::vector<T, boost::container::new_allocator<T>, boost_options>;

constexpr static int C = 10;

template<class T>
using uwr_static_vector = uwr::static_vector<T, C>;
template<class T>
using boost_static_vector = boost::container::static_vector<T, C>;

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

// using T = std::string;
using T = int;
// using T = std::array<int, 10>;
// using Vector = rvector<T>;
using std_vector = std::vector<T>;
using boost_vector_t = boost_vector<T>;
using uwr_vector = uwr::vector<T>;
using uwr_c_vector = uwr::vector<T, uwr::mem::malloc_allocator<T>>;


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
    // show_capacity<uwr_vector>();
    // show_capacity<rvector_t>();
    // show_capacity<boost_vector_t>();
    // show_capacity<uwr_std_vector>();

    // using Vector = std_vector;
    // using Vector = uwr_std_vector;
    // using Vector = boost_vector_t;

    // std_vector v(5);
    // cout << v.capacity() << endl;
    // v.assign(12, 1);
    // v.resize(12, 1);
    // v.reserve(12);
    // v.assign({1, 2, 3, 4, 5, 6});
    // v.insert(v.begin(), 3, 5);
    // v.emplace(v.begin(), 1);
    // v.erase(v.begin(), v.end());
    // v.clear();
    // cout << v.capacity() << endl;

    using T = test_type;
    // using Vector = uwr_static_vector<T>;
    using Vector = boost_static_vector<T>;

    test_type::start_recording();

    Vector v1(5);
    Vector v2(5 * 5 / 3);

    PRINT_ON;
    
    std::reverse(v1.begin(), v1.end());
    std::swap(v1, v2);

    PRINT_OFF;

    test_type::print_stats();

    return 0;
}
