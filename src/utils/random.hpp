#pragma once

#include <random>

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
    Int<T> rand(T n) {
        return dist_t<T>{0, n - 1}(rng);
    }
    
    /*
     * random integer number from [a, b)
     */
    template<class T>
    Int<T> rand(T a, T b) {
        return dist_t<T>{a, b - 1}(rng);
    }

    /*
     * random decimal number from [a, b]
     */
    template<class T>
    N_Int<T> rand(T a, T b) {
        return dist_t<T>{a, b}(rng);
    }
}

