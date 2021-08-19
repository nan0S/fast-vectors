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

	static void print();
	static void reset();

private:
	std::string m_name;
	clock_t::time_point m_start;

private:
	static std::map<std::string, delta_t> m_acc_records;
};