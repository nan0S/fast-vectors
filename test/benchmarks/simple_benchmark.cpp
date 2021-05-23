#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <utils/utils.hpp>
#include <test_type/test_type.hpp>
#include <benchmark/benchmark.h>

using T = int;
constexpr uint C = 50000;

using uwr_static_vector = uwr::static_vector<T, C>;
using boost_static_vector = boost::container::static_vector<T, C>;

static constexpr int PUSH_BACK_TIMES = 30000;
#define PUSH_BACK_ITERS -1
static constexpr int PUSH_BACK_POP_BACK_TIMES = 10000;
#define PUSH_BACK_POP_BACK_ITERS -1
static constexpr int SWAP_TIMES = 3000;
#define SWAP_ITERS -1
static constexpr int RESIZE_TIMES = 3000000;
#define RESIZE_ITERS -1

template<class Vector>
void BM_push_back(::benchmark::State& s) {
    int times = s.range(0);

    for (auto _ : s) {
        T sum = 0;
        ::benchmark::DoNotOptimize(sum);

        for (int i = 0; i < times; ++i) {
            Vector v;
            ::benchmark::DoNotOptimize(v);

            for (int j = 0; j < times; ++j)
                v.push_back(T());
            for (const auto& x : v)
                sum += x;
        }
    }

    s.counters["t1"];
}

template<class Vector>
void BM_push_back_pop_back(::benchmark::State& s) {
    using size_type = typename Vector::size_type;
    size_type times = s.range(0);

    for (auto _ : s)
        for (size_type i = 0; i < times; ++i) {
            Vector v;
            size_type iters = times;
            while (iters) {
                size_type c = rand() % iters + 1;
                iters -= c;
                if (c > v.size() || (v.size() + c <= v.capacity() && rand() % 2 == 0))
                    for (size_type j = 0; j < c; ++j)
                        v.push_back(j);
                else
                    while (c--)
                        v.pop_back();
            }
        }

    s.counters["t2"];
}

template<class Vector>
void BM_swap(::benchmark::State& s) {
    int times = s.range(0);

    for (auto _ : s)
        for (int i = 0; i < times; ++i) {
            Vector v1({1, 2, 3, 4, 5, 6, 7});
            Vector v2({1, 2, 3});
            int many = times + rand() % 2;
            for (int j = 0; j < many; ++j)
                v1.swap(v2);
        }

    s.counters["t3"];
}

template<class Vector>
void BM_resize(::benchmark::State& s) {
    int times = s.range(0);

    for (auto _ : s)
        for (int i = 0; i < times; ++i) {
            int s = rand() % 10 + 5;
            Vector v;
            v.resize(s);
            v.resize(rand() % (s - 3));
        }

    s.counters["t4"];
}

BENCHMARK_TEMPLATE(BM_push_back, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (PUSH_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_TIMES);
BENCHMARK_TEMPLATE(BM_push_back, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (PUSH_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_TIMES);

BENCHMARK_TEMPLATE(BM_push_back_pop_back, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (PUSH_BACK_POP_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_POP_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_POP_BACK_TIMES);
BENCHMARK_TEMPLATE(BM_push_back_pop_back, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (PUSH_BACK_POP_BACK_ITERS != -1)
    ->Iterations(PUSH_BACK_POP_BACK_ITERS)
#endif
    ->Arg(PUSH_BACK_POP_BACK_TIMES);

BENCHMARK_TEMPLATE(BM_swap, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (SWAP_ITERS != -1)
    ->Iterations(SWAP_ITERS)
#endif
    ->Arg(SWAP_TIMES);
BENCHMARK_TEMPLATE(BM_swap, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (SWAP_ITERS != -1)
    ->Iterations(SWAP_ITERS)
#endif
    ->Arg(SWAP_TIMES);

BENCHMARK_TEMPLATE(BM_resize, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (RESIZE_ITERS != -1)
    ->Iterations(RESIZE_ITERS)
#endif
    ->Arg(RESIZE_TIMES);
BENCHMARK_TEMPLATE(BM_resize, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->MinTime(0.5)
#if (RESIZE_ITERS != -1)
    ->Iterations(RESIZE_ITERS)
#endif
    ->Arg(RESIZE_TIMES);

BENCHMARK_MAIN();
