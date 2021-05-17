#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>

static constexpr uwr::len_t C = 10;
template<class T>
using vector_t = uwr::static_vector<T, C>;

#include "vector_tests_base.hpp"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
