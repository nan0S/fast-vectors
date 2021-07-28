#include <benchmark/benchmark.h>
#include <common/memory.hpp>
#include <test_type/test_type.hpp>

using namespace benchmark;
using namespace uwr::mem;

#define CONSTRUCT_RANGE 1<<5, 1<<15
using trivial_type_t = int;
using non_trivial_type_t = test_type;

/*
 * benchmark construct(begin, end)
 */
template<class T>
void BM_construct_range(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        char t[n * sizeof(T)];
        DoNotOptimize((void*)t);

        construct<T>((T*)t, (T*)t + n);

        ClobberMemory();
    }

    s.counters["2"];
}

/*
 * benchmark construct(begin, n)
 */
template<class T>
void BM_construct_range_n(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        char t[n * sizeof(T)];
        DoNotOptimize((void*)t);

        construct<T>((T*)t, n);

        ClobberMemory();
    }

    s.counters["3"];
}

BENCHMARK_TEMPLATE(BM_construct_range, trivial_type_t)
    ->Range(CONSTRUCT_RANGE);
BENCHMARK_TEMPLATE(BM_construct_range, non_trivial_type_t)
    ->Range(CONSTRUCT_RANGE);

BENCHMARK_TEMPLATE(BM_construct_range_n, trivial_type_t)
    ->Range(CONSTRUCT_RANGE);
BENCHMARK_TEMPLATE(BM_construct_range_n, non_trivial_type_t)
    ->Range(CONSTRUCT_RANGE);

BENCHMARK_MAIN();
