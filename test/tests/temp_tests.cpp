#include <bits/stdc++.h>
#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <vector.hpp>
#include <test_type/test_type.hpp>
// #include <utils/utils.hpp>

template<std::array<typename T, std::size_t C>>
std::array<T, C> get_value(int id) {
    std::array<T, C> arr;
    arr.fill(id);

    return arr;
}

int main() {
    constexpr static int C = 10;
    using T = int;
    // using Vector = boost::container::static_vector<T, C>;
    // using Vector = uwr::static_vector<T, C>;
    // using Vector = uwr::vector<T>;
    ON;

    // std::cout << get_value<std::string>(10) << std::endl;
    // std::cout << get_value<test_type>(10) << std::endl;
    // std::cout << get_value<int>(10) << std::endl;

    auto arr = get_value<std::array<int, 5>>(3);
    for (const auto& x : arr)
        std::cout << x << " ";
    std::cout << std::endl;

    OFF;
    return 0;
}
