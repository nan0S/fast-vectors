#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>
#include <benchmark/benchmark.h>

using T = int;
constexpr uint C = 50000;
using uwr_static_vector = uwr::static_vector<T, C>;
using boost_static_vector = boost::container::static_vector<T, C>;

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

    s.counters["t1"] = 1;
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

    s.counters["t2"] = 1;
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

    s.counters["t3"] = 1;
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

    s.counters["t4"] = 1;
}

static constexpr int PUSH_BACK_TIMES = 10000;
BENCHMARK_TEMPLATE(BM_push_back, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(PUSH_BACK_TIMES);
BENCHMARK_TEMPLATE(BM_push_back, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(PUSH_BACK_TIMES);

static constexpr int PUSH_BACK_POP_BACK_TIMES = 10000;
BENCHMARK_TEMPLATE(BM_push_back_pop_back, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(PUSH_BACK_POP_BACK_TIMES);
BENCHMARK_TEMPLATE(BM_push_back_pop_back, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(PUSH_BACK_POP_BACK_TIMES);

static constexpr int SWAP_TIMES = 3000;
BENCHMARK_TEMPLATE(BM_swap, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(SWAP_TIMES);
BENCHMARK_TEMPLATE(BM_swap, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(SWAP_TIMES);

static constexpr int RESIZE_TIMES = 3000000;
BENCHMARK_TEMPLATE(BM_resize, boost_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(RESIZE_TIMES);
BENCHMARK_TEMPLATE(BM_resize, uwr_static_vector)
    ->Unit(::benchmark::kMillisecond)
    ->Arg(RESIZE_TIMES);

BENCHMARK_MAIN();

// int main() {
    // info("simple benchmark (static_vector):", 0);

    // #if 1
    // constexpr int TEST1_TIMES = 50000;
    // info("push_back:", 1);
    // test1<boost::container::static_vector<T, C>, TEST1_TIMES>("boost::static_vector");
    // test1<uwr::static_vector<T, C>, TEST1_TIMES>("uwr::static_vector");

    // constexpr int TEST2_TIMES = C;
    // info("push_back/pop_back interlaced:", 1);
    // test2<boost::container::static_vector<T, C>, TEST2_TIMES>("boost::static_vector");
    // test2<uwr::static_vector<T, C>, TEST2_TIMES>("uwr::static_vector");

    // constexpr int TEST3_TIMES = 10000;
    // info("two vectors swap:", 1);
    // test3<boost::container::static_vector<T, C>, TEST3_TIMES>("boost::static_vector");
    // test3<uwr::static_vector<T, C>, TEST3_TIMES>("uwr::static_vector");

    // constexpr int TEST4_TIMES = 10000000;
    // info("resize:", 1);
    // test4<boost::container::static_vector<T, C>, TEST4_TIMES>("boost::static_vector");
    // test4<uwr::static_vector<T, C>, TEST4_TIMES>("uwr::static_vector");
    // #endif

    // return 0;
// }
