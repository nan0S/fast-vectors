#pragma once

#include <string>
#include <chrono>
#include <map>

class bench_timer {
public:
	using clock_t = std::chrono::high_resolution_clock;
	using delta_t = double;

	bench_timer(const std::string& name);
	~bench_timer();

	void save_epoch(int iter);

	static void print();
	static void print_epoch(const std::string& type, int vecs);
	static void reset();

private:
	std::string m_name;
	clock_t::time_point m_start;

private:
	static std::map<std::string, delta_t> m_acc_records;
	static std::map<int, delta_t> m_iters;
    static int m_time;
};
