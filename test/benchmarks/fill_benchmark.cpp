#include <benchmark/benchmark.h>
#include <common/memory.hpp>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15
using trivial_type_t = int;
using non_trivial_type_t = test_type;

/*
 * benchmark fill(begin, end)
 */
template<class T>
void BM_fill_range(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T t[n];
        DoNotOptimize((void*)t);

        fill(t, t + n, get_value<T>(13));

        ClobberMemory();
    }

    s.counters["1"];
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

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_fill_range, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_fill_range, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_fill_n, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_fill_n, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
