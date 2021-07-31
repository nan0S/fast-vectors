#include <vector.hpp>
#include <boost/container/vector.hpp>
#include <vector>
#include <test_type/test_type.hpp>
#include <rvector.h>

#include "vector_benchmark_base.hpp"

template<class T>
using uwr_vector = uwr::vector<T>;
template<class T>
using boost_vector = boost::container::vector<T>;
template<class T>
using std_vector = std::vector<T>;

static constexpr int N = 10;

static constexpr int INT_TIMES = 2000;
static constexpr int STRING_TIMES = 1000;
static constexpr int TEST_TYPE_TIMES = 1200;
static constexpr int ARRAY_TIMES = 1200;
static constexpr int INT_STRING_ARRAY_TIMES = 1000;

/*
 * int benchmark
 */
BENCHMARK_TEMPLATE(BM_environment, std_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->Args({INT_TIMES, 1});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->Args({INT_TIMES, 1});
// BENCHMARK_TEMPLATE(BM_environment, uwr_vector, int)
    // ->Unit(::benchmark::kMillisecond)
    // ->Args({INT_TIMES, 1});
BENCHMARK_TEMPLATE(BM_environment, rvector, int)
    ->Unit(::benchmark::kMillisecond)
    ->Args({INT_TIMES, 1});

/*
 * std::string benchmark
 */
BENCHMARK_TEMPLATE(BM_environment, std_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->Args({STRING_TIMES, 2});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->Args({STRING_TIMES, 2});
// BENCHMARK_TEMPLATE(BM_environment, uwr_vector, std::string)
    // ->Unit(::benchmark::kMillisecond)
    // ->Args({STRING_TIMES, 2});
BENCHMARK_TEMPLATE(BM_environment, rvector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->Args({STRING_TIMES, 2});

/*
 * test_type benchmark
 */
BENCHMARK_TEMPLATE(BM_environment, std_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->Args({TEST_TYPE_TIMES, 3});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->Args({TEST_TYPE_TIMES, 3});
// BENCHMARK_TEMPLATE(BM_environment, uwr_vector, test_type)
    // ->Unit(::benchmark::kMillisecond)
    // ->Args({TEST_TYPE_TIMES, 3});
BENCHMARK_TEMPLATE(BM_environment, rvector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->Args({TEST_TYPE_TIMES, 3});

/*
 * std::array<int, N> benchmark
 */
BENCHMARK_TEMPLATE(BM_environment, std_vector, std::array<int, N>)
    ->Unit(::benchmark::kMillisecond)
    ->Args({ARRAY_TIMES, 4});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, std::array<int, N>)
    ->Unit(::benchmark::kMillisecond)
    ->Args({ARRAY_TIMES, 4});
// BENCHMARK_TEMPLATE(BM_environment, uwr_vector, std::array<int, N>)
    // ->Unit(::benchmark::kMillisecond)
    // ->Args({INT_STRING_TIMES, 4});
BENCHMARK_TEMPLATE(BM_environment, rvector, std::array<int, N>)
    ->Unit(::benchmark::kMillisecond)
    ->Args({ARRAY_TIMES, 4});

/*
 * int, std::string, std::array<int, C> benchmark
 */
BENCHMARK_TEMPLATE(BM_environment, std_vector, int, std::string, std::array<int, N>)
    ->Unit(::benchmark::kMillisecond)
    ->Args({INT_STRING_ARRAY_TIMES, 5});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, int, std::string, std::array<int, N>)
    ->Unit(::benchmark::kMillisecond)
    ->Args({INT_STRING_ARRAY_TIMES, 5});
// BENCHMARK_TEMPLATE(BM_environment, uwr_vector, int, std::string, std::array<int, N>)
    // ->Unit(::benchmark::kMillisecond)
    // ->Args({INT_STRING_ARRAY_TIMES, 5});
BENCHMARK_TEMPLATE(BM_environment, rvector, int, std::string, std::array<int, N>)
    ->Unit(::benchmark::kMillisecond)
    ->Args({INT_STRING_ARRAY_TIMES, 5});

BENCHMARK_MAIN();
