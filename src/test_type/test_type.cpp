#include "test_type.hpp"
#include <iostream>

int test_type::do_print = false;
int test_type::instances = 0;

test_type::test_type() : val(0) {
    ++instances;
    if (do_print)
        std::cout << "()" << std::endl;
}

test_type::test_type(int val) : val(val) {
    ++instances;
    if (do_print)
        std::cout << "(int)" << std::endl;
}

test_type::test_type(const test_type& o) : val(o.val) {
    ++instances;
    if (do_print)
        std::cout << "(const&)" << std::endl;
}

test_type::test_type(test_type&& o) : val(o.val) {
    ++instances;
    if (do_print)
        std::cout << "(&&)" << std::endl;
}

test_type& test_type::operator=(const test_type& o) {
    if (do_print)
        std::cout << "op=(const&)" << std::endl;
    val = o.val;
    return *this;
}

test_type& test_type::operator=(test_type&& o) {
    if (do_print)
        std::cout << "op=(&&)" << std::endl;
    val = o.val;
    return *this;
}

test_type::~test_type() {
    --instances;
    if (do_print)
        std::cout << "~()" << std::endl;
}

test_type::operator int() {
    return val;
}

bool operator==(const test_type& x, const test_type& y) {
    return x.val == y.val;
}

std::ostream& operator<<(std::ostream& out, const test_type& o) {
    return out << o.val;
}
