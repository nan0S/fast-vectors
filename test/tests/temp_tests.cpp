#include <bits/stdc++.h>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>
#include <utils/utils.hpp>

int main() {
    constexpr static int C = 10;
    using T = test_type;
    using Vector = boost::container::static_vector<T, C>;
    // using Vector = uwr::static_vector<T, C>;
    Vector v { 1, 2, 3 };
    ON;

    v.erase(v.begin(), v.begin());

    OFF;
    return 0;
}
