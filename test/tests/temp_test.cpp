#include <bits/stdc++.h>
#include <static_vector.hpp>
#include <static_vector_alt.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>

int main() {
    constexpr static int C = 10;
    using T = test_type;
    using Vector1 = boost::container::static_vector<T, C>;
    using Vector2 = uwr::static_vector_alt<T, C>;

    Vector1 v1 { 1, 2, 3, 4, 5 };
    Vector2 v2 { 1, 2, 3, 4, 5 };
    const test_type val = 13;
    int pos = 0;

    ON;

    v1.emplace(v1.begin() + pos, val);
    std::cout << std::endl;

    v2.emplace(v2.begin() + pos, val);
    std::cout << std::endl;

    v1.emplace(v1.begin() + pos, 13);
    std::cout << std::endl;

    v2.emplace(v2.begin() + pos, 13);
    std::cout << std::endl;

    for (const auto& x : v1)
        std::cout << x << " ";
    std::cout << std::endl;

    for (const auto& x : v2)
        std::cout << x << " ";
    std::cout << std::endl;

    OFF;
    return 0;
}
