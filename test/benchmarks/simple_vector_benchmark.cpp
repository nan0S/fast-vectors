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
#include <std_vector.hpp>

using namespace benchmark; 
using args_t = std::vector<int64_t>;

/*
 * configurable parameters
 */
using T_t = std::array<int, 10>;
using NT_t = std::string;

// number of consecutive push backs in one iteration
static const args_t PUSH_BACK_ARG = { 120'000'000 };
// maximum vector size and number of push_back/pop_back rounds in iteration 
static const args_t PUSH_BACK_POP_BACK_ARG = { 50'000, 10 };
// maximum resize size and resize count
static const args_t RESIZE_ARG = { 50'000, 10 };
// maximum size of created vector and insert count
static const args_t INSERT_ARG = { 1'000, 100 };
// maximum size of created vector
static const args_t CREATE_ARG = { 50'000 };
// maximum size of created and assigned vector and assign count
static const args_t ASSIGN_ARG = { 50'000, 20 };
// maximum size of created vector and emplace count
static const args_t EMPLACE_ARG = { 200'000, 10 };
// initial size of vector and number of erases in one iteration
static const args_t ERASE_ARG = { 200'000, 10 };


/* use the same number of iterations in all benchmarks */
#define COMMON_ITERS 0

#define  PUSH_BACK_ITERS           (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  PUSH_BACK_POP_BACK_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  RESIZE_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INSERT_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  CREATE_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  ASSIGN_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  EMPLACE_ITERS             (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  ERASE_ITERS               (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)

// #define DO_STD_VECTOR_BENCH
// #define DO_BOOST_VECTOR_BENCH
#define DO_UWR_VECTOR_BENCH
#define DO_RVECTOR_BENCH
#define DO_UWR_STD_VECTOR_BENCH

// turn on verbose printing for test_type type
// #define VERBOSE_FOR_TEST_TYPE

/*
 * tested vectors
 */
template<class T>
using std_vector = std::vector<T>;
using boost_gf = boost::container::growth_factor_100;
using boost_options = boost::container::vector_options_t<boost::container::growth_factor<boost_gf>>;
template<class T>
using boost_vector = boost::container::vector<T, boost::container::new_allocator<T>, boost_options>; 
template<class T>
using rvector_t = rvector<T>;
template<class T>
using uwr_vector = uwr::vector<T>;
template<class T>
using uwr_std_vector = uwr::std_vector<T>;

/*
 * benchmark push_back
 */
template<class Vector>
void BM_push_back(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::start_recording();
    #endif

    size_type times = s.range(0);

    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v.data());
        // v.reserve(times);

        for (size_type j = 0; j < times; ++j) {
            v.push_back(get_value<T>(j));
        }
        ClobberMemory();
    }

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::print_stats();
    #endif

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

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::start_recording();
    #endif

    size_type max_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v);

        for (size_type i = 0; i < times; ++i) {
            if (Random::rand(2)) {
                size_type rest = max_size - v.size();
                size_type c = Random::rand<size_type>(rest + 1);
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

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::print_stats();
    #endif

    if constexpr (std::is_trivial_v<T>)
        s.counters["T2"];
    else
        s.counters["NT2"];
}

/*
 * benchmark resizing vector
 */
template<class Vector>
void BM_resize(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(31231);

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::start_recording();
    #endif

    size_type max_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v);

        for (size_type i = 0; i < times; ++i) {
            size_type new_size = Random::rand(max_size + 1);
            v.resize(new_size);
            ClobberMemory();
        }
    }

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::print_stats();
    #endif

    if constexpr (std::is_trivial_v<T>)
        s.counters["T3"];
    else
        s.counters["NT3"];
}

/*
 * benchmark inserting to vector
 */
template<class Vector>
void BM_insert(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(123);

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::start_recording();
    #endif

    size_type max_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        size_type size = Random::rand(max_size + 1);
        Vector v(size, get_value<T>(size + 10));
        DoNotOptimize(v);
        ClobberMemory();
        
        for (size_type i = 0; i < times; ++i) {
            size_type pos = Random::rand(size + 1);
            size_type count = Random::rand(size / 5 + 1);
            v.insert(v.begin() + pos, count, get_value<T>(size));
            ClobberMemory();
        }

    }

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::print_stats();
    #endif

    if constexpr (std::is_trivial_v<T>)
        s.counters["T4"];
    else
        s.counters["NT4"];
}

/*
 * benchmark creating vector
 */
template<class Vector>
void BM_create(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(123);

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::start_recording();
    #endif

    size_type max_size = s.range(0);

    for (auto _ : s) {
        size_type size = Random::rand(max_size + 1);
        Vector v(size);
        DoNotOptimize(v);
        ClobberMemory();
    }
    
    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::print_stats();
    #endif

    if constexpr (std::is_trivial_v<T>)
        s.counters["T5"];
    else
        s.counters["NT5"];
}

/*
* benchmark assigning to vector
*/
template<class Vector>
void BM_assign(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(123);

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::start_recording();
    #endif

    size_type max_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        size_type size = Random::rand(max_size + 1);
        Vector v(size);
        DoNotOptimize(v);
        ClobberMemory();

        for (size_type i = 0; i < times; ++i) {
            size_type new_size = Random::rand(max_size + 1);
            v.assign(new_size, get_value<T>(new_size));
            ClobberMemory();
        }
    }

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::print_stats();
    #endif
    
    if constexpr (std::is_trivial_v<T>)
        s.counters["T6"];
    else
        s.counters["NT6"];
}

/*
* benchmark emplacing to vector
*/
template<class Vector>
void BM_emplace(State& s) {
    using T = typename Vector::value_type;
    using size_type = typename Vector::size_type;
    Random::seed(123);

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::start_recording();
    #endif

    size_type max_size = s.range(0);
    size_type times = s.range(1);

    for (auto _ : s) {
        size_type size = Random::rand(max_size + 1);
        Vector v(size);
        DoNotOptimize(v);
        ClobberMemory();

        for (size_type i = 0; i < times; ++i) {
            size_type pos = Random::rand(size + 1);
            v.emplace(v.begin() + pos, get_value<T>(pos));
            ClobberMemory();
        }
    }

    #ifdef VERBOSE_FOR_TEST_TYPE
    test_type::print_stats();
    #endif
    
    if constexpr (std::is_trivial_v<T>)
        s.counters["T7"];
    else
        s.counters["NT7"];
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
        s.counters["T8"];
    else
        s.counters["NT8"];
}

/*
 * some macro magic
 */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, type, vector) \
    BENCHMARK_TEMPLATE(func, vector<type>) \
        ->Unit(unit) \
        ->Iterations(CONCAT(varname, _ITERS)) \
        ->Args(CONCAT(varname, _ARG))

#ifdef DO_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, type, std_vector)
#else
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(func, unit, varname, type)
#endif

#ifdef DO_BOOST_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, type, boost_vector)
#else
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(func, unit, varname, type)
#endif

#ifdef DO_UWR_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, type, uwr_vector)
#else
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(func, unit, varname, type)
#endif

#ifdef DO_RVECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_RVECTOR(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, type, rvector_t)
#else
#define REGISTER_BENCHMARK_FOR_RVECTOR(func, unit, varname, type)
#endif

#ifdef DO_UWR_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, type, uwr_std_vector)
#else
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(func, unit, varname, type)
#endif

#define REGISTER_BENCHMARK_FOR_TYPE(func, unit, varname, type) \
    REGISTER_BENCHMARK_FOR_STD_VECTOR(func, unit, varname, type); \
    REGISTER_BENCHMARK_FOR_BOOST_VECTOR(func, unit, varname, type); \
    REGISTER_BENCHMARK_FOR_RVECTOR(func, unit, varname, type); \
    REGISTER_BENCHMARK_FOR_UWR_VECTOR(func, unit, varname, type); \
    REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(func, unit, varname, type)

#define REGISTER_BENCHMARK(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_TYPE(func, unit, varname, T_t); \
    REGISTER_BENCHMARK_FOR_TYPE(func, unit, varname, NT_t)

/*
 * register all benchmarks
 */
REGISTER_BENCHMARK(BM_push_back,           kMillisecond,  PUSH_BACK);
REGISTER_BENCHMARK(BM_push_back_pop_back,  kMicrosecond,  PUSH_BACK_POP_BACK);
REGISTER_BENCHMARK(BM_resize,              kMicrosecond,  RESIZE);
REGISTER_BENCHMARK(BM_insert,              kMicrosecond,  INSERT);
REGISTER_BENCHMARK(BM_create,              kMicrosecond,  CREATE);
REGISTER_BENCHMARK(BM_assign,              kMicrosecond,  ASSIGN);
REGISTER_BENCHMARK(BM_emplace,             kMicrosecond,  EMPLACE);
REGISTER_BENCHMARK(BM_erase,               kMicrosecond,  ERASE);

int main(int argc, char** argv) {
    // turn on thounsand commas when printing
    std::cout.imbue(std::locale(""));

    Initialize(&argc, argv);
    if (ReportUnrecognizedArguments(argc, argv))
        return 1;
    RunSpecifiedBenchmarks();
    Shutdown();

    #ifdef UWR_TRACK
    uwr::mem::counters::print();
    #endif
    #ifdef RVECTOR_TRACK
    mm::counters::print();
    #endif

    return 0;
}
