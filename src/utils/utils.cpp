#include "utils.hpp"

std::mt19937 Random::rng(std::random_device{}());

void Random::seed(int seed) {
    rng.seed(seed);
}

template<>
std::string get_value(int id) {
    return "test" + std::to_string(id);
}

template<>
std::array<int, 10> get_value(int id) {
    std::array<int, 10> arr;
    arr.fill(id);

    return arr;
}

template<>
std::array<int, 100> get_value(int id) {
    std::array<int, 100> arr;
    arr.fill(id);

    return arr;
}
