#include <benchmark/benchmark.h>

#include "uwr/common/memory.hpp"

#include "test_type/test_type.hpp"

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15
using trivial_type_t = int;
using non_trivial_type_t = test_type;

/*
 * benchmark destroy(begin, end)
 */
template<class T>
void BM_destroy_range(State& s) {
   int n = s.range(0);

   for (auto _ : s) {
      T t[n];
      DoNotOptimize((void*)t);

      destroy(t, t + n);

      ClobberMemory();
   }

   s.counters["1"];
}

/*
 * benchmark destroy(begin, n)
 */
template<class T>
void BM_destroy_n(State& s) {
   int n = s.range(0);

   for (auto _ : s) {
      T t[n];
      DoNotOptimize((void*)t);

      destroy(t, n);

      ClobberMemory();
   }

   s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_destroy_range, trivial_type_t)
   ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_destroy_range, non_trivial_type_t)
   ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_destroy_n, trivial_type_t)
   ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_destroy_n, non_trivial_type_t)
   ->Range(RANGE);

BENCHMARK_MAIN();
