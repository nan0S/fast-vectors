#include <vector.hpp>
#include <boost/container/vector.hpp>

template<class T>
using vector_t = uwr::vector<T>;

#include "vector_test_base.hpp"

template<class T>
class VectorTests : public VectorTestsBase<T> {
};

TYPED_TEST_SUITE(VectorTests, TestedTypes, TypeNames);

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
