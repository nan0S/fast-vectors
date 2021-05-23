#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>

#include "vector_benchmark_base.hpp"

static constexpr uint C = 500000;

template<class T>
using uwr_static_vector = uwr::static_vector<T, C>;
template<class T>
using boost_static_vector = boost::container::static_vector<T, C>;

static constexpr int INT_ITERS = 500;
static constexpr int STRING_ITERS = 300;
static constexpr int TEST_TYPE_ITERS = 300;
static constexpr int INT_STRING_ITERS = 200;
static constexpr int INT_STRING_ARRAY_ITERS = 100;

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_ITERS, 1});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_ITERS, 1});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({STRING_ITERS, 2});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({STRING_ITERS, 2});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({TEST_TYPE_ITERS, 3});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({TEST_TYPE_ITERS, 3});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ITERS, 4});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ITERS, 4});

BENCHMARK_TEMPLATE(BM_environment, boost_static_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ARRAY_ITERS, 5});
BENCHMARK_TEMPLATE(BM_environment, uwr_static_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ARRAY_ITERS, 5});

BENCHMARK_MAIN();
