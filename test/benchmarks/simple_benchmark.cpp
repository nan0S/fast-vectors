#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

#include <static_vector.hpp>
#include <static_vector_alt.hpp>
#include <boost/container/static_vector.hpp>
#include <utils/utils.hpp>
#include <test_type/test_type.hpp>
#include <benchmark/benchmark.h>

using namespace benchmark; 

constexpr uint C = 50000;
using value_type = int;

using boost_static_vector = boost::container::static_vector<value_type, C>;
using uwr_static_vector = uwr::static_vector<value_type, C>;
using uwr_static_vector_alt = uwr::static_vector_alt<value_type, C>;

#define COMMON_ITERS -1

static constexpr int PUSH_BACK_TIMES = 10000;
#define PUSH_BACK_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)
static constexpr int PUSH_BACK_POP_BACK_TIMES = 10000;
#define PUSH_BACK_POP_BACK_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)
static constexpr int SWAP_TIMES = 50000;
#define SWAP_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)
static constexpr int RESIZE_TIMES = 1000000;
#define RESIZE_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)

static_assert(C > PUSH_BACK_TIMES, "PUSH_BACK_TIMES has to be bigger than C");

#define DO_STATIC_VECTOR_BENCH
#define DO_STATIC_VECTOR_ALT_BENCH

/*
 * benchmark push_back member function
 */
template<class Vector>
void BM_push_back(State& s) {
    using T = typename Vector::value_type;
    int times = s.range(0);

    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v.data());
        
        for (int j = 0; j < times; ++j)
            v.push_back(get_value<T>(j));

        ClobberMemory();
    }

    s.counters["t1"];
}

/*
 * benchmark push_back and pop_back interleaved
 */
template<class Vector>
void BM_push_back_pop_back(State& s) {
    using size_type = typename Vector::size_type;
    using T = typename Vector::value_type;
    size_type times = s.range(0);

    for (auto _ : s) {
        Vector v;
        size_type iters = times;
        DoNotOptimize(v.data());

        while (iters) {
            size_type c = rand() % iters + 1;
            iters -= c;
            if (v.size() + c <= v.capacity() && rand() % 2 == 0)
                for (size_type j = 0; j < c; ++j)
                    v.push_back(get_value<T>(j));
            else
                while (!v.empty() && c--)
                    v.pop_back();
        }

        ClobberMemory();
    }

    s.counters["t2"];
}

/*
 * benchmark swapping vectors of parameterized size
 */
template<class Vector>
void BM_swap(State& s) {
    int size1 = s.range(0);
    int size2 = size1 * 3 / 4;

    for (auto _ : s) {
        Vector v1 = get_container<Vector>(size1);
        Vector v2 = get_container<Vector>(size2);

        DoNotOptimize(v1.data());
        DoNotOptimize(v2.data());

        v1.swap(v2);

        ClobberMemory();
    }

    s.counters["t5"];
}

/*
 * benchmark resizing vector
 */
template<class Vector>
void BM_resize(State& s) {

    for (auto _ : s) {
        int s = rand() % 10 + 5;
        Vector v;
        DoNotOptimize(v.data());

        v.resize(s);
        v.resize(rand() % (s - 3));

        ClobberMemory();
    }

    s.counters["t4"];
}

/*
 * BM_push_back
 */
BENCHMARK_TEMPLATE(BM_push_back, boost_static_vector)
    ->Unit(kMicrosecond)
#if (PUSH_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_TIMES);

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_push_back, uwr_static_vector)
    ->Unit(kMicrosecond)
#if (PUSH_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_TIMES);
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_push_back, uwr_static_vector_alt)
    ->Unit(kMicrosecond)
#if (PUSH_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_TIMES);
#endif

/*
 * BM_push_back_pop_back
 */
BENCHMARK_TEMPLATE(BM_push_back_pop_back, boost_static_vector)
    ->Unit(kMicrosecond)
#if (PUSH_BACK_POP_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_POP_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_POP_BACK_TIMES);

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_push_back_pop_back, uwr_static_vector)
    ->Unit(kMicrosecond)
#if (PUSH_BACK_POP_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_POP_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_POP_BACK_TIMES);
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_push_back_pop_back, uwr_static_vector_alt)
    ->Unit(kMicrosecond)
#if (PUSH_BACK_POP_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_POP_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_POP_BACK_TIMES);
#endif

/*
 * BM_swap
 */
BENCHMARK_TEMPLATE(BM_swap, boost_static_vector)
    ->Unit(kMicrosecond)
#if (SWAP_ITERS != -1)
    ->Iterations(SWAP_ITERS)
#endif
    ->Arg(SWAP_TIMES);

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_swap, uwr_static_vector)
    ->Unit(kMicrosecond)
#if (SWAP_ITERS != -1)
    ->Iterations(SWAP_ITERS)
#endif
    ->Arg(SWAP_TIMES);
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_swap, uwr_static_vector_alt)
    ->Unit(kMicrosecond)
#if (SWAP_ITERS != -1)
    ->Iterations(SWAP_ITERS)
#endif
    ->Arg(SWAP_TIMES);
#endif

/*
 * BM_resize
 */
BENCHMARK_TEMPLATE(BM_resize, boost_static_vector)
    ->Unit(kNanosecond)
#if (RESIZE_ITERS != -1)
    ->Iterations(RESIZE_ITERS)
#endif
    ->Arg(RESIZE_TIMES);

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_resize, uwr_static_vector)
    ->Unit(kNanosecond)
#if (RESIZE_ITERS != -1)
    ->Iterations(RESIZE_ITERS)
#endif
    ->Arg(RESIZE_TIMES);
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_resize, uwr_static_vector)
    ->Unit(kNanosecond)
#if (RESIZE_ITERS != -1)
    ->Iterations(RESIZE_ITERS)
#endif
    ->Arg(RESIZE_TIMES);
#endif

BENCHMARK_MAIN();
