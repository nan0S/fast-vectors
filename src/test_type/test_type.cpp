#include "test_type.hpp"
#include <iostream>

int test_type::do_print = false;
int test_type::instances = 0;

test_type::test_type() : a(0) {
    ++instances;
    if (do_print)
        std::cout << "()" << std::endl;
}

test_type::test_type(int a) : a(a) {
    ++instances;
    if (do_print)
        std::cout << "(int)" << std::endl;
}

test_type::test_type(const test_type& o) : a(o.a) {
    ++instances;
    if (do_print)
        std::cout << "(const&)" << std::endl;
}

test_type::test_type(test_type&& o) : a(o.a) {
    ++instances;
    if (do_print)
        std::cout << "(&&)" << std::endl;
}

test_type& test_type::operator=(const test_type& o) {
    if (do_print)
        std::cout << "op=(const&)" << std::endl;
    a = o.a;
    return *this;
}

test_type& test_type::operator=(test_type&& o) {
    if (do_print)
        std::cout << "op=(&&)" << std::endl;
    a = o.a;
    return *this;
}

test_type::~test_type() {
    --instances;
    if (do_print)
        std::cout << "~()" << std::endl;
}

test_type::operator int() {
    return a;
}

std::ostream& operator<<(std::ostream& out, const test_type& o) {
    return out << o.a;
}
