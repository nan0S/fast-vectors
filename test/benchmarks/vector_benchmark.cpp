#include <test_type/test_type.hpp>
#include <boost/container/vector.hpp>
#include <vector>
#include <rvector.h>
#include <vector.hpp>
#include <std_vector.hpp>
#include <simple_vector.hpp>
#include <big_vector.hpp>

#include "vector_benchmark_base.hpp"

using namespace benchmark;

/*
 * configurable parameters
 */
static  constexpr  int  INT_ARG               =  2000;
static  constexpr  int  STRING_ARG            =  1000;
static  constexpr  int  TEST_TYPE_ARG         =  1000;
static  constexpr  int  ARRAY_ARG             =  1200;
static  constexpr  int  INT_STRING_ARG        =  1000;
static  constexpr  int  INT_STRING_ARRAY_ARG  =  1000;

/* use the same number of iterations in all benchmarks */
#define COMMON_ITERS 0

#define  INT_ITERS               (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  STRING_ITERS            (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  TEST_TYPE_ITERS         (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  ARRAY_ITERS             (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INT_STRING_ITERS        (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INT_STRING_ARRAY_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)

#define DO_BOOST_VECTOR_BENCH
#define DO_STD_VECTOR_BENCH
#define DO_RVECTOR_BENCH
#define DO_UWR_VECTOR_BENCH
#define DO_UWR_STD_VECTOR_BENCH
#define DO_SIMPLE_VECTOR_BENCH
// #define DO_UWR_BIG_VECTOR_BENCH

static constexpr int N = 10; // used in std::array<int, N>

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
using uwr_vector = uwr::vector<T>;
template<class T>
using uwr_std_vector = uwr::std_vector<T>;
template<class T>
using simple_vector = uwr::simple_vector<T>;
template<class T>
using uwr_big_vector = uwr::big_vector<T>;

/*
 * some macro magic
 */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, vector, ...) \
    BENCHMARK_TEMPLATE(BM_environment, vector, __VA_ARGS__) \
        ->Unit(unit) \
        ->Iterations(CONCAT(varname, _ITERS)) \
        ->Args({CONCAT(varname, _ARG), counter})

#ifdef DO_BOOST_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, boost_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(unit, varname, counter, ...)
#endif

#ifdef DO_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, std_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(unit, varname, counter, ...)
#endif

#ifdef DO_RVECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_RVECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, rvector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_RVECTOR(unit, varname, counter, ...)
#endif

#ifdef DO_UWR_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, uwr_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(unit, varname, counter, ...)
#endif

#ifdef DO_UWR_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, uwr_std_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(unit, varname, counter, ...)
#endif

#ifdef DO_SIMPLE_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_SIMPLE_VECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, simple_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_SIMPLE_VECTOR(unit, varname, counter, ...)
#endif

#ifdef DO_UWR_BIG_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_BIG_VECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, uwr_big_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_UWR_BIG_VECTOR(unit, varname, counter, ...)
#endif

#define REGISTER_BENCHMARK(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_STD_VECTOR(unit, varname, counter, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_BOOST_VECTOR(unit, varname, counter, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_RVECTOR(unit, varname, counter, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_UWR_VECTOR(unit, varname, counter, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(unit, varname, counter, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_SIMPLE_VECTOR(unit, varname, counter, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_UWR_BIG_VECTOR(unit, varname, counter, __VA_ARGS__)

/*
 * register all benchmarks
 */
REGISTER_BENCHMARK(kMillisecond,  INT,               1,  int);                                            
REGISTER_BENCHMARK(kMillisecond,  STRING,            2,  std::string);                                    
REGISTER_BENCHMARK(kMillisecond,  TEST_TYPE,         3,  test_type);                                      
REGISTER_BENCHMARK(kMillisecond,  ARRAY,             4,  std::array<int, N>);                            
REGISTER_BENCHMARK(kMillisecond,  INT_STRING,        5,  int, std::string);                   
REGISTER_BENCHMARK(kMillisecond,  INT_STRING_ARRAY,  6,  int, std::string, std::array<int, N>);

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
