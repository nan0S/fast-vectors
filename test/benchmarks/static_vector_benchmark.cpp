#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>

#include "vector_benchmark_base.hpp"

static constexpr uint C = 500000;

template<class T>
using uwr_static_vector = uwr::static_vector<T, C>;
template<class T>
using boost_static_vector = boost::container::static_vector<T, C>;

static constexpr int INT_TIMES = 500;
#define INT_ITERS -1
static constexpr int STRING_TIMES = 300;
#define STRING_ITERS -1
static constexpr int TEST_TYPE_TIMES = 300;
#define TEST_TYPE_ITERS -1
static constexpr int INT_STRING_TIMES = 200;
#define INT_STRING_ITERS -1
static constexpr int INT_STRING_ARRAY_TIMES = 100;
#define INT_STRING_ARRAY_ITERS -1

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_ITERS != -1)
    ->Iterations(INT_ITERS)
#endif
    ->Args({INT_TIMES, 1});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_ITERS != -1)
    ->Iterations(INT_ITERS)
#endif
    ->Args({INT_TIMES, 1});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (STRING_ITERS != -1)
    ->Iterations(STRING_ITERS)
#endif
    ->Args({STRING_TIMES, 2});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (STRING_ITERS != -1)
    ->Iterations(STRING_ITERS)
#endif
    ->Args({STRING_TIMES, 2});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (TEST_TYPE_ITERS != -1)
    ->Iterations(TEST_TYPE_ITERS)
#endif
    ->Args({TEST_TYPE_TIMES, 3});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (TEST_TYPE_ITERS != -1)
    ->Iterations(TEST_TYPE_ITERS)
#endif
    ->Args({TEST_TYPE_TIMES, 3});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ITERS != -1)
    ->Iterations(INT_STRING_ITERS)
#endif
    ->Args({INT_STRING_TIMES, 4});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ITERS != -1)
    ->Iterations(INT_STRING_ITERS)
#endif
    ->Args({INT_STRING_TIMES, 4});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ARRAY_ITERS != -1)
    ->Iterations(INT_STRING_ARRAY_ITERS)
#endif
    ->Args({INT_STRING_ARRAY_TIMES, 5});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (INT_STRING_ARRAY_ITERS != -1)
    ->Iterations(INT_STRING_ARRAY_ITERS)
#endif
    ->Args({INT_STRING_ARRAY_TIMES, 5});

BENCHMARK_MAIN();
