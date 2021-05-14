#include <bits/stdc++.h>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>

int main() {
    constexpr static int C = 5;
    using T = test_type;
    // using Vector = boost::container::static_vector<T, C>;
    using Vector = uwr::static_vector<T, C>;
    ON;

    Vector v(5);

    return 0;
}
