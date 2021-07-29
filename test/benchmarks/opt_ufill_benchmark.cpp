#include <benchmark/benchmark.h>
#include <common/memory.hpp>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15
using trivial_type_t = std::array<int, 10>;

/*
 * benchmark ufill(begin, n)
 */
template<class T>
void BM_ufill_n(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        char t[n * sizeof(T)];
        DoNotOptimize((void*)t);

        ufill((T*)t, n, get_value<T>(13));

        ClobberMemory();
    }

    s.counters["1"];
}

/*
 * benchmark opt_ufill(begin, n)
 */
template<class T>
void BM_opt_ufill(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        char t[n * sizeof(T)];
        DoNotOptimize((void*)t);

        opt_ufill((T*)t, n, get_value<T>(13));

        ClobberMemory();
    }

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_ufill_n, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_opt_ufill, trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
