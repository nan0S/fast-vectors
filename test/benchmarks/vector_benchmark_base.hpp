#include <utils/utils.hpp>
#include <benchmark/benchmark.h>
#include <test_type/test_type.hpp>
#include <boost/format.hpp>
#include <bench_timer/bench_timer.hpp>
#include <common/identifiers.hpp>

using namespace benchmark;

enum bench_type : int {
    NONE = 0,
    PUSH_ONLY = 1,
    PUSH_CONS_DEST = 2,
    ALL = 4,
};

namespace std {
    std::string to_string(bench_type type) {
        switch (type) {
            case NONE: return "NONE";
            case PUSH_ONLY: return "PUSH_ONLY";
            case PUSH_CONS_DEST: return "PUSH_CONS_DEST";
            case ALL: return "ALL";
            default: return "UNKNOWN";
        }
    }
}

template<template<class> class V, class... Ts>
class vector_bench_env {
public:
    vector_bench_env(bench_type type, int seed, int verbose)
        : type(type), gen(seed), verbose(verbose) {}

    void run_simulation(int iters=1000) {
        this->iters = iters;
        for (int i = 0; i < iters; ++i) {
            (dispatch_action<Ts>(i), ...);
        }
        if (verbose > 0)
            print_stats();
    }

private:
    void print_stats() {
        size_t total_vectors = get_total_number_of_vectors();
        size_t total_size_sum = get_vector_size_sum();
        size_t min_vector_size = get_min_vector_size();
        size_t max_vector_size = get_max_vector_size();
        size_t avg_vector_size = total_size_sum / total_vectors;

        std::cout
            << "total number of vectors = "
            << total_vectors << "\n"
            << "vector size sum         = "
            << total_size_sum << "\n"
            << "avg vector size         = "
            << avg_vector_size << "\n"
            << "min vector size         = "
            << min_vector_size << "\n"
            << "max vector size         = "
            << max_vector_size << "\n";
    }

    size_t get_total_number_of_vectors() {
        return (get_env_of_type<Ts>().size() + ...);
    }

    size_t get_vector_size_sum() {
        return (get_typed_vector_size_sum<Ts>() + ...);
    }

    template<class T>
    size_t get_typed_vector_size_sum() {
        auto& typed_env = this->get_env_of_type<T>();
        size_t length_sum = 0;

        for (const auto& v : typed_env)
            length_sum += v.size();

        return length_sum;
    }

    size_t get_min_vector_size() {
        return std::min({(get_typed_min_vector_size<Ts>(), ...)});
    }

    template<class T>
    size_t get_typed_min_vector_size() {
        auto& typed_env = this->get_env_of_type<T>();
        size_t min_size = std::numeric_limits<size_t>::max();

        for (const auto& v : typed_env)
            if (v.size() < min_size)
                min_size = v.size();

        return min_size;
    }

    size_t get_max_vector_size() {
        return std::min({(get_typed_max_vector_size<Ts>(), ...)});
    }

    template<class T>
    size_t get_typed_max_vector_size() {
        auto& typed_env = this->get_env_of_type<T>();
        size_t max_size = std::numeric_limits<size_t>::min();

        for (const auto& v : typed_env)
            if (v.size() > max_size)
                max_size = v.size();

        return max_size;
    }

    template<class T>
    void dispatch_action(int i) {
        // TODO: maybe add probability distribution other than uniform
        int chosen;
        switch (this->type) {
            case PUSH_ONLY:
                chosen = 3;
                break;
            case PUSH_CONS_DEST:
                chosen = random(1, 3);
                break;
            case ALL:
                chosen = random(1, 7);
                break;
            default:
                chosen = -1;
                break;
        }
        
        switch (chosen) {
            case 1: construct_action<T>(i); break;
            case 2: destroy_action<T>(i); break;
            case 3: push_back_action<T>(i); break;
            case 4: pop_back_action<T>(i); break;
            case 5: copy_action<T>(i); break;
            case 6: insert_action<T>(i); break;
            case 7: erase_action<T>(i); break;
            default: break;
        }
    }

    template<class T>
    void construct_action(int i) {
        if (verbose > 4)
            std::cout << boost::format("construct_action(%d)\n") % i;

        auto& typed_env = this->get_env_of_type<T>();
        int q = random(1, 3);
        std::uniform_int_distribution<> size_dist(1, i + 10);
        bench_timer timer("construct");

        while (q--) {
            typed_env.emplace_back(size_dist(this->gen));
        }
    }

    template<class T>
    void destroy_action(int i) {
        if (verbose > 4)
            std::cout << boost::format("destroy_action(%d)\n") % i;

        auto& typed_env = this->get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, std::min(3, typed_env_size));
        bench_timer timer("destroy");

        while (q--) {
            int pick = random(0, --typed_env_size);

            std::swap(typed_env[pick], typed_env.back());
            typed_env.pop_back();
        }
    }

    template<class T>
    void push_back_action(int i) {
        if (verbose > 4)
            std::cout << boost::format("push_back_action(%d)\n") % i;

        auto& typed_env = this->get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        // TODO: maybe sth more complex (?)
        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);
        size_type<T> max_size = i * 100 + 1;
        bench_timer timer("push_back");

        while (q--) {
            auto& picked = typed_env[pick_dist(this->gen)];
            auto can_put_in = picked.max_size() - picked.size();
            auto put_in = random<size_type<T>>(0, std::min(max_size, can_put_in));

            while (put_in--) {
                picked.emplace_back();
            }
        }
    }

    template<class T>
    void pop_back_action(int i) {
        if (verbose > 4)
            std::cout << boost::format("pop_back_action(%d)\n") % i;

        auto& typed_env = this->get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);
        bench_timer timer("pop_back");

        while (q--) {
            auto& picked = typed_env[pick_dist(this->gen)];
            auto cnt = random<size_type<T>>(0, picked.size());

            while (cnt--) {
                picked.pop_back();
            }
        }
    }

    template<class T>
    void copy_action(int i) {
        if (verbose > 4)
            std::cout << boost::format("copy_action(%d)\n") % i;

        auto& typed_env = this->get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, 3);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);
        bench_timer timer("copy");

        while (q--) {
            auto pick = pick_dist(this->gen);

            typed_env.emplace_back();
            typed_env.back() = typed_env[pick];
        }
    }

    template<class T>
    void insert_action(int i) {
        if (verbose > 4)
            std::cout << boost::format("insert_action(%d)\n") % i;

        auto& typed_env = this->get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);
        size_type<T> max_insert = i + 1;
        bench_timer timer("insert");

        while (q--) {
            auto& picked = typed_env[pick_dist(this->gen)];
            auto can_insert = picked.max_size() - picked.size();
            auto cnt = random<size_type<T>>(0, std::min(can_insert, max_insert));
            auto pos = random<size_type<T>>(0, picked.size());

            picked.insert(picked.begin() + pos, cnt, T());
        }
    }

    template<class T>
    void erase_action(int i) {
        if (verbose > 4)
            std::cout << boost::format("erase_action(%d)\n") % i;

        auto& typed_env = this->get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);
        bench_timer timer("erase");

        while (q--) {
            auto& picked = typed_env[pick_dist(this->gen)];
            auto picked_size = picked.size();
            if (picked_size == 0)
                continue;

            auto end = random<decltype(picked_size)>(1, picked_size);
            auto begin = random<decltype(picked_size)>(0, end - 1);

            picked.erase(picked.begin() + begin, picked.begin() + end);
        }
    }

    template<class T>
    T random(T a, T b) {
        return std::uniform_int_distribution<T>{a, b}(this->gen);
    }

    template<class T>
    inline auto& get_env_of_type() {
        return std::get<env_type<T>>(env);
    }

private:
    template<class Vec>
    using env_container = std::vector<Vec>;

    template<class T>
    using env_type = env_container<V<T>>;

    template<class T>
    using size_type = typename V<T>::size_type;

private:
    bench_type type;
    std::mt19937 gen;
    int verbose;
    int iters;
    std::tuple<env_container<V<Ts>>...> env;
};

template<class Vector>
void BM_push_back(State& s) {
    using T = typename Vector::value_type;
    int max_iters = s.range(0);
    int iters = s.range(0);

    while (iters > 1000)
        iters /= 10;

    for (auto _ : s)
        for (; iters < max_iters; iters *= 10) {
            Vector v;
            for (int i = 0; i < iters; ++i)
                v.push_back(T());
        }

    int id = s.range(1);
    s.counters["t" + std::to_string(id)];
}

template<template<class> class V, class... Ts>
void BM_environment(State& s, bench_type type, int verbose) {
    constexpr int seed = 12345512;
    int iters = s.range(0);
    int r = 0;

    constexpr bool is_test_type = (std::is_same_v<Ts, test_type> | ...);
    if constexpr (is_test_type)
        if (verbose > 3)
            test_type::start_recording();

    for (auto _ : s) {
        vector_bench_env<V, Ts...> v_env(
                type, seed + r++, verbose);
        DoNotOptimize(v_env);

        v_env.run_simulation(iters);
        
        ClobberMemory();
    }

    /* test_type printing */
    if constexpr (is_test_type)
        if (verbose > 3)
            test_type::print_stats();

    /* bench timer stats printing */
    if (verbose > 2) {
        bench_timer::print();
        bench_timer::reset();
    }

    /* uwr vector allocator statistics */
    #ifdef UWR_TRACK
    if constexpr (uwr::is_uwr_vector<V<int>>::value)
        if (verbose > 1) {
            uwr::mem::counters::print();
            uwr::mem::counters::reset();
        }
    #endif
    /* rvector allocator statistics */
    #ifdef RVECTOR_TRACK
    if constexpr (uwr::is_rvector<V<int>>::value)
        if (verbose > 1) {
            mm::counters::print();
            mm::counters::reset();
        }
    #endif

    int id = s.range(1);
    s.counters["t" + std::to_string(id)];
}
