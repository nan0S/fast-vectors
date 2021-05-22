#include <utils/utils.hpp>
#include <benchmark/benchmark.h>

template<template<class> class V, class... Ts>
class vector_bench_env {
public:
    vector_bench_env(int seed) :
        gen(seed) {}

    void run_simulation(int iters=1000) {
        for (int i = 0; i < iters; ++i) {
            (dispatch_action<Ts>(i), ...);
        }
    }

private:
    size_t get_length_sum() {
        return (get_typed_length_sum<Ts>() + ...);
    }

    template<class T>
    size_t get_typed_length_sum() {
        auto& typed_env = get_env_of_type<T>();
        size_t length_sum = 0;

        for (const auto& v : typed_env)
            length_sum += v.size();

        return length_sum;
    }

    template<class T>
    void dispatch_action(int i) {
        // TODO: maybe add probability distribution other than uniform
        switch (random(1, 7)) {
            case 1: construct_action<T>(i); break;
            case 2: destroy_action<T>(i); break;
            case 3: push_back_action<T>(i); break;
            case 4: pop_back_action<T>(i); break;
            case 5: copy_action<T>(i); break;
            case 6: insert_action<T>(i); break;
            case 7: erase_action<T>(i); break;
        }
    }

    template<class T>
    void construct_action(int i) {
        // std::cout << boost::format("construct_action(%d)\n") % i;

        auto& typed_env = get_env_of_type<T>();
        int q = random(1, 3);
        std::uniform_int_distribution<> size_dist(1, i + 10);

        while (q--)
            typed_env.emplace_back(size_dist(gen));
    }

    template<class T>
    void destroy_action(int i) {
        // std::cout << boost::format("destroy_action(%d)\n") % i;

        auto& typed_env = get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, std::min(3, typed_env_size));

        while (q--) {
            int pick = random(0, --typed_env_size);

            std::swap(typed_env[pick], typed_env.back());
            typed_env.pop_back();
        }
    }

    template<class T>
    void push_back_action(int i) {
        // std::cout << boost::format("push_back_action(%d)\n") % i;

        auto& typed_env = get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        // TODO: maybe sth more complex (?)
        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);
        size_type<T> max_size = i * 100 + 1;

        while (q--) {
            auto& picked = typed_env[pick_dist(gen)];
            auto can_put_in = picked.max_size() - picked.size();
            auto put_in = random<size_type<T>>(0, std::min(max_size, can_put_in));

            while (put_in--)
                picked.emplace_back();
        }
    }

    template<class T>
    void pop_back_action(int i) {
        // std::cout << boost::format("pop_back_action(%d)\n") % i;

        auto& typed_env = get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);

        while (q--) {
            auto& picked = typed_env[pick_dist(gen)];
            auto cnt = random<size_type<T>>(0, picked.size());

            while (cnt--)
                picked.pop_back();
        }
    }

    template<class T>
    void copy_action(int i) {
        // std::cout << boost::format("copy_action(%d)\n") % i;

        auto& typed_env = get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, 3);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);

        while (q--) {
            auto pick = pick_dist(gen);

            typed_env.emplace_back();
            typed_env.back() = typed_env[pick];
        }
    }

    template<class T>
    void insert_action(int i) {
        // std::cout << boost::format("insert_action(%d)\n") % i;

        auto& typed_env = get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);
        size_type<T> max_insert = i + 1;

        while (q--) {
            auto& picked = typed_env[pick_dist(gen)];
            auto can_insert = picked.max_size() - picked.size();
            auto cnt = random<size_type<T>>(0, std::min(can_insert, max_insert));
            auto pos = random<size_type<T>>(0, picked.size());

            picked.insert(picked.begin() + pos, cnt, T());
        }
    }

    template<class T>
    void erase_action(int i) {
        // std::cout << boost::format("erase_action(%d)\n") % i;

        auto& typed_env = get_env_of_type<T>();
        int typed_env_size = typed_env.size();
        if (typed_env_size == 0) {
            construct_action<T>(i);
            return;
        }

        int q = random(1, typed_env_size / 3 + 1);
        std::uniform_int_distribution<> pick_dist(0, typed_env_size - 1);

        while (q--) {
            auto& picked = typed_env[pick_dist(gen)];
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
        return std::uniform_int_distribution<T>{a, b}(gen);
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
    std::mt19937 gen;
    std::tuple<env_container<V<Ts>>...> env;
};

template<class Vector>
void BM_push_back(::benchmark::State& s) {
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
void BM_environment(::benchmark::State& s) {
    constexpr int seed = 12345512;
    int iters = s.range(0);
    int r = 0;

    for (auto _ : s) {
        vector_bench_env<V, Ts...> v_env(seed + r++);
        v_env.run_simulation(iters);
    }

    int id = s.range(1);
    s.counters["t" + std::to_string(id)];
}
