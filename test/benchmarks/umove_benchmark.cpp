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
 * benchmark umove(begin, end)
 */
template<class T>
void BM_umove_range(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T src[n];
        char dest[n * sizeof(T)];
        
        DoNotOptimize((void*)src);
        DoNotOptimize((void*)dest);

        umove((T*)dest, src, src + n);

        ClobberMemory();
    }

    s.counters["1"];
}

/*
 * benchmark umove(begin, n)
 */
template<class T>
void BM_umove_n(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T src[n];
        char dest[n * sizeof(T)];

        DoNotOptimize((void*)src);
        DoNotOptimize((void*)dest);

        umove((T*)dest, src, n);

        ClobberMemory();
    }

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_umove_range, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_umove_range, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_umove_n, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_umove_n, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
