#include "test_type.hpp"
#include <iostream>

int test_type::do_print = false;
int test_type::instances = 0;

test_type::test_type() : val(0), moved(false) {
    ++instances;
    if (do_print)
        std::cout << "()" << std::endl;
}

test_type::test_type(int val) : val(val), moved(false) {
    ++instances;
    if (do_print)
        std::cout << "(int)" << std::endl;
}

test_type::test_type(const test_type& o) : val(o.val), moved(o.moved) {
    ++instances;
    if (do_print)
        std::cout << "(const&)" << std::endl;
}

test_type::test_type(test_type&& o) : val(o.val), moved(o.moved) {
    ++instances;
    o.moved = true;
    if (do_print)
        std::cout << "(&&)" << std::endl;
}

test_type& test_type::operator=(const test_type& o) {
    val = o.val;
    moved = o.moved;
    if (do_print)
        std::cout << "op=(const&)" << std::endl;

    return *this;
}

test_type& test_type::operator=(test_type&& o) {
    val = o.val;
    moved = o.moved;
    o.moved = true;
    if (do_print)
        std::cout << "op=(&&)" << std::endl;

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
    if (x.moved || y.moved)
        return false;
    return x.val == y.val;
}

bool operator!=(const test_type& x, const test_type& y) {
    if (x.moved || y.moved)
        return true;
    return x.val != y.val;
}

bool operator<(const test_type& x, const test_type& y) {
    if (x.moved || y.moved)
        return false;
    return x.val < y.val;
}

bool operator<=(const test_type& x, const test_type& y) {
    if (x.moved || y.moved)
        return false;
    return x.val <= y.val;
}

bool operator>(const test_type& x, const test_type& y) {
    if (x.moved || y.moved)
        return false;
    return x.val > y.val;
}

bool operator>=(const test_type& x, const test_type& y) {
    if (x.moved || y.moved)
        return false;
    return x.val >= y.val;
}

std::ostream& operator<<(std::ostream& out, const test_type& o) {
    return out << o.val;
}
