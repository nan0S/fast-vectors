#pragma once

#include <iostream>
#include <deque>
#include <common/define.hpp>

#define ON test_type::do_print = true
#define OFF test_type::do_print = false

class test_type {
public:
    test_type();
    test_type(int a);
    test_type(const test_type&);
    test_type(test_type&&);
    test_type& operator=(const test_type&);
    test_type& operator=(test_type&&);
    ~test_type();

    operator int();

    friend bool operator==(const test_type& x, const test_type& y);
    friend bool operator!=(const test_type& x, const test_type& y);
    friend bool operator<(const test_type& x, const test_type& y);
    friend bool operator<=(const test_type& x, const test_type& y);
    friend bool operator>(const test_type& x, const test_type& y);
    friend bool operator>=(const test_type& x, const test_type& y);
#if CPP_ABOVE_17
    auto operator<=>(const test_type& y) const = default;
#endif

    friend std::ostream& operator<<(std::ostream& out, const test_type& o);

public:
    static bool do_print;
    static long instances;

    struct record_t {
        static constexpr int ops_count = 7;

        union {
            struct {
                long copy_cons, copy_op;
                long move_cons, move_op;
                long val_cons, def_cons;
                long dest;
            };
            long ops[ops_count];
        };

        friend bool operator<=(const record_t& x, const record_t& y);
    };

    static std::deque<record_t> records;
    static record_t record;
    
    static void start_recording();
    static void stop_recording();
    static bool is_current_not_better();
    static void print_stats();

private:
    int val;
    bool moved;
};
