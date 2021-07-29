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
 * benchmark copy(begin, end)
 */
template<class T>
void BM_copy_range(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T src[n], dest[n];
        
        DoNotOptimize((void*)src);
        DoNotOptimize((void*)dest);

        copy(dest, src, src + n);

        ClobberMemory();
    }

    s.counters["1"];
}

/*
 * benchmark copy(begin, n)
 */
template<class T>
void BM_copy_n(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T src[n], dest[n];

        DoNotOptimize((void*)src);
        DoNotOptimize((void*)dest);

        copy(dest, src, n);

        ClobberMemory();
    }

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_copy_range, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_copy_range, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_copy_n, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_copy_n, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
