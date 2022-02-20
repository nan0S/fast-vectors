#include "random.hpp"

std::mt19937 Random::rng(std::random_device{}());

void Random::seed(int seed) {
   rng.seed(seed);
}
