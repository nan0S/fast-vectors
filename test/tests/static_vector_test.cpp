#include <static_vector.hpp>
#include <boost/container/static_vector.hpp>

static constexpr uwr::len_t C = 10;
template<class T>
using tested_vector_t = uwr::static_vector<T, C>;
template<class T>
using compare_vector_t = boost::container::static_vector<T, C>;

#include "vector_test_base.hpp"

template<class V>
class StaticVectorTestFixture : public VectorTestBaseFixture<V> {
};

TYPED_TEST_SUITE(StaticVectorTestFixture, TestedTypes, TypeNames);

TYPED_TEST(StaticVectorTestFixture, IsMaxSizeConstantAndCorrect) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.max_size(), C);

    v.resize(5);

    EXPECT_EQ(v.max_size(), C);
}

TYPED_TEST(StaticVectorTestFixture, IsCapacityConstantAndCorrect) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.capacity(), C);

    v.resize(5);

    EXPECT_EQ(v.capacity(), C);
}

TYPED_TEST(StaticVectorTestFixture, PushBackByCopyToFilledVectorCausesException) {
    typename TestFixture::vector v(C);
    const auto val = this->GetValue(0);

    EXPECT_ANY_THROW(v.push_back(val));
}

TYPED_TEST(StaticVectorTestFixture, PushBackByMoveToFilledVectorCausesException) {
    typename TestFixture::vector v(C);

    EXPECT_ANY_THROW(v.push_back(this->GetValue(0)));
}

TYPED_TEST(StaticVectorTestFixture, EmplaceBackToFullVectorCausesException) {
    typename TestFixture::vector v(C);

    EXPECT_ANY_THROW(v.emplace_back(this->GetValue(3)));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
