#pragma once

#include <iostream>

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
	friend std::ostream& operator<<(std::ostream& out, const test_type& o);

	static int do_print;

private:
	int a;
};