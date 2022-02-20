#include "bench_timer.hpp"

#include <iostream>
#include <functional>
#include <fstream>

std::map<std::string, bench_timer::delta_t> bench_timer::m_acc_records;
std::map<int, bench_timer::delta_t> bench_timer::m_iters;
int bench_timer::m_time = 0;

bench_timer::bench_timer(const std::string& name)
   : m_name(name), m_start(clock_t::now()) {}

bench_timer::~bench_timer() {
   if (m_name != "special") {
      clock_t::time_point now = clock_t::now();
      delta_t delta = std::chrono::duration<delta_t>(now - m_start).count();
      m_acc_records[m_name] += delta;
   }
}

void bench_timer::save_epoch(int iter) {
   clock_t::time_point now = clock_t::now();
   delta_t delta = std::chrono::duration<delta_t>(now - m_start).count();
   m_iters[iter] += delta * 1000;
}

void bench_timer::print() {
   std::cout << "\n==== bench_timer::print() ====\n";
   for (const auto& [name, time] : m_acc_records) {
      std::cout << "\t" << name << ": " << time * 1000 << "ms\n";
   }
   std::cout << std::endl;
}

void bench_timer::print_epoch(const std::string& type, int vecs) {
   std::string t;
   std::transform(type.begin(), type.end(), std::back_inserter(t), tolower);
   std::string full_name = "vector_benchmark_" + t;
   if (t == "push_only")
      full_name += "_" + std::to_string(vecs);
   std::ofstream fs("./logs/" + full_name + ".csv",
                    m_time == 0 ? std::ios_base::trunc : std::ios_base::app);
   if (m_time == 0) {
      std::cout << "id;iter;time\n";
      fs << "id;iter;time\n";
   }
   for (const auto& [iter, time] : m_iters) {
      std::cout << m_time << ";" << iter << ";" << time << "\n"; 
      fs << m_time << ";" << iter << ";" << time << "\n";
   }
   ++m_time;
}

void bench_timer::reset() {
   m_acc_records.clear();
   m_iters.clear();
}
