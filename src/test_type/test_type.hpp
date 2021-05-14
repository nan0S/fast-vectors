#pragma once

#include <iostream>

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
    friend std::ostream& operator<<(std::ostream& out, const test_type& o);

public:
    static int do_print;
    static int instances;

private:
    int val;
};
