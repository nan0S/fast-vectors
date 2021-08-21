#include <getopt.h>
#include <unistd.h>
#include <boost/container/static_vector.hpp>
#include <static_vector.hpp>
#include <static_vector_alt.hpp>
#include <test_type/test_type.hpp>

#include "vector_benchmark_base.hpp"

using namespace benchmark;

/*
 * configurable parameters
 */

/* number of iterations in vector environment */
static  int  INT_ARG               =  1000;
static  int  STRING_ARG            =  900;
static  int  TEST_TYPE_ARG         =  1000;
static  int  ARRAY_ARG             =  900;
static  int  INT_STRING_ARG        =  900;
static  int  INT_STRING_ARRAY_ARG  =  700;

/* use the same number of iterations in all benchmarks */
static  int  COMMON_ITERS = 0;

static  int  INT_ITERS               (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS);
static  int  STRING_ITERS            (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS);
static  int  TEST_TYPE_ITERS         (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS);
static  int  ARRAY_ITERS             (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS);
static  int  INT_STRING_ITERS        (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS);
static  int  INT_STRING_ARRAY_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS);

/* turn on/off bencharmsk for specific vectors */
static  bool  DO_BOOST_STATIC_VECTOR_BENCH    =  1;
static  bool  DO_UWR_STATIC_VECTOR_BENCH      =  1;
static  bool  DO_UWR_STATIC_VECTOR_ALT_BENCH  =  1;

/* turn off some vectors from even compiling */
#define TURN_ON_BOOST_STATIC_VECTOR_BENCH
#define TURN_ON_UWR_STATIC_VECTOR_BENCH
#define TURN_ON_UWR_STATIC_VECTOR_ALT_BENCH

/* default benchmark type to run */
static int benchmark_type = bench_type::ALL;

/* verbose level:
    - 0: nothing
    - 1: vector environment statistics
    - 2: allocator counters
    - 3: bench timer statistics
    - 4: test_type stats */
static int verbose = 0;

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
        {  "do_boost_static_vector",    optional_argument,  0,  0    },
        {  "do_uwr_static_vector",      optional_argument,  0,  1    },
        {  "do_uwr_static_vector_alt",  optional_argument,  0,  2    },
        {  "common_iters",              required_argument,  0,  3    },
        {  "int_arg",                   required_argument,  0,  4    },
        {  "int_iters",                 required_argument,  0,  5    },
        {  "string_arg",                required_argument,  0,  6    },
        {  "string_iters",              required_argument,  0,  7    },
        {  "test_type_arg",             required_argument,  0,  8    },
        {  "test_type_iters",           required_argument,  0,  9    },
        {  "array_arg",                 required_argument,  0,  10   },
        {  "array_iters",               required_argument,  0,  11   },
        {  "int_string_arg",            required_argument,  0,  12   },
        {  "int_string_iters",          required_argument,  0,  13   },
        {  "int_string_array_arg",      required_argument,  0,  14   },
        {  "int_string_array_iters",    required_argument,  0,  15   },
        {  "push_only",                 no_argument,        0,  16   },
        {  "push_cons_dest",            no_argument,        0,  17   },
        {  "all",                       no_argument,        0,  18   },
        {  "verbose",                   required_argument,  0,  19   },
        {  "help",                      no_argument,        0,  'h'  },
    };
    static const char* shortopts = "h";
    static const char* helpstr =
        "List of possible options:\n"
        "\t--do_boost_static_vector[=0/1]\n"
        "\t--do_uwr_static_vector[=0/1]\n"
        "\t--do_uwr_static_vector_alt[=0/1]\n"
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
            case 0: SetOptVar(DO_BOOST_STATIC_VECTOR_BENCH); break;
            case 1: SetOptVar(DO_UWR_STATIC_VECTOR_BENCH); break;
            case 2: SetOptVar(DO_UWR_STATIC_VECTOR_ALT_BENCH); break;
            case 3: SetReqVar(COMMON_ITERS); break;
            case 4: SetReqVar(INT_ARG); break;
            case 5: SetReqVar(INT_ITERS); break;
            case 6: SetReqVar(STRING_ARG); break;
            case 7: SetReqVar(STRING_ITERS); break;
            case 8: SetReqVar(TEST_TYPE_ARG); break;
            case 9: SetReqVar(TEST_TYPE_ITERS); break;
            case 10: SetReqVar(ARRAY_ARG); break;
            case 11: SetReqVar(ARRAY_ITERS); break;
            case 12: SetReqVar(INT_STRING_ARG); break;
            case 13: SetReqVar(INT_STRING_ITERS); break;
            case 14: SetReqVar(INT_STRING_ARRAY_ARG); break;
            case 15: SetReqVar(INT_STRING_ARRAY_ITERS); break;
            case 16: benchmark_type |= bench_type::PUSH_ONLY; break;
            case 17: benchmark_type |= bench_type::PUSH_CONS_DEST; break;
            case 18: benchmark_type |= bench_type::ALL; break;
            case 19: SetReqVar(verbose); break;
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

#ifdef TURN_ON_BOOST_STATIC_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_BOOST_STATIC_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_BOOST_STATIC_VECTOR_BENCH, unit, varname, counter, type, boost_static_vector, __VA_ARGS__)
#else
#define REREGISTER_BENCHMARK_FOR_BOOST_STATIC_VECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_UWR_STATIC_VECTOR_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_STATIC_VECTOR(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_UWR_STATIC_VECTOR_BENCH, unit, varname, counter, type, uwr_static_vector, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_UWR_STATIC_VECTOR(unit, varname, counter, type, ...)
#endif

#ifdef TURN_ON_UWR_STATIC_VECTOR_ALT_BENCH
#define REGISTER_BENCHMARK_FOR_UWR_STATIC_VECTOR_ALT(unit, varname, counter, type, ...) \
    COND_REGISTER_BENCHMARK_FOR_VECTOR(DO_UWR_STATIC_VECTOR_ALT_BENCH, unit, varname, counter, type, uwr_static_vector_alt, __VA_ARGS__)
#else
#define REGISTER_BENCHMARK_FOR_UWR_STATIC_VECTOR_ALT(unit, varname, counter, type, ...)
#endif

#define REGISTER_BENCHMARK(unit, varname, counter, type, ...) \
    REGISTER_BENCHMARK_FOR_BOOST_STATIC_VECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_UWR_STATIC_VECTOR(unit, varname, counter, type, __VA_ARGS__); \
    REGISTER_BENCHMARK_FOR_UWR_STATIC_VECTOR_ALT(unit, varname, counter, type, __VA_ARGS__)

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
