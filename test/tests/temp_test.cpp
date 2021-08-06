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
    
    Vector v(511);
    cout << v.capacity() << endl;
    test_type::start_recording();
    v.emplace_back();
    cout << v.capacity() << endl;
    // v.insert(v.begin(), 10000, 1);

    test_type::print_stats();
    cout << endl;
}

#define RUN(v) f<v>(#v)

int main() {
    using T = test_type;
    // using Vector = uwr::vector<T>;
    // using Vector = std::vector<T>;
    // using Vector = boost_vector<T>;

    RUN(uwr::vector<T>);
    RUN(std::vector<T>);
    RUN(rvector<T>);
    RUN(boost_vector<T>);

    return 0;
}
