#include <static_vector.hpp>
#include <static_vector_alt.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>

#include "vector_benchmark_base.hpp"

using namespace benchmark;

/*
 * configurable parameters
 */
static  constexpr  int  INT_ARG               =  500;
static  constexpr  int  STRING_ARG            =  300;
static  constexpr  int  TEST_TYPE_ARG         =  300;
static  constexpr  int  ARRAY_ARG             =  200;
static  constexpr  int  INT_STRING_ARG        =  200;
static  constexpr  int  INT_STRING_ARRAY_ARG  =  100;

/* use the same number of iterations in all benchmarks */
#define COMMON_ITERS 100

#define  INT_ITERS               (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  STRING_ITERS            (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  TEST_TYPE_ITERS         (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  ARRAY_ITERS             (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INT_STRING_ITERS        (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INT_STRING_ARRAY_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)

#define DO_STATIC_VECTOR_BENCH
#define DO_STATIC_VECTOR_ALT_BENCH

static constexpr int C = 500000; // static_vector capacity
static constexpr int N = 10;     // used in std::array<int, N>

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
 * some macro magic
 */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, vector, ...) \
    BENCHMARK_TEMPLATE(BM_environment, vector, __VA_ARGS__) \
        ->Unit(unit) \
        ->Iterations(CONCAT(varname, _ITERS)) \
        ->Args({CONCAT(varname, _ARG), counter})

#ifdef DO_STATIC_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, uwr_static_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR(unit, varname, counter, ...)
#endif

#ifdef DO_STATIC_VECTOR_ALT_BENCH
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR_ALT(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, uwr_static_vector_alt, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_STATIC_VECTOR_ALT(unit, varname, counter, ...)
#endif

#define REGISTER_BENCHMARK(unit, varname, counter, ...) \
    REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, boost_static_vector, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_STATIC_VECTOR(unit, varname, counter, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_STATIC_VECTOR_ALT(unit, varname, counter, __VA_ARGS__)

/*
 * register all benchmarks
 */
REGISTER_BENCHMARK(kMillisecond,  INT,               1,  int);                                            
REGISTER_BENCHMARK(kMillisecond,  STRING,            2,  std::string);                                    
REGISTER_BENCHMARK(kMillisecond,  TEST_TYPE,         3,  test_type);                                      
REGISTER_BENCHMARK(kMillisecond,  ARRAY,             4,  std::array<int, N>);                            
REGISTER_BENCHMARK(kMillisecond,  INT_STRING,        5,  int, std::string);                   
REGISTER_BENCHMARK(kMillisecond,  INT_STRING_ARRAY,  6,  int, std::string, std::array<int, N>);

BENCHMARK_MAIN();
