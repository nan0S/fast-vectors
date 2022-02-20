#include <benchmark/benchmark.h>

#include "uwr/common/memory.hpp"

#include "test_type/test_type.hpp"

using namespace benchmark;
using namespace uwr::mem;

#define RANGE 1<<5, 1<<15
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

      construct((T*)t, (T*)t + n);

      ClobberMemory();
   }

   s.counters["1"];
}

/*
 * benchmark construct(begin, n)
 */
template<class T>
void BM_construct_n(State& s) {
   int n = s.range(0);

   for (auto _ : s) {
      char t[n * sizeof(T)];
      DoNotOptimize((void*)t);

      construct((T*)t, n);

      ClobberMemory();
   }

   s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_construct_range, trivial_type_t)
   ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_construct_range, non_trivial_type_t)
   ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_construct_n, trivial_type_t)
   ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_construct_n, non_trivial_type_t)
   ->Range(RANGE);

BENCHMARK_MAIN();
