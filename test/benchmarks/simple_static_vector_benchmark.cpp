#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <benchmark/benchmark.h>
#include <utils/utils.hpp>
#include <test_type/test_type.hpp>
#include <boost/container/static_vector.hpp>
#include <static_vector.hpp>
#include <static_vector_alt.hpp>

using namespace benchmark;
using args_t = std::vector<int64_t>;

/*
 * configurable parameters
 */
using T_t = std::array<int, 10>;
using NT_t = std::string;

// number of consecutive push_backs in one iteration
static  const  args_t  PUSH_BACK_ARGS           =  { 100'000 };
// number of push_back/pop_back rounds in iteration 
static  const  args_t  PUSH_BACK_POP_BACK_ARGS  =  { 10 };
// sizes of swapped vectors
static  const  args_t  SWAP_ARGS                =  { 100'000 };
// number of resizes in one iteration
static  const  args_t  RESIZE_ARGS              =  { 10 };
// initial vector size and number of erases in one iteration
static  const  args_t  ERASE_ARGS               =  { 500'000, 10 };
// initial vector size and number of inserts in one iteration
static  const  args_t  INSERT_ARGS              =  { 100'000, 1 };

/* use the same number of iterations in all benchmarks */
#define COMMON_ITERS 0

#define  PUSH_BACK_ITERS           (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  PUSH_BACK_POP_BACK_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  SWAP_ITERS                (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  RESIZE_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  ERASE_ITERS               (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INSERT_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)

#define DO_STATIC_VECTOR_BENCH
#define DO_STATIC_VECTOR_ALT_BENCH

static constexpr int C = 500'000; // static_vector capacity

/*
 * tested vectors
 */
template<class T>
using boost_static_vector = boost::container::static_vector<T, C>;
template<class T>
using uwr_static_vector = uwr::static_vector<T, C>;
template<class T>
using uwr_static_vector_alt = uwr::static_vector_alt<T, C>;

/*
 * benchmark push_back
 */
template<class Vector>
void BM_push_back(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;

    size_type times = s.range(0);

    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v.data());
        
        for (size_type j = 0; j < times; ++j) {
            v.push_back(get_value<T>(j));
        }
        ClobberMemory();
    }

    if constexpr (std::is_trivial_v<T>)
        s.counters["T1"];
    else
        s.counters["NT1"];
}

/*
 * benchmark push_back and pop_back interleaved
 */
template<class Vector>
void BM_push_back_pop_back(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(12321);

    size_type times = s.range(0);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());

        for (size_type i = 0; i < times; ++i) {
            if (Random::rand(2)) {
                size_type rest = v.capacity() - v.size();
                size_type c = Random::rand<size_type>(0, rest + 1);

                while (c--) {
                    v.push_back(get_value<T>(c));
                }
                ClobberMemory();
            }
            else {
                size_type c = Random::rand<size_type>(0, v.size() + 1);
                
                while (c--) {
                    v.pop_back();
                }
                ClobberMemory();
            }
        }
    }

    if constexpr (std::is_trivial_v<T>)
        s.counters["T2"];
    else
        s.counters["NT2"];
}

/*
 * benchmark swapping vectors of parameterized size
 */
template<class Vector>
void BM_swap(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(3213121);

    size_type size1 = s.range(0);
    size_type size2 = size1 * 1 / 4;
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

    if constexpr (std::is_trivial_v<T>)
        s.counters["T3"];
    else
        s.counters["NT3"];
}

/*
 * benchmark resizing vector
 */
template<class Vector>
void BM_resize(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(31231);

    size_type times = s.range(0);

    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v.data());

        for (size_type i = 0; i < times; ++i) {
            size_type new_size = Random::rand(v.capacity() + 1);
            v.resize(new_size);
            ClobberMemory();
        }
    }

    if constexpr (std::is_trivial_v<T>)
        s.counters["T4"];
    else
        s.counters["NT4"];
}

/*
* benchmark erasing from vector
*/
template<class Vector>
void BM_erase(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(3213);

    size_type initial_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        Vector v(initial_size);
        DoNotOptimize(v.data());
        ClobberMemory();

        for (size_type i = 0; i < times; ++i) {
            size_type size = v.size();
            size_type pos = Random::rand(size + 1);
            size_type count = Random::rand(size - pos + 1);
            v.erase(v.begin() + pos, v.begin() + pos + count);
            ClobberMemory();
        }
    }

    if constexpr (std::is_trivial_v<T>)
        s.counters["T5"];
    else
        s.counters["NT5"];
}

/*
* benchmark inserting by fill to vector
*/
template<class Vector>
void BM_insert_fill(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(3213);

    size_type initial_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        Vector v(initial_size);
        DoNotOptimize(v.data());
        ClobberMemory();

        for (size_type i = 0; i < times; ++i) {
            size_type pos = Random::rand(v.size() + 1);
            size_type count = Random::rand(v.capacity() - v.size() + 1);
            v.insert(v.begin() + pos, count, get_value<T>(pos));
            ClobberMemory();
        }
    }

    if constexpr (std::is_trivial_v<T>)
        s.counters["T6"];
    else
        s.counters["NT6"];
}

/*
* benchmark inserting by range to vector
*/
template<class Vector>
void BM_insert_range(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(3213);

    T range[C];
    for (size_type i = 0; i < C; ++i)
        range[i] = get_value<T>(i);

    size_type initial_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        Vector v(initial_size);
        DoNotOptimize(v.data());
        ClobberMemory();

        for (size_type i = 0; i < times; ++i) {
            size_type pos = Random::rand(v.size() + 1);
            size_type count = Random::rand(v.capacity() - v.size() + 1);
            size_type range_pos = Random::rand(C - count + 1);
            v.insert(v.begin() + pos, range + range_pos, range + range_pos + count);
            ClobberMemory();
        }
    }

    if constexpr (std::is_trivial_v<T>)
        s.counters["T7"];
    else
        s.counters["NT7"];
}

/*
 * some macro magic
 */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, vector, type) \
    BENCHMARK_TEMPLATE(func, vector<type>) \
        ->Unit(unit) \
        ->Iterations(CONCAT(varname, _ITERS)) \
        ->Args({CONCAT(varname, _ARGS)})

#ifdef DO_STATIC_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, uwr_static_vector, type)
#else
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR(func, unit, varname, type)
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR_ALT(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, uwr_static_vector_alt, type)
#else
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR_ALT(func, unit, varname, type)
#endif

#define REGISTER_BENCHMARK_FOR_TYPE(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, boost_static_vector, type); \
    REGISTER_BENCHMARK_FOR_STATIC_VECTOR(func, unit, varname, type); \
    REGISTER_BENCHMARK_FOR_STATIC_VECTOR_ALT(func, unit, varname, type)

#define REGISTER_BENCHMARK(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_TYPE(func, unit, varname, T_t); \
    REGISTER_BENCHMARK_FOR_TYPE(func, unit, varname, NT_t)

/*
 * register all benchmarks
 */
REGISTER_BENCHMARK(BM_push_back,           kMicrosecond,  PUSH_BACK);
REGISTER_BENCHMARK(BM_push_back_pop_back,  kMicrosecond,  PUSH_BACK_POP_BACK);
REGISTER_BENCHMARK(BM_swap,                kMicrosecond,  SWAP);
REGISTER_BENCHMARK(BM_resize,              kNanosecond,   RESIZE);
REGISTER_BENCHMARK(BM_erase,               kMicrosecond,  ERASE);
REGISTER_BENCHMARK(BM_insert_fill,         kMicrosecond,  INSERT);
REGISTER_BENCHMARK(BM_insert_range,        kMicrosecond,  INSERT);

BENCHMARK_MAIN();
