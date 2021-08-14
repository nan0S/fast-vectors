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
using value_type = std::string;

// number of consecutive push backs in one iteration
static const args_t PUSH_BACK_ARG = { 50000 };
// number of push_back/pop_back rounds in iteration and maximum vector size
static const args_t PUSH_BACK_POP_BACK_ARG = { 10, 50000 };
// number of resize rounds and maximum resize size
static const args_t RESIZE_ARG = { 10, 50000 };
// maximum size of created vector
static const args_t INSERT_ARG = { 1000 };
// maximum size of created vector
static const args_t CREATE_ARG = { 50000 };

/* use the same number of iterations in all benchmarks */
#define COMMON_ITERS 0

#define  PUSH_BACK_ITERS           (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  PUSH_BACK_POP_BACK_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  RESIZE_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INSERT_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  CREATE_ITERS              (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)

#define DO_STD_VECTOR_BENCH
#define DO_BOOST_VECTOR_BENCH
#define DO_UWR_VECTOR_BENCH
#define DO_RVECTOR_BENCH
#define DO_UWR_STD_VECTOR_BENCH

/*
 * tested vectors
 */
using std_vector = std::vector<value_type>;
using boost_gf = boost::container::growth_factor_100;
using boost_options = boost::container::vector_options_t<boost::container::growth_factor<boost_gf>>;
using boost_vector = boost::container::vector<value_type, boost::container::new_allocator<value_type>, boost_options>; 
// using boost_vector = boost::container::vector<value_type>;
using uwr_vector = uwr::vector<value_type>;
using rvector_t = rvector<value_type>;
using uwr_std_vector = uwr::std_vector<value_type>;

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
    int max_size = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());

        for (int i = 0; i < times; ++i) {
            if (Random::rand(2)) {
                size_type rest = max_size - v.size();
                size_type c = Random::rand<size_type>(rest + 1);

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
 * benchmark resizing vector
 */
template<class Vector>
void BM_resize(State& s) {
    Random::seed(31231);

    int times = s.range(0);
    int max_size = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());

        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(max_size + 1);
            v.resize(new_size);
            ClobberMemory();
        }
    }

    s.counters["t3"];
}

/*
 * benchmark inserting to vector
 */
template<class Vector>
void BM_insert(State& s) {
    using size_type = typename Vector::size_type;
    using T = typename Vector::value_type;
    Random::seed(123);

    size_type max_size = s.range(0);

    for (auto _ : s) {
        size_type size = Random::rand(max_size + 1);
        Vector v(size, get_value<T>(size + 10));

        DoNotOptimize(v.data());
        
        for (int i = 0; i < 100; ++i) {
            size_type pos = Random::rand(size + 1);
            size_type count = Random::rand(size / 10 + 1);

            v.insert(v.begin() + pos, count, get_value<T>(size));
            ClobberMemory();
        }

    }
    
    s.counters["t4"];
}

/*
 * benchmark creating vector
 */
template<class Vector>
void BM_create(State& s) {
    using size_type = typename Vector::size_type;
    Random::seed(123);

    size_type max_size = s.range(0);

    for (auto _ : s) {
        size_type size = Random::rand(max_size + 1);
        Vector v(size);

        DoNotOptimize(v.data());
        ClobberMemory();
    }
    
    s.counters["t5"];
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
        ->Args(CONCAT(varname, _ARG))

#ifdef DO_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, std_vector)
#else
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(func, unit, varname)
#endif

#ifdef DO_BOOST_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, boost_vector)
#else
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(func, unit, varname)
#endif

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

#ifdef DO_UWR_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_VECTOR(func, unit, varname, uwr_std_vector)
#else
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(func, unit, varname)
#endif

#define REGISTER_BENCHMARK(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_STD_VECTOR(func, unit, varname); \
    REGISTER_BENCHMARK_FOR_BOOST_VECTOR(func, unit, varname); \
    REGISTER_BENCHMARK_FOR_RVECTOR(func, unit, varname); \
    REGISTER_BENCHMARK_FOR_UWR_VECTOR(func, unit, varname); \
    REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(func, unit, varname)
/*
 * register all benchmarks
 */
REGISTER_BENCHMARK(BM_push_back,           kMicrosecond,  PUSH_BACK);
REGISTER_BENCHMARK(BM_push_back_pop_back,  kMicrosecond,  PUSH_BACK_POP_BACK);
REGISTER_BENCHMARK(BM_resize,              kMicrosecond,  RESIZE);
REGISTER_BENCHMARK(BM_insert,              kMicrosecond,  INSERT);
REGISTER_BENCHMARK(BM_create,              kMicrosecond,  CREATE);

int main(int argc, char** argv) {
    Initialize(&argc, argv);
    if (ReportUnrecognizedArguments(argc, argv))
        return 1;
    RunSpecifiedBenchmarks();
    Shutdown();
    
    std::cout << "uwr::success: " << uwr::mem::counters::success << std::endl;
    std::cout << "uwr::mremaps: " << uwr::mem::counters::mremaps << std::endl;
    std::cout << "rve::success: " << mm::success << std::endl;
    std::cout << "rve::mremaps: " << mm::mremaps << std::endl;

    return 0;
}
