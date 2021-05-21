#pragma once

#include <iostream>

#define CPP_ABOVE_17 __cplusplus > 201703L

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
    static int do_print;
    static int instances;
    static constexpr int ops_count = 7;
    static int ops[2][ops_count];
    static int ops_idx;

    static void save_ops();
    static bool are_ops_better();

    enum PRIO {
        COPY_CONSTRUCTOR = 0,
        COPY_OPERATOR,
        MOVE_CONSTRUCTOR,
        MOVE_OPERATOR,
        DESTRUCTOR,
        VALUE_CONSTRUCTOR,
        CONSTRUCTOR
    };

private:
    int val;
    bool moved;
};
