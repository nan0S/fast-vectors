#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>

#include "vector_benchmark_base.hpp"

static constexpr uint C = 500000;

template<class T>
using ustatic_vector = uwr::static_vector<T, C>;
template<class T>
using bstatic_vector = boost::container::static_vector<T, C>;

BENCHMARK_TEMPLATE(BM_environment, bstatic_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(500);
BENCHMARK_TEMPLATE(BM_environment, ustatic_vector, int)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(500);

BENCHMARK_TEMPLATE(BM_environment, bstatic_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(300);
BENCHMARK_TEMPLATE(BM_environment, ustatic_vector, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(300);

BENCHMARK_TEMPLATE(BM_environment, bstatic_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(300);
BENCHMARK_TEMPLATE(BM_environment, ustatic_vector, test_type)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(300);

BENCHMARK_TEMPLATE(BM_environment, bstatic_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(200);
BENCHMARK_TEMPLATE(BM_environment, ustatic_vector, int, std::string)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(200);

BENCHMARK_TEMPLATE(BM_environment, bstatic_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(100);
BENCHMARK_TEMPLATE(BM_environment, ustatic_vector, int, std::string, std::array<int, 10>)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(100);

BENCHMARK_MAIN();
