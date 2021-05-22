#include <vector.hpp>
#include <boost/container/vector.hpp>
#include <vector>
#include <test_type/test_type.hpp>

#include "vector_benchmark_base.hpp"

template<class T>
using uwr_vector = uwr::vector<T>;
template<class T>
using boost_vector = boost::container::vector<T>;
template<class T>
using std_vector = std::vector<T>;

static constexpr int INT_ITERS = 600;
static constexpr int STRING_ITERS = 600;
static constexpr int TEST_TYPE_ITERS = 600;
static constexpr int INT_STRING_ITERS = 600;
static constexpr int INT_STRING_ARRAY_ITERS = 600;

BENCHMARK_TEMPLATE(BM_environment, std_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_ITERS, 1});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_ITERS, 1});
BENCHMARK_TEMPLATE(BM_environment, uwr_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_ITERS, 1});

BENCHMARK_TEMPLATE(BM_environment, std_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({STRING_ITERS, 2});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({STRING_ITERS, 2});
BENCHMARK_TEMPLATE(BM_environment, uwr_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({STRING_ITERS, 2});

BENCHMARK_TEMPLATE(BM_environment, std_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({TEST_TYPE_ITERS, 3});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({TEST_TYPE_ITERS, 3});
BENCHMARK_TEMPLATE(BM_environment, uwr_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({TEST_TYPE_ITERS, 3});

BENCHMARK_TEMPLATE(BM_environment, std_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ITERS, 4});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ITERS, 4});
BENCHMARK_TEMPLATE(BM_environment, uwr_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ITERS, 4});

BENCHMARK_TEMPLATE(BM_environment, std_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ARRAY_ITERS, 5});
BENCHMARK_TEMPLATE(BM_environment, boost_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ARRAY_ITERS, 5});
BENCHMARK_TEMPLATE(BM_environment, uwr_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
    ->Args({INT_STRING_ARRAY_ITERS, 5});

BENCHMARK_MAIN();
