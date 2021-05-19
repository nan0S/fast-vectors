#pragma once

#include <iostream>
#include <random>
#include <array>

template<class T>
void info(const T& msg, int indent=0, bool reset=false);

template<class T>
T get_value(int id);
// template<class T, int C>
// template<class std::array<typename T, int C>>
// std::array<T, C> get_value(int id);
template<>
std::string get_value(int id);

namespace Random {
    extern std::mt19937 rng;

    template<class T>
    using Int = std::enable_if_t<std::is_integral_v<T>, T>;
    template<class T>
    using N_Int = std::enable_if_t<!std::is_integral_v<T>, T>;

    template<class T>
    using dist_t = std::conditional_t<
        std::is_integral_v<T>,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>
    >;

    template<class T>
    Int<T> rand(T n) {
        return dist_t<T>{0, n - 1}(rng);
    }

    template<class T>
    T rand(T a, T b) {
        return dist_t<T>{a, b}(rng);
    }
}

template<class T>
void info(const T& msg, int indent, bool reset) {
    if (reset)
        std::cout << '\r';
    std::cout << std::string(3 * indent, ' ') << msg;
    if (!reset)
        std::cout << std::endl;
    else
        std::cout.flush();
}

template<class T>
T get_value(int id) {
    return id;
}

// template<class T, std::size_t C>
// std::array<T, C> get_value(int id) {
    // std::array<T, C> arr;
    // arr.fill(get_value<T>(id));

    // return arr;
// }
