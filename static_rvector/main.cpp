#include "static_rvector.hpp"
// #include "static_vector/include/palotasb/static_vector.hpp"
#include <boost/container/static_vector.hpp>
#include "test_type.hpp"

#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

#define ON test_type::do_print = true
#define OFF test_type::do_print = false

template<typename T, uint C>
using rvector = static_rvector<T, C>;

template<typename T, std::size_t C>
using bvector = boost::container::static_vector<T, C>;

using rtvector = rvector<test_type, 5>;
using btvector = bvector<test_type, 5>;

using T = int;
constexpr uint C = 50000;

void measure(std::function<std::string()> f) {
	auto start = std::chrono::high_resolution_clock::now(); 
	auto name = f();
	auto end = std::chrono::high_resolution_clock::now();
	auto delta = end - start;
	auto elapsed = std::chrono::duration<double>(end - start).count();
	std::cout << name << ": " << elapsed * 1000 << "ms" << std::endl;
}

template<typename Vector, uint TIMES=C>
void test1(const std::string& name) {
	auto f = [&name] {
		size_t sum = 0;
		for (uint i = 0; i < TIMES; ++i) {
			Vector v;
			for (uint j = 0; j < TIMES; ++j)
				v.push_back(j);
			for (auto x : v)
				sum += x;
		}
		std::cout << sum << std::endl;
		return name;
	};

	measure(f);
}

template<typename Vector, uint TIMES=C>
void test2(const std::string& name) {
	auto f = [&name] {
		for (uint i = 0; i < TIMES; ++i) {
			Vector v;
			int times = TIMES;
			while (times) {
				int c = rand() % times + 1;
				times -= c;
				if (c > v.size() || (v.size() + c <= v.capacity() && rand() % 2 == 0))
					for (int j = 0; j < c; ++j)
						v.push_back(j);
				else
					while (c--)
						v.pop_back();
			}
		}
		return name;
	};

	measure(f);
}

template<typename Vector, uint TIMES=C>
void test3(const std::string& name) {
	auto f = [&name] {
		for (uint i = 0; i < TIMES; ++i) {
			Vector v1({1, 2, 3, 4, 5, 6, 7});
			Vector v2({1, 2, 3});
			int times = TIMES + rand() % 2;
			for (int j = 0; j < times; ++j)
				v1.swap(v2);
		}
		return name;
	};

	measure(f);
}

template<typename Vector, uint TIMES=C>
void test4(const std::string& name) {
	auto f = [&name] {
		for (uint i = 0; i < TIMES; ++i) {
			int s = rand() % 10 + 5;
			Vector v;
			v.resize(s);
			v.resize(rand() % (s - 3));
		}
		return name;
	};

	measure(f);
}


int main() {
	test1<boost::container::static_vector<T, C>>("boost::static_vector");
	// test1<stlpb::static_vector<T, C>>("stlpb::static_vector");
	test1<static_rvector<T, C>>("static_rvector");

	// test2<boost::container::static_vector<T, C>>("boost::static_vector");
	// test2<stlpb::static_vector<T, C>>("stlpb::static_vector");
	// test2<static_rvector<T, C>>("static_rvector");
	
	// test3<boost::container::static_vector<T, C>>("boost::static_vector");
	// test3<stlpb::static_vector<T, C>>("stlpb::static_vector");
	// test3<static_rvector<T, C>>("static_rvector");

	// test4<boost::container::static_vector<T, C>>("boost::static_vector");
	// test4<stlpb::static_vector<T, C>>("stlpb::static_vector");
	// test4<static_rvector<T, C>>("static_rvector");

	return 0;
}
