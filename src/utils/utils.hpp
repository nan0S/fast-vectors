#pragma once

#include <iostream>
#include <random>

template<class T>
void info(const T& msg, int indent=0, bool reset=false) {
    if (reset)
        std::cout << '\r';
    std::cout << std::string(3 * indent, ' ') << msg;
    if (!reset)
        std::cout << std::endl;
    else
        std::cout.flush();
}

namespace Random {
    extern std::mt19937 rng;

    template<class T>
    using Int = std::enable_if_t<
        std::is_integral_v<T>, T>;
    template<class T>
    using N_Int = std::enable_if_t<
        !std::is_integral_v<T>, T>;

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
