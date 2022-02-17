#include <benchmark/benchmark.h>

#include "uwr/common/memory.hpp"

#include "test_type/test_type.hpp"
#include "utils/value.hpp"

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15
using trivial_type_t = int;
using non_trivial_type_t = test_type;

/*
 * benchmark uninitialized fill(begin, end)
 */
template<class T>
void BM_ufill_range(State& s) {
    int n = s.range(0);

    for (auto _ : s) {
        char t[n * sizeof(T)];
        DoNotOptimize((void*)t);

        ufill((T*)t, (T*)t + n, get_value<T>(13));

        ClobberMemory();
    }

    s.counters["1"];
}

/*
 * benchmark uninitialized fill(begin, n)
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

    s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_ufill_range, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_ufill_range, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_ufill_n, trivial_type_t)
    ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_ufill_n, non_trivial_type_t)
    ->Range(RANGE);

BENCHMARK_MAIN();
