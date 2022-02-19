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

// using T = std::string;
using T = int;
// using T = std::array<int, 10>;
// using Vector = rvector<T>;
using std_vector = std::vector<T>;
using boost_vector_t = boost_vector<T>;
using uwr_vector = uwr::vector<T>;
using uwr_c_vector = uwr::vector<T, uwr::mem::malloc_allocator<T>>;

constexpr long ps = 4096;

int main() {
    using uwr::static_vector;
    {
       static_vector<test_type, 5> v1(5);
       cout << test_type::instances << endl;
       static_vector<test_type, 5> v2(v1);
       cout << test_type::instances << endl;
       print(v1);
       print(v2);
    }
    cout << test_type::instances << endl;

    return 0;
}
