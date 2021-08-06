#include <iostream>
#include <chrono>
#include <functional>
#include <benchmark/benchmark.h>
#include <utils/utils.hpp>
#include <test_type/test_type.hpp>
#include <boost/container/vector.hpp>
#include <vector>
#include <vector.hpp>
#include <rvector.h>

using namespace benchmark; 

/*
 * configurable parameters
 */
using value_type = std::string;

static  constexpr  int  PUSH_BACK_ARG           =  1000000;
static  constexpr  int  PUSH_BACK_POP_BACK_ARG  =  500000;
static  constexpr  int  SWAP_ARG                =  10000;
static  constexpr  int  RESIZE_ARG              =  500000;

/* use the same number of iterations in all benchmarks */
#define COMMON_ITERS 0

#define  PUSH_BACK_ITERS           (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  PUSH_BACK_POP_BACK_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  SWAP_ITERS                (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  RESIZE_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)

#define DO_UWR_VECTOR_BENCH
#define DO_RVECTOR_BENCH

/*
 * tested vectors
 */
using std_vector = std::vector<value_type>;
using boost_vector = boost::container::vector<value_type>;
using uwr_vector = uwr::vector<value_type>;
using rvector_t = rvector<value_type>;

/*
 * benchmark push_back
 */
template<class Vector>
void BM_push_back(State& s) {
    using T = typename Vector::value_type;
    int times = s.range(0);

    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v.data());
        
        for (int j = 0; j < times; ++j) {
            v.push_back(get_value<T>(j));
            ClobberMemory();
        }
    }

    s.counters["t1"];
}

/*
 * benchmark push_back and pop_back interleaved
 */
template<class Vector>
void BM_push_back_pop_back(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(12321);

    int times = s.range(0);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());

        for (int i = 0; i < times; ++i) {
            if (Random::rand(2)) {
                size_type rest = v.capacity() - v.size();
                size_type c = Random::rand<size_type>(0, rest + 1);

                while (c--) {
                    v.push_back(get_value<T>(c));
                    ClobberMemory();
                }
            }
            else {
                size_type c = Random::rand<size_type>(0, v.size() + 1);
                
                while (c--) {
                    v.pop_back();
                    ClobberMemory();
                }
            }
        }
    }

    s.counters["t2"];
}

/*
 * benchmark swapping vectors of parameterized size
 */
template<class Vector>
void BM_swap(State& s) {
    Random::seed(3213121);

    int size1 = s.range(0);
    int size2 = size1 * 3 / 4;

    if (Random::rand(2))
        std::swap(size1, size2);

    for (auto _ : s) {
        Vector v1 = get_container<Vector>(size1);
        Vector v2 = get_container<Vector>(size2);

        DoNotOptimize(v1.data());
        DoNotOptimize(v2.data());

        v1.swap(v2);

        ClobberMemory();
    }

    s.counters["t3"];
}

/*
 * benchmark resizing vector
 */
template<class Vector>
void BM_resize(State& s) {
    Random::seed(31231);
    int times = s.range(0);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());

        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(v.capacity() + 1);
            v.resize(new_size);
            ClobberMemory();
        }
    }

    s.counters["t4"];
}

/*
 * some macro magic
 */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, vector) \
    BENCHMARK_TEMPLATE(func, vector) \
        ->Unit(unit) \
        ->Iterations(CONCAT(varname, _ITERS)) \
        ->Arg(CONCAT(varname, _ARG))

#ifdef DO_UWR_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, uwr_vector)
#else
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(func, unit, varname)
#endif

#ifdef DO_RVECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_RVECTOR(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, rvector_t)
#else
#define REGISTER_BENCHMARK_FOR_RVECTOR(func, unit, varname)
#endif

#define REGISTER_BENCHMARK(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_UWR_VECTOR(func, unit, varname); \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, std_vector); \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, boost_vector); \
    REGISTER_BENCHMARK_FOR_RVECTOR(func, unit, varname)

/*
 * register all benchmarks
 */
REGISTER_BENCHMARK(BM_push_back,           kMicrosecond,  PUSH_BACK);
REGISTER_BENCHMARK(BM_push_back_pop_back,  kMicrosecond,  PUSH_BACK_POP_BACK);
REGISTER_BENCHMARK(BM_swap,                kMicrosecond,  SWAP);
REGISTER_BENCHMARK(BM_resize,              kNanosecond,   RESIZE);

BENCHMARK_MAIN();
