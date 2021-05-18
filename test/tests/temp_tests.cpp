#include <bits/stdc++.h>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>
#include <utils/utils.hpp>
#include <vector.hpp>

int main() {
    constexpr static int C = 10;
    using T = int;
    // using Vector = boost::container::static_vector<T, C>;
    // using Vector = uwr::static_vector<T, C>;
    using Vector = uwr::vector<T>;
    Vector v { 1, 2, 3 };
    ON;

    for (const auto& x : v)
        std::cout << x << std::endl;

    OFF;
    return 0;
}
