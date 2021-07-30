#include <benchmark/benchmark.h>
#include <common/memory.hpp>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15

constexpr int C = 50;
using trivial_type_t = std::array<int, C>;

template<>
std::array<int, C> get_value(int id) {
    std::array<int, C> arr;
    arr.fill(id);

    return arr;
}

/*
 * benchmark fill(begin, n)
 */
template<class T>
void BM_fill_n(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T t[n];
        DoNotOptimize((void*)t);

        fill(t, n, get_value<T>(13));

        ClobberMemory();
    }

    s.counters["1"];
}

/*
 * benchmark hybrid_fill(begin, n)
 */
template<class T>
void BM_hybrid_fill(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T t[n];
        DoNotOptimize((void*)t);

        hybrid_fill(t, n, get_value<T>(13));

        ClobberMemory();
    }

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_fill_n, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_hybrid_fill, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
