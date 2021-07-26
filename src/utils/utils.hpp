#pragma once

#include <iostream>
#include <random>
#include <array>

/*
 * print separated by spaces and newline at the end
 */
template<class First, class... Rest>
void log(First&& first, Rest&&... rest);

/*
 * map int to class T, e.g. get_value<str>(10) -> "test10"
 * feel free to specify for your own types
 */
template<class T>
T get_value(int id);

template<>
std::string get_value(int id);
template<>
std::array<int, 10> get_value(int id);

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

    /*
     * set seed
     */
    void seed(int seed);

    /*
     * random integer from [0, n - 1]
     */
    template<class T>
    Int<T> rand(T n);
    
    /*
     * random number from [a, b]
     */
    template<class T>
    T rand(T a, T b);
}

/*
 * implementations
 */

namespace Random {
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
T get_value(int id) {
    return id;
}

template<class V>
V get_container(int size) {
    using T = typename V::value_type;
    V v;
    v.reserve(size);
    for (int i = 0; i < size; ++i)
        v.emplace_back(get_value<T>(i));
    return v;
}

template<class First, class... Args>
void log(First&& first, Args&&... args) {
    std::cout << std::forward<First>(first) << " ";
    log(std::forward<Args>(args)...);
}
