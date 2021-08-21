#include <getopt.h>
#include <unistd.h>
#include <bits/getopt_ext.h>
#include <test_type/test_type.hpp>
#include <identifiers/identifiers.hpp>
#include <boost/container/vector.hpp>
#include <vector>
#include <rvector.h>
#include <vector.hpp>
#include <std_vector.hpp>
#include <big_vector.hpp>
#include <c_vector.hpp>

#include "vector_benchmark_base.hpp"

using namespace benchmark;

/*
 * configurable parameters
 */

/* number of iterations in vector environment */
static  int  INT_ARG               =  2000;
static  int  STRING_ARG            =  1000;
static  int  TEST_TYPE_ARG         =  1000;
static  int  ARRAY_ARG             =  1200;
static  int  INT_STRING_ARG        =  1000;
static  int  INT_STRING_ARRAY_ARG  =  1000;

/* use the same number of iterations in all benchmarks */
static int COMMON_ITERS = 0;

static  int  INT_ITERS               =  COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS;
static  int  STRING_ITERS            =  COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS;
static  int  TEST_TYPE_ITERS         =  COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS;
static  int  ARRAY_ITERS             =  COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS;
static  int  INT_STRING_ITERS        =  COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS;
static  int  INT_STRING_ARRAY_ITERS  =  COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS;

/* turn on/off benchamarks for specific vectors */
static  bool  DO_STD_VECTOR_BENCH      =  1;
static  bool  DO_BOOST_VECTOR_BENCH    =  0;
static  bool  DO_RVECTOR_BENCH         =  1;
static  bool  DO_UWR_VECTOR_BENCH      =  1;
static  bool  DO_UWR_STD_VECTOR_BENCH  =  0;
static  bool  DO_BIG_VECTOR_BENCH      =  0;
static  bool  DO_C_VECTOR_BENCH        =  1;

/* turn off some vectors from even compiling */
#define TURN_ON_STD_VECTOR_BENCH
// #define TURN_ON_BOOST_VECTOR_BENCH
#define TURN_ON_RVECTOR_BENCH
#define TURN_ON_UWR_VECTOR_BENCH
// #define TURN_ON_UWR_STD_VECTOR_BENCH
// #define TURN_ON_BIG_VECTOR_BENCH
#define TURN_ON_C_VECTOR_BENCH

/* default benchmark type to run */
static int benchmark_type = bench_type::PUSH_ONLY;

/* verbose level:
    - 0: nothing
    - 1: vector environment statistics
    - 2: allocator counters
    - 3: bench timer statistics
    - 4: test_type stats */
static int verbose = 0;

/* used in std::array<int, N> */
static constexpr int N = 10;

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
using big_vector = uwr::big_vector<T>;
template<class T>
using c_vector = uwr::c_vector<T>;

/*
 * option parsing functions
 */
static void SetOptVar(bool& var, bool def=true) {
    if (optarg)
        def = std::atoi(optarg);
    var = def;
}

static void SetReqVar(int& var) {
    var = std::atoi(optarg);
}

static void ParseCustomOptions(int argc, char** argv) {
    static struct option longopts[] = {
        { "do_std_vector", optional_argument, 0, 0 },
        { "do_boost_vector", optional_argument, 0, 1 },
        { "do_rvector", optional_argument, 0, 2 },
        { "do_uwr_vector", optional_argument, 0, 3 },
        { "do_uwr_std_vector", optional_argument, 0, 4 },
        { "do_big_vector", optional_argument, 0, 5 },
        { "do_c_vector", optional_argument, 0, 6 },
        { "common_iters", required_argument, 0, 7 },
        { "int_arg", required_argument, 0, 8 },
        { "int_iters", required_argument, 0, 9 },
        { "string_arg", required_argument, 0, 10 },
        { "string_iters", required_argument, 0, 11 },
        { "test_type_arg", required_argument, 0, 12 },
        { "test_type_iters", required_argument, 0, 13 },
        { "array_arg", required_argument, 0, 14 },
        { "array_iters", required_argument, 0, 15 },
        { "int_string_arg", required_argument, 0, 16 },
        { "int_string_iters", required_argument, 0, 17 },
        { "int_string_array_arg", required_argument, 0, 18 },
        { "int_string_array_iters", required_argument, 0, 19 },
        { "push_only", no_argument, 0, 20 },
        { "push_cons_dest", no_argument, 0, 21 },
        { "all", no_argument, 0, 22 },
        { "verbose", required_argument, 0, 23 },
        { "help", no_argument, 0, 'h' },
    };
    static const char* shortopts = "h";
    static const char* helpstr =
        "List of possible options:\n"
        "\t--do_std_vector[=0/1]\n"
        "\t--do_boost_vector[=0/1]\n"
        "\t--do_rvector[=0/1]\n"
        "\t--do_uwr_vector[=0/1]\n"
        "\t--do_uwr_std_vector[=0/1]\n"
        "\t--do_big_vector[=0/1]\n"
        "\t--do_c_vector[=0/1]\n"
        "\t--common_iters=VALUE\n"
        "\t--int_arg=VALUE\n"
        "\t--int_iters=VALUE\n"
        "\t--string_arg=VALUE\n"
        "\t--string_iters=VALUE\n"
        "\t--test_type_arg=VALUE\n"
        "\t--test_type_iters=VALUE\n"
        "\t--array_arg=VALUE\n"
        "\t--array_iters=VALUE\n"
        "\t--int_string_arg=VALUE\n"
        "\t--int_string_iters=VALUE\n"
        "\t--int_string_array_arg=VALUE\n"
        "\t--int_string_array_iters=VALUE\n"
        "\t--push_only\n"
        "\t--push_cons_dest\n"
        "\t--all\n"
        "\t--verbose=VALUE\n"
        "\t-h,--help\n";

    opterr = 0;
    benchmark_type = 0;

    while (true) {
        int idx, opt = getopt_long(argc, argv, shortopts,
                                   longopts, &idx);
        if (opt == -1)
            break;

        switch (opt) {
            case 0: SetOptVar(DO_STD_VECTOR_BENCH); break;
            case 1: SetOptVar(DO_BOOST_VECTOR_BENCH); break;
            case 2: SetOptVar(DO_RVECTOR_BENCH); break;
            case 3: SetOptVar(DO_UWR_VECTOR_BENCH); break;
            case 4: SetOptVar(DO_UWR_STD_VECTOR_BENCH); break;
            case 5: SetOptVar(DO_BIG_VECTOR_BENCH); break;
            case 6: SetOptVar(DO_C_VECTOR_BENCH); break;
            case 7: SetReqVar(COMMON_ITERS); break;
            case 8: SetReqVar(INT_ARG); break;
            case 9: SetReqVar(INT_ITERS); break;
            case 10: SetReqVar(STRING_ARG); break;
            case 11: SetReqVar(STRING_ITERS); break;
            case 12: SetReqVar(TEST_TYPE_ARG); break;
            case 13: SetReqVar(TEST_TYPE_ITERS); break;
            case 14: SetReqVar(ARRAY_ARG); break;
            case 15: SetReqVar(ARRAY_ITERS); break;
            case 16: SetReqVar(INT_STRING_ARG); break;
            case 17: SetReqVar(INT_STRING_ITERS); break;
            case 18: SetReqVar(INT_STRING_ARRAY_ARG); break;
            case 19: SetReqVar(INT_STRING_ARRAY_ITERS); break;
            case 20: benchmark_type |= bench_type::PUSH_ONLY; break;
            case 21: benchmark_type |= bench_type::PUSH_CONS_DEST; break;
            case 22: benchmark_type |= bench_type::ALL; break;
            case 23: SetReqVar(verbose); break;
            case 'h': printf("%s", helpstr); break;
            default: break;
        }
    }

    if (!benchmark_type)
        benchmark_type = bench_type::PUSH_ONLY;
}

/*
 * some macro magic
 */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, type, vector, ...) \
    RegisterBenchmark("BM_environment<" #vector ", " #__VA_ARGS__ ">", BM_environment<vector, __VA_ARGS__>, type, verbose) \
        ->Unit(unit) \
        ->Iterations(CONCAT(varname, _ITERS)) \
        ->Args({CONCAT(varname, _ARG), counter})

#define COND_REGISTER_BENCHMARK_FOR_VECTOR(cond, unit, varname, counter, type, vector, ...) \
    if (cond) \
        REGISTER_BENCHMARK_FOR_VECTOR(unit, varname, counter, type, vector, __VA_ARGS__)

#ifdef TURN_ON_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_STD_VECTOR_BENCH, unit, varname, counter, type, std_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_STD_VECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_BOOST_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_BOOST_VECTOR_BENCH, unit, varname, counter, type, boost_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_BOOST_VECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_RVECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_RVECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_RVECTOR_BENCH, unit, varname, counter, type, rvector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_RVECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_UWR_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_UWR_VECTOR_BENCH, unit, varname, counter, type, uwr_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_UWR_VECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_UWR_STD_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_UWR_STD_VECTOR_BENCH, unit, varname, counter, type, uwr_std_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_BIG_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_BIG_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_BIG_VECTOR_BENCH, unit, varname, counter, type, big_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_BIG_VECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_C_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_C_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_C_VECTOR_BENCH, unit, varname, counter, type, c_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_C_VECTOR(unit, varname, counter, type, ...)
#endif

#define REGISTER_BENCHMARK(unit, varname, counter, type, ...) \
    REGISTER_BENCHMARK_FOR_STD_VECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_BOOST_VECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_RVECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_UWR_VECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_UWR_STD_VECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_BIG_VECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_C_VECTOR(unit, varname, counter, type, __VA_ARGS__)

static void RegisterBenchmarkForType(bench_type type) {
    std::cout << boost::format("==== Running: %s benchmark. ====\n") % std::to_string(type).c_str();
    REGISTER_BENCHMARK(kMillisecond,  INT,               1,  type, int);
    REGISTER_BENCHMARK(kMillisecond,  STRING,            2,  type, std::string);
    REGISTER_BENCHMARK(kMillisecond,  TEST_TYPE,         3,  type, test_type);
    REGISTER_BENCHMARK(kMillisecond,  ARRAY,             4,  type, std::array<int, N>);
    REGISTER_BENCHMARK(kMillisecond,  INT_STRING,        5,  type, int, std::string);
    REGISTER_BENCHMARK(kMillisecond,  INT_STRING_ARRAY,  6,  type, int, std::string, std::array<int, N>);
}

int main(int argc, char** argv) {
    /* turn on thounsand commas when printing */
    std::cout.imbue(std::locale(""));
    // mallopt(M_MMAP_THRESHOLD, 64 * 1024 * 1024);

    ParseCustomOptions(argc, argv);
    Initialize(&argc, argv);

    if (benchmark_type & bench_type::PUSH_ONLY)
        RegisterBenchmarkForType(bench_type::PUSH_ONLY);
    if (benchmark_type & bench_type::PUSH_CONS_DEST)
        RegisterBenchmarkForType(bench_type::PUSH_CONS_DEST);
    if (benchmark_type & bench_type::ALL)
        RegisterBenchmarkForType(bench_type::ALL);

    RunSpecifiedBenchmarks();
    Shutdown();
    
    return 0;
}
