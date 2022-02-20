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
 * benchmark ucopy(begin, end)
 */
template<class T>
void BM_ucopy_range(State& s) {
   int n = s.range(0);

   for (auto _ : s) {
      T src[n];
      char dest[n * sizeof(T)];

      DoNotOptimize((void*)src);
      DoNotOptimize((void*)dest);

      ucopy((T*)dest, src, src + n);

      ClobberMemory();
   }

   s.counters["1"];
}

/*
 * benchmark ucopy(begin, n)
 */
template<class T>
void BM_ucopy_n(State& s) {
   int n = s.range(0);

   for (auto _ : s) {
      T src[n];
      char dest[n * sizeof(T)];

      DoNotOptimize((void*)src);
      DoNotOptimize((void*)dest);

      ucopy((T*)dest, src, n);

      ClobberMemory();
   }

   s.counters["2"];
}

BENCHMARK_TEMPLATE(BM_ucopy_range, trivial_type_t)
   ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_ucopy_range, non_trivial_type_t)
   ->Range(RANGE);

BENCHMARK_TEMPLATE(BM_ucopy_n, trivial_type_t)
   ->Range(RANGE);
BENCHMARK_TEMPLATE(BM_ucopy_n, non_trivial_type_t)
   ->Range(RANGE);

BENCHMARK_MAIN();
