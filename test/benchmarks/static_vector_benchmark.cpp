#include "benchmark.hpp"

#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <test_type/test_type.hpp>

static constexpr uint C = 500000;

template<class T>
using ustatic_vector = uwr::static_vector<T, C>;
template<class T>
using bstatic_vector = boost::container::static_vector<T, C>;

int main() {
    test_type::do_print = false;

    info("static_vector benchmark:", 0);

    // push_back_benchmark<bstatic_vector<int>>(
        // "boost::static_vector", C, 1000);
    // push_back_benchmark<ustatic_vector<int>>(
        // "uwr::static_vector", C, 1000);

    info("<int>", 1);
    experiment<bstatic_vector, int>(
        "boost::static_vector", 500, 20);
    experiment<ustatic_vector, int>(
        "uwr::static_vector", 500, 20);

    info("<std::string>", 1);
    experiment<bstatic_vector, std::string>(
        "boost::static_vector", 300, 20);
    experiment<ustatic_vector, std::string>(
        "uwr::static_vector", 300, 20);

    info("<test_type>", 1);
    experiment<bstatic_vector, test_type>(
        "boost::static_vector", 300, 20);
    experiment<ustatic_vector, test_type>(
        "uwr::static_vector", 300, 20);

    info("<int, std::string>", 1);
    experiment<bstatic_vector, int, std::string>(
        "boost::static_vector", 200, 20);
    experiment<ustatic_vector, int, std::string>(
        "uwr::static_vector", 200, 20);

    info("<int, std::string, std::array<int, 10>>", 1);
    experiment<bstatic_vector, int, std::string, std::array<int, 10>>(
        "boost::static_vector>", 100, 30);
    experiment<ustatic_vector, int, std::string, std::array<int, 10>>(
        "uwr::static_vector>", 100, 30);

    return 0;
}
