#include <boost/container/static_vector.hpp>
#include "static_rvector.hpp"
#include "static_vector/include/palotasb/static_vector.hpp"

#include <chrono>
#include <iostream>
#include <vector>

using T = int;
constexpr uint C = 50000;

template<typename Vector, uint TIMES=C>
void test(const std::string& name) {
	auto start = std::chrono::high_resolution_clock::now(); 

	uint sum = 0;
	for (uint i = 0; i < TIMES; ++i) {
		Vector v;
		for (uint i = 0; i < TIMES; ++i)
			v.emplace_back(i);
		for (auto x : v)
			sum += x;
	}
	std::cout << sum << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	auto delta = end - start;
	auto elapsed = std::chrono::duration<double>(end - start).count();
	std::cout << name << ": " << elapsed * 1000 << "ms" << std::endl;
}

int main() {
	test<boost::container::static_vector<T, C>>("boost::static_vector");
	// test<stlpb::static_vector<T, C>>("stlpb::static_vector");
	test<static_rvector<T, C>>("static_rvector");
	// test<std::vector<T>>("std::vector");
	// test<boost::container::vector<T>>("boost::vector");

	return 0;
}