#include <bits/stdc++.h>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>
#include <utils/utils.hpp>

int main() {
    constexpr static int C = 5;
    using T = test_type;
    // using Vector = boost::container::static_vector<T, C>;
    using Vector = uwr::static_vector<T, C>;
    Vector v { 1, 2, 3, 4 };
    test_type val = 13;
    ON;

    v.insert(v.begin(), 0, val);

    for (const auto& x : v)
        std::cout << x << std::endl;

    OFF;

    return 0;
}
