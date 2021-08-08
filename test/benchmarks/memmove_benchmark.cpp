#include <benchmark/benchmark.h>
#include <common/memory.hpp>
#include <utils/utils.hpp>

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15
using trivial_type_t = int;

/*
 * benchmark memmove(begin, end)
 */
template<class T>
void BM_memmove(State& s) {
    Random::seed(321);
    int n = s.range(0);

    for (auto _ : s) {
        T t[n];
        DoNotOptimize((void*)t);
        
        int shift = Random::rand(n) + 1;
        std::memmove(t, t + shift, (n - shift) * sizeof(T));

        ClobberMemory();
    }

    s.counters["1"];
}

/*
 * benchmark construct(begin, n)
 */
template<class T>
void BM_move(State& s) {
    Random::seed(321);
    int n = s.range(0);

    for (auto _ : s) {
        T t[n];
        DoNotOptimize((void*)t);

        int shift = Random::rand(n) + 1;
        std::move(t + shift, t + n, t);

        ClobberMemory();
    }

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_memmove, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_move, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
