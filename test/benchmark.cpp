#include <bits/stdc++.h>

#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>

template<class Vector>
void push_back_benchmark(std::string test_name, int max_iters=100000, int repeat=10) {
	auto begin = std::chrono::high_resolution_clock::now();

	int iters = max_iters;
	while (iters > 1000)
		iters /= 10;

	for (; iters < max_iters; iters *= 10)
		for (int t = 0; t < repeat; ++t) {
			Vector v;
			for (int i = 0; i < iters; ++i)
				v.emplace_back();
		}

	auto end = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration<double>(end - begin).count();

	std::cout << test_name << ": " << delta * 1000 << "ms" << std::endl;
}

template<class T, uint C>
using ustatic_vector = uwr::static_vector<T, C>;
template<class T, uint C>
using bstatic_vector = boost::container::static_vector<T, C>;

static constexpr uint C = 50000;

int main() {
	push_back_benchmark<ustatic_vector<int, C>>(
		"uwr::static_vector", C, 1000);
	push_back_benchmark<bstatic_vector<int, C>>(
		"boost::static_vector", C, 1000);

	return 0;
}
