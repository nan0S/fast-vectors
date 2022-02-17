#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

#include <benchmark/benchmark.h>

#include "uwr/static_vector.hpp"

#include "utils/value.hpp"
#include "utils/random.hpp"
#include "test_type/test_type.hpp"

using namespace benchmark; 

/*
 * In multiple places in static_vector implementation (not limited strictly
 * to static_vector but rather general vector problems) likely/unlikely
 * could be used (or additional branches - in other word micro optimizations).
 * These tests provide benchmarks to test what variant is better. Majority of
 * these functions may not exist in static_vector source file (e.g. resize1/resize2)
 * right now, but these benchmarks serve as "documentation", maybe they will be useful
 * in the future.
 */

/*
 * configurable parameters
 */
using value_type = int;

static  constexpr  int  RESIZE_ARG1         =  100;   //  how     many  times    do   resize                             
static  constexpr  int  RESIZE_ARG2         =  1;     //  what    is    maximum  n    in      resize                     
static  constexpr  int  AT_ARG1             =  1;     //  how     big   vector   can  be      at      start  and  index  limit
static  constexpr  int  AT_ARG2             =  -1;    //  ignore                                                         
static  constexpr  int  ASSIGN_ARG1         =  1000;  //  how     many  times    do   assign                             
static  constexpr  int  ASSIGN_ARG2         =  1;     //  what    is    maximum  n    in      assign                     
static  constexpr  int  SAFE_POP_BACK_ARG1  =  1;     //  how     big   vector   can  be      at      start              
static  constexpr  int  SAFE_POP_BACK_ARG2  =  -1;    //  ignore                                                         
static  constexpr  int  INSERT_ARG1         =  1;     //  how     big   vector   can  be      at      start              
static  constexpr  int  INSERT_ARG2         =  -1;    //  ignore                                                         
static  constexpr  int  ERASE_ARG1          =  1;     //  how     big   vector   can  be      at      start              
static  constexpr  int  ERASE_ARG2          =  -1;    //  ignore                                                         
static  constexpr  int  EMPLACE_BACK_ARG1   =  -1;    //  ignore                                                         
static  constexpr  int  EMPLACE_BACK_ARG2   =  -1;    //  ignore                                                         

/* use the same number of iterations in all benchmarks */
#define COMMON_ITERS 0

#define  RESIZE_ITERS         (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  AT_ITERS             (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  ASSIGN_ITERS         (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  SAFE_POP_BACK_ITERS  (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  INSERT_ITERS         (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  ERASE_ITERS          (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)
#define  EMPLACE_BACK_ITERS   (COMMON_ITERS  ==  0  ?  0  :  COMMON_ITERS)

static constexpr int C = 1; // static_vector capacity

static_assert(C  >=  RESIZE_ARG2,         "N  cannot  be  smaller  than  RESIZE_ARG2!");
static_assert(C  >=  AT_ARG1,             "N  cannot  be  smaller  than  AT_ARG1");
static_assert(C  >=  ASSIGN_ARG2,         "N  cannot  be  smaller  than  ASSIGN_ARG2!");
static_assert(C  >=  SAFE_POP_BACK_ARG1,  "N  cannot  be  smaller  than  SAFE_POP_BACK_ARG1!");
static_assert(C  >=  INSERT_ARG1,         "N  cannot  be  smaller  than  INSERT_ARG1!");
static_assert(C  >=  ERASE_ARG1,          "N  cannot  be  smaller  than  ERASE_ARG1!");

using uwr_static_vector = uwr::static_vector<value_type, C>;

#if 0 // turn off because out-of-date

/*
 * benchmark resize
 */
template<class Vector>
void BM_resize_n_1(State& s) {
    Random::seed(1321312);

    int times = s.range(0);
    int size_limit = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());
        
        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(size_limit) + 1;
            v.resize1(new_size);

            ClobberMemory();
        }
    }
}

template<class Vector>
void BM_resize_n_2(State& s) {
    Random::seed(1321312);

    int times = s.range(0);
    int size_limit = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());
        
        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(size_limit) + 1;
            v.resize2(new_size);

            ClobberMemory();
        }
    }
}

/*
 * benchmark resize val
 */
template<class Vector>
void BM_resize_n_val_1(State& s) {
    using T = typename Vector::value_type;
    Random::seed(1321312);

    int times = s.range(0);
    int size_limit = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());
        
        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(size_limit) + 1;
            v.resize1(new_size, get_value<T>(i));

            ClobberMemory();
        }
    }
}

template<class Vector>
void BM_resize_n_val_2(State& s) {
    using T = typename Vector::value_type;
    Random::seed(1321312);

    int times = s.range(0);
    int size_limit = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());
        
        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(size_limit) + 1;
            v.resize2(new_size, get_value<T>(i));

            ClobberMemory();
        }
    }
}

/*
 * benchmark at
 */
template<class Vector>
void BM_at_1(State& s) {
    Random::seed(12313);
    
    int size_limit = s.range(0);
    int initial_size = Random::rand(size_limit);

    Vector v(initial_size);

    DoNotOptimize(v.data());
    ClobberMemory();

    for (auto _ : s) {
        int idx = Random::rand(size_limit);
        try {
            auto& x = v.at1(idx);
            DoNotOptimize(x);
        }
        catch (...) {}
    }
}

template<class Vector>
void BM_at_2(State& s) {
    Random::seed(12313);
    
    int size_limit = s.range(0);
    int initial_size = Random::rand(size_limit);

    Vector v(initial_size);

    for (auto _ : s) {
        int idx = Random::rand(size_limit);
        try {
            auto& x = v.at2(idx);
            DoNotOptimize(x);
        }
        catch (...) {}
    }
}

/*
 * benchmark assign
 */
template<class Vector>
void BM_assign_1(State& s) {
    using T = typename Vector::value_type;
    Random::seed(1321312);

    int times = s.range(0);
    int size_limit = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());
        
        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(size_limit + 1);
            v.assign1(new_size, get_value<T>(i));

            ClobberMemory();
        }
    }
}

template<class Vector>
void BM_assign_2(State& s) {
    using T = typename Vector::value_type;
    Random::seed(1321312);

    int times = s.range(0);
    int size_limit = s.range(1);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());
        
        for (int i = 0; i < times; ++i) {
            int new_size = Random::rand(size_limit + 1);
            v.assign2(new_size, get_value<T>(i));

            ClobberMemory();
        }
    }
}

/*
 * benchmark safe_pop_back
 */
template<class Vector>
void BM_safe_pop_back_1(State& s) {
    Random::seed(312312);

    int size_limit = s.range(0);

    for (auto _ : s) {
        int size = Random::rand(size_limit + 1);
        Vector v(size);

        DoNotOptimize(v.data());

        while (v.safe_pop_back1())
            ClobberMemory();
    }
}

template<class Vector>
void BM_safe_pop_back_2(State& s) {
    Random::seed(312312);

    int size_limit = s.range(0);

    for (auto _ : s) {
        int size = Random::rand(size_limit + 1);
        Vector v(size);

        DoNotOptimize(v.data());

        while (v.safe_pop_back2())
            ClobberMemory();
    }
}

/*
 * benchmark insert
 */
template<class Vector>
void BM_insert_1(State& s) {
    using T = typename Vector::value_type;
    Random::seed(321321);
    
    int size_limit = s.range(0);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());

        int count = Random::rand(size_limit + 1);
        v.insert1(v.begin(), count, get_value<T>(0));

        ClobberMemory();
    }
}

template<class Vector>
void BM_insert_2(State& s) {
    using T = typename Vector::value_type;
    Random::seed(321321);
    
    int size_limit = s.range(0);

    for (auto _ : s) {
        Vector v;

        DoNotOptimize(v.data());

        int count = Random::rand(size_limit + 1);
        v.insert2(v.begin(), count, get_value<T>(0));

        ClobberMemory();
    }
}

/*
 * benchmark erase
 */
template<class Vector>
void BM_erase_1(State& s) {
    Random::seed(4324321);
    int size_limit = s.range(0);

    for (auto _ : s) {
        int size = Random::rand(size_limit + 1);
        Vector v(size);

        DoNotOptimize(v.data());
        
        int e_end = Random::rand(size + 1);
        int e_begin = Random::rand(e_end + 1);

        v.erase1(v.begin() + e_begin, v.begin() + e_end);

        ClobberMemory();
    }
}

template<class Vector>
void BM_erase_2(State& s) {
    Random::seed(4324321);
    int size_limit = s.range(0);

    for (auto _ : s) {
        int size = Random::rand(size_limit + 1);
        Vector v(size);

        DoNotOptimize(v.data());
        
        int e_end = Random::rand(size + 1);
        int e_begin = Random::rand(e_end + 1);

        v.erase2(v.begin() + e_begin, v.begin() + e_end);

        ClobberMemory();
    }
}

/*
 * benchmark emplace_back
 */
template<class Vector>
void BM_emplace_back_1(State& s) {
    using T = typename Vector::value_type;
    Random::seed(31231);
    
    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v.data());

        try {
            for (int i = 0;; ++i) {
                v.emplace_back1(get_value<T>(i));
                ClobberMemory();
            }
        }
        catch (...) { }
    }
}

template<class Vector>
void BM_emplace_back_2(State& s) {
    using T = typename Vector::value_type;
    Random::seed(31231);
    
    for (auto _ : s) {
        Vector v;
        DoNotOptimize(v.data());

        try {
            for (int i = 0;; ++i) {
                v.emplace_back2(get_value<T>(i));
                ClobberMemory();
            }
        }
        catch (...) { }
    }
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
        ->Args({CONCAT(varname, _ARG1), CONCAT(varname, _ARG2), })

#define REGISTER_BENCHMARK(func, unit, varname) \
    REGISTER_BENCHMARK_FOR_VECTOR(CONCAT(func, _1), unit, varname, uwr_static_vector); \
    REGISTER_BENCHMARK_FOR_VECTOR(CONCAT(func, _2), unit, varname, uwr_static_vector)

/*
 * register all benchmarks
 */
REGISTER_BENCHMARK(BM_resize_n,       kNanosecond,  RESIZE);
REGISTER_BENCHMARK(BM_resize_n_val,   kNanosecond,  RESIZE);
REGISTER_BENCHMARK(BM_at,             kNanosecond,  AT);
REGISTER_BENCHMARK(BM_assign,         kNanosecond,  ASSIGN);
REGISTER_BENCHMARK(BM_safe_pop_back,  kNanosecond,  SAFE_POP_BACK);
REGISTER_BENCHMARK(BM_insert,         kNanosecond,  INSERT);
REGISTER_BENCHMARK(BM_erase,          kNanosecond,  ERASE);
REGISTER_BENCHMARK(BM_emplace_back,   kNanosecond,  EMPLACE_BACK);

#endif // out-of-data

BENCHMARK_MAIN();
