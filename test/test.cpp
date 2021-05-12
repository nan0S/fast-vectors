#include <bits/stdc++.h>
#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>

int main() {
    using T = int;
    constexpr int C = 5;
    using Vector = uwr::static_vector<T, C>;
    // using Vector = boost::container::static_vector<T, C>;

    Vector v(1);
    v.erase(v.begin());

    for (const auto& x : v)
        std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
