#include <benchmark/benchmark.h>
#include <common/memory.hpp>
#include <test_type/test_type.hpp>

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15
using trivial_type_t = int;

/*
 * benchmark memmove(begin, end)
 */
template<class T>
void BM_memmove(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T t[n];
        DoNotOptimize((void*)t);

        std::memmove(t, t + 1, (n - 1) * sizeof(T));

        ClobberMemory();
    }

    s.counters["1"];
}

/*
 * benchmark construct(begin, n)
 */
template<class T>
void BM_move(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        T t[n];
        DoNotOptimize((void*)t);

        std::move(t + 1, t + n, t);

        ClobberMemory();
    }

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_memmove, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_move, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
