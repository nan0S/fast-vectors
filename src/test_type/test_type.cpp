#include "test_type.hpp"

#include <iostream>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <cstring>

bool test_type::do_print = false;
long test_type::instances = 0;
test_type::record_t test_type::record;
std::deque<test_type::record_t> test_type::records;

test_type::test_type() : val(0), moved(false) {
    ++record.def_cons;
    ++instances;

    if (do_print)
        std::cout << "()" << std::endl;
}

test_type::test_type(int val) : val(val), moved(false) {
    ++record.val_cons;
    ++instances;

    if (do_print)
        std::cout << "(int)" << std::endl;
}

test_type::test_type(const test_type& o) : val(o.val), moved(o.moved) {
    ++record.copy_cons;
    ++instances;

    if (do_print)
        std::cout << "(const&)" << std::endl;
}

test_type::test_type(test_type&& o) : val(o.val), moved(o.moved) {
    ++record.move_cons;
    ++instances;
    o.moved = true;

    if (do_print)
        std::cout << "(&&)" << std::endl;
}

test_type& test_type::operator=(const test_type& o) {
    ++record.copy_op;
    val = o.val;
    moved = o.moved;

    if (do_print)
        std::cout << "op=(const&)" << std::endl;

    return *this;
}

test_type& test_type::operator=(test_type&& o) {
    ++record.move_op;
    val = o.val;
    moved = o.moved;
    o.moved = true;

    if (do_print)
        std::cout << "op=(&&)" << std::endl;

    return *this;
}

test_type::~test_type() {
    ++record.dest;
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

bool operator<=(const test_type::record_t& x, const test_type::record_t& y) {
    int sx = std::accumulate(x.ops, x.ops + x.ops_count, 0);
    int sy = std::accumulate(y.ops, y.ops + y.ops_count, 0);
    
    if (sx != sy)
        return sx < sy;

    return !std::lexicographical_compare(y.ops, y.ops + y.ops_count,
                                         x.ops, x.ops + x.ops_count);
}

void test_type::start_recording() {
    std::memset(&record, 0, sizeof(record_t));
}

void test_type::stop_recording() {
    records.push_back(record);
}

bool test_type::is_current_not_better() {
    assert(!records.empty());

    record_t compare = records.front();
    records.pop_front();

    return compare <= record;
}

void test_type::print_stats() {
    std::cout << "\n==== test_type::print_stats() ====\n";

    std::cout << "()          = " << record.def_cons << "\n";
    std::cout << "(int)       = " << record.val_cons << "\n";
    std::cout << "(const&)    = " << record.copy_cons << "\n";
    std::cout << "(&&)        = " << record.move_cons << "\n";
    std::cout << "op=(const&) = " << record.copy_op << "\n";
    std::cout << "op=(&&)     = " << record.move_op << "\n";
    std::cout << "Total ~()   = " << record.dest << "\n";

    long total_cons = record.def_cons + record.val_cons +
        record.copy_cons + record.move_cons;
    long total_op = record.copy_op + record.move_op;

    std::cout << "Total ()    = " << total_cons << "\n";
    std::cout << "Total op    = " << total_op << "\n";
    std::cout << "Total calls = " << total_cons + total_op + record.dest << "\n";

    std::cout << std::endl;
}
