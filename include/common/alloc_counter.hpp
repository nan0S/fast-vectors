#pragma once

#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

using namespace boost::accumulators;

namespace uwr::mem {

struct alloc_counter {
    alloc_counter(std::string name) : name(name) {}

    void print() {
        std::cout << "\n==== " << name << "::print() ====\n"
                  << "total mremaps    = " << count(mremaps) << "\n"
                  << "success mremaps  = " << sum(mremaps) << "\n"
                  << "failed mremaps   = " << count(mremaps) - sum(mremaps) << "\n"
                  << "objects mremaped = " << sum(objects) << "\n"
                  << "mean success     = " << mean(mremaps) << "\n"
                  << "mean grow        = " << mean(grows) << "\n"
                  << std::endl;
    }

    void reset() {
        mremaps = {};
        grows = {};
    }

    std::string name;

    accumulator_set<int,
        features<tag::count,
                 tag::sum,
                 tag::mean>> mremaps;
    accumulator_set<double,
        features<tag::mean>> grows;
    accumulator_set<size_t,
        features<tag::sum>> objects;
};

} // namespace uwr::mem