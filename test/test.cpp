#include <bits/stdc++.h>
#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <rvector/rvector.h>

constexpr int C = 5;
// using T = int;
using T = test_type;
// using Vector = uwr::static_vector<T, C>;
// using Vector = boost::container::static_vector<T, C>;
// using Vector = rvector<T>;
using Vector = boost::container::vector<T>;

void test1() {
    Vector v(1);
    v.erase(v.begin());

    for (const auto& x : v)
        std::cout << x << ' ';
    std::cout << '\n';

    v.assign({1, 2, 3, 4});
    for (const auto& x : v)
        std::cout << x << ' ';
    std::cout << '\n';

    for (auto it = v.rbegin(); it != v.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';
}

void test2() {
    OFF;
    Vector v1 { 1, 2, 3 };
    Vector v2 { 1, 2, 3, 4 };
    ON;
    std::cout << v1.capacity() << std::endl;
    std::cout << v2.capacity() << std::endl;
    v1 = v2;
    OFF;
}

int main() {
    // test1();
    test2();

    return 0;
}
