#include "uwr/static_vector.hpp"

#include <boost/container/static_vector.hpp>

static constexpr uwr::len_t C = 10;
template<class T>
using tested_vector_t = uwr::static_vector<T, C>;
template<class T>
using compare_vector_t = boost::container::static_vector<T, C>;

static constexpr uwr::len_t C2 = 5;

#include "vector_test_base.hpp"

static_assert(C == VectorTestBaseFixture<tested_vector_t<int>>::C,
        "Capacity of static_vector should be the same as defined in VectorTestBaseFixture class!");

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

TYPED_TEST(StaticVectorTestFixture, CopyConstructVectorWithDifferentCapacity) {
    auto v = this->GetVectorOfSize(C2);

    uwr::static_vector<typename TypeParam::value_type, C2> v2(v);

    EXPECT_EQ(v2.size(), v.size());
    for (typename TestFixture::size_type i = 0; i < C2; ++i)
        EXPECT_EQ(v[i], v2[i]);
}

TYPED_TEST(StaticVectorTestFixture, MoveConstructVectorWithDifferentCapacity) {
    auto v = this->GetVectorOfSize(C2);

    uwr::static_vector<typename TypeParam::value_type, C2> v2(std::move(v));

    EXPECT_EQ(v2.size(), C2);
    for (typename TestFixture::size_type i = 0; i < C2; ++i)
        EXPECT_EQ(v2[i], this->GetValue(i));
}

TYPED_TEST(StaticVectorTestFixture, CopyAssignVectorWithDifferentCapacity) {
    auto v = this->GetVectorOfSize(C2);
    uwr::static_vector<typename TypeParam::value_type, C2> v2;

    v2 = v;

    EXPECT_EQ(v2.size(), v.size());
    for (typename TestFixture::size_type i = 0; i < C2; ++i)
        EXPECT_EQ(v[i], v2[i]);
}

TYPED_TEST(StaticVectorTestFixture, MoveAssignVectorWithDifferentCapacity) {
    auto v = this->GetVectorOfSize(C2);
    uwr::static_vector<typename TypeParam::value_type, C2> v2;

    v2 = std::move(v);

    EXPECT_EQ(v2.size(), C2);
    for (typename TestFixture::size_type i = 0; i < C2; ++i)
        EXPECT_EQ(v2[i], this->GetValue(i));
}

TYPED_TEST(StaticVectorTestFixture, SwapVectorWithDifferentCapacity) {
    auto v = this->GetVectorOfSize(C2);
    uwr::static_vector<typename TypeParam::value_type, C2> v2(C2);
    for (typename TestFixture::size_type i = 0; i < C2; ++i)
       v2[i] = this->GetValue(C2 - 1 - i);

    v.swap(v2);

    EXPECT_EQ(v.size(), C2);
    EXPECT_EQ(v2.size(), C2);
    for (typename TestFixture::size_type i = 0; i < C2; ++i)
       EXPECT_EQ(v[i], this->GetValue(C2 - 1 - i));
    for (typename TestFixture::size_type i = 0; i < C2; ++i)
       EXPECT_EQ(v2[i], this->GetValue(i));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
