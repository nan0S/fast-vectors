#include "test_type.hpp"

#include <iostream>
#include <cassert>
#include <numeric>
#include <algorithm>

int test_type::do_print = false;
int test_type::instances = 0;
int test_type::ops[2][test_type::ops_count] = {};
int test_type::ops_idx = 0;

test_type::test_type() : val(0), moved(false) {
    ++ops[ops_idx][CONSTRUCTOR];
    ++instances;
    if (do_print)
        std::cout << "()" << std::endl;
}

test_type::test_type(int val) : val(val), moved(false) {
    ++ops[ops_idx][VALUE_CONSTRUCTOR];
    ++instances;
    if (do_print)
        std::cout << "(int)" << std::endl;
}

test_type::test_type(const test_type& o) : val(o.val), moved(o.moved) {
    ++ops[ops_idx][COPY_CONSTRUCTOR];
    ++instances;
    if (do_print)
        std::cout << "(const&)" << std::endl;
}

test_type::test_type(test_type&& o) : val(o.val), moved(o.moved) {
    ++ops[ops_idx][MOVE_CONSTRUCTOR];
    ++instances;
    o.moved = true;
    if (do_print)
        std::cout << "(&&)" << std::endl;
}

test_type& test_type::operator=(const test_type& o) {
    ++ops[ops_idx][COPY_OPERATOR];
    val = o.val;
    moved = o.moved;
    if (do_print)
        std::cout << "op=(const&)" << std::endl;

    return *this;
}

test_type& test_type::operator=(test_type&& o) {
    ++ops[ops_idx][MOVE_OPERATOR];
    val = o.val;
    moved = o.moved;
    o.moved = true;
    if (do_print)
        std::cout << "op=(&&)" << std::endl;

    return *this;
}

test_type::~test_type() {
    ++ops[ops_idx][DESTRUCTOR];
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

void test_type::save_ops() {
    assert(ops_idx == 0);
    std::copy_n(ops[0], ops_count, ops[1]);
    std::fill_n(ops[0], ops_count, 0);
}

bool test_type::are_ops_better() {
    int s1 = std::accumulate(ops[0], ops[0] + ops_count, 0);
    int s2 = std::accumulate(ops[1], ops[1] + ops_count, 0);

    if (s1 != s2)
        return s1 < s2;

    return !std::lexicographical_compare(ops[1], ops[1] + ops_count,
                                         ops[0], ops[0] + ops_count);
}
