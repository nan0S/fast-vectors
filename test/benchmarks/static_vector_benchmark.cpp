#include <static_vector.hpp>
#include <static_vector_alt.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>

#include "vector_benchmark_base.hpp"

static constexpr uint C = 500000;

template<class T>
using boost_static_vector = boost::container::static_vector<T, C>;
template<class T>
using uwr_static_vector = uwr::static_vector<T, C>;
template<class T>
using uwr_static_vector_alt = uwr::static_vector_alt<T, C>;

#define COMMON_ITERS 100

static constexpr int INT_TIMES = 500;
#define INT_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)
static constexpr int STRING_TIMES = 300;
#define STRING_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)
static constexpr int TEST_TYPE_TIMES = 300;
#define TEST_TYPE_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)
static constexpr int INT_STRING_TIMES = 200;
#define INT_STRING_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)
static constexpr int INT_STRING_ARRAY_TIMES = 100;
#define INT_STRING_ARRAY_ITERS (COMMON_ITERS == -1 ? -1 : COMMON_ITERS)

#define DO_STATIC_VECTOR_BENCH
#define DO_STATIC_VECTOR_ALT_BENCH

/*
 * BM_environment<int>
 */
BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_ITERS != -1)
    ->Iterations(INT_ITERS)
#endif
    ->Args({INT_TIMES, 1});

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_ITERS != -1)
    ->Iterations(INT_ITERS)
#endif
    ->Args({INT_TIMES, 1});
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector_alt, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_ITERS != -1)
    ->Iterations(INT_ITERS)
#endif
    ->Args({INT_TIMES, 1});
#endif

/*
 * BM_environment<std::string>
 */
BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (STRING_ITERS != -1)
    ->Iterations(STRING_ITERS)
#endif
    ->Args({STRING_TIMES, 2});

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (STRING_ITERS != -1)
    ->Iterations(STRING_ITERS)
#endif
    ->Args({STRING_TIMES, 2});
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector_alt, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (STRING_ITERS != -1)
    ->Iterations(STRING_ITERS)
#endif
    ->Args({STRING_TIMES, 2});
#endif

/*
 * BM_environment<test_type>
 */
BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (TEST_TYPE_ITERS != -1)
    ->Iterations(TEST_TYPE_ITERS)
#endif
    ->Args({TEST_TYPE_TIMES, 3});

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (TEST_TYPE_ITERS != -1)
    ->Iterations(TEST_TYPE_ITERS)
#endif
    ->Args({TEST_TYPE_TIMES, 3});
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector_alt, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (TEST_TYPE_ITERS != -1)
    ->Iterations(TEST_TYPE_ITERS)
#endif
    ->Args({TEST_TYPE_TIMES, 3});
#endif

/*
 * BM_environment<int, std::string>
 */
BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ITERS != -1)
    ->Iterations(INT_STRING_ITERS)
#endif
    ->Args({INT_STRING_TIMES, 4});

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ITERS != -1)
    ->Iterations(INT_STRING_ITERS)
#endif
    ->Args({INT_STRING_TIMES, 4});
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector_alt, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ITERS != -1)
    ->Iterations(INT_STRING_ITERS)
#endif
    ->Args({INT_STRING_TIMES, 4});
#endif

/*
 * BM_environment<int, std::string, std::array<int, 10>>
 */
BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ARRAY_ITERS != -1)
    ->Iterations(INT_STRING_ARRAY_ITERS)
#endif
    ->Args({INT_STRING_ARRAY_TIMES, 5});

#ifdef DO_STATIC_VECTOR_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ARRAY_ITERS != -1)
    ->Iterations(INT_STRING_ARRAY_ITERS)
#endif
    ->Args({INT_STRING_ARRAY_TIMES, 5});
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector_alt, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ARRAY_ITERS != -1)
    ->Iterations(INT_STRING_ARRAY_ITERS)
#endif
    ->Args({INT_STRING_ARRAY_TIMES, 5});
#endif

BENCHMARK_MAIN();
