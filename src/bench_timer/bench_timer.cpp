#include "bench_timer.hpp"

#include <iostream>

std::map<std::string, bench_timer::delta_t>
	bench_timer::m_acc_records;

bench_timer::bench_timer(const std::string& name)
	: m_name(name), m_start(clock_t::now()) {}

bench_timer::~bench_timer() {
	clock_t::time_point now = clock_t::now();
	delta_t delta = std::chrono::duration<delta_t>(now - m_start).count();
	m_acc_records[m_name] += delta;
}

void bench_timer::print() {
	std::cout << "\n==== bench_timer::print() ====\n";
	for (const auto& [name, time] : m_acc_records) {
		std::cout << "\t" << name << ": " 
				  << time * 1000 << "ms\n";
	}
	std::cout << std::endl;
}

void bench_timer::reset() {
	m_acc_records.clear();
}