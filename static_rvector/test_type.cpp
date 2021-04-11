#include "test_type.hpp"
#include <iostream>

test_type::test_type() : a(0) { 
	std::cout << "()" << std::endl;
}

test_type::test_type(int a) : a(a) {
	std::cout << "(int)" << std::endl;
}

test_type::test_type(const test_type& o) : a(o.a) {
	std::cout << "(const&)" << std::endl;
}

test_type::test_type(test_type&& o) : a(o.a) {
	std::cout << "(&&)" << std::endl;
}

test_type& test_type::operator=(const test_type& o) {
	std::cout << "op=(const&)" << std::endl;
	a = o.a;
	return *this;
}

test_type& test_type::operator=(test_type&& o) {
	std::cout << "op=(&&)" << std::endl;
	a = o.a;
	return *this;
}

test_type::~test_type() {
	std::cout << "~()" << std::endl;
}

test_type::operator int() {
	return a;
}

std::ostream& operator<<(std::ostream& out, const test_type& o) {
	return out << o.a;
}