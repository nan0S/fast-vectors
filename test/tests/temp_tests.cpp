#include <bits/stdc++.h>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>
#include <utils/utils.hpp>

int main() {
    constexpr static int C = 10;
    using T = std::string;
    using Vector = boost::container::static_vector<T, C>;
    // using Vector = uwr::static_vector<T, C>;
    Vector v1 {
        "test0", "test2", "test13"
    };
    Vector v2 {
        "test0", "test15"
    };
    ON;

    std::cout << (v1 < v2);

    OFF;
    return 0;
}
