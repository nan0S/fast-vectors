#include <static_vector.hpp>

static constexpr uwr::len_t C = 10;
template<class T>
using vector_t = uwr::static_vector<T, C>;

#include "vector_tests_base.hpp"

template<class T>
class StaticVectorTests : public VectorTestsBase<T> {
};

TYPED_TEST_SUITE(StaticVectorTests, TestedTypes, TypeNames);

TYPED_TEST(StaticVectorTests, IsMaxSizeConstantAndCorrect) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.max_size(), C);

    v.resize(5);

    EXPECT_EQ(v.max_size(), C);
}

TYPED_TEST(StaticVectorTests, IsCapacityConstantAndCorrect) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.capacity(), C);

    v.resize(5);

    EXPECT_EQ(v.capacity(), C);
}

TYPED_TEST(StaticVectorTests, PushBackByCopyToFilledVectorCausesException) {
    typename TestFixture::vector v(C);
    const TypeParam val = this->GetValue(0);

    EXPECT_ANY_THROW(v.push_back(val));
}

TYPED_TEST(StaticVectorTests, PushBackByMoveToFilledVectorCausesException) {
    typename TestFixture::vector v(C);

    EXPECT_ANY_THROW(v.push_back(this->GetValue(0)));
}

TYPED_TEST(StaticVectorTests, EmplaceBackToFullVectorCausesException) {
    typename TestFixture::vector v(C);

    EXPECT_ANY_THROW(v.emplace_back(this->GetValue(3)));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
