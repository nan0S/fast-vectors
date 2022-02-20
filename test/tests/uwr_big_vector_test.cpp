#include "uwr/vector.hpp"
#include "uwr/allocator/big_allocator.hpp"

#include <boost/container/vector.hpp>

#define DONT_COMPARE

template<class T>
using tested_vector_t = uwr::vector<T, uwr::default_growth_factor, uwr::mem::big_allocator<T>>;
template<class T>
using compare_vector_t = boost::container::vector<T>;

#include "vector_test_base.hpp"

template<class T>
class VectorTestFixture : public VectorTestBaseFixture<T> {
};

TYPED_TEST_SUITE(VectorTest, TestedTypes, TypeNames);

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
