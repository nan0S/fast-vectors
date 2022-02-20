#include "uwr/vector.hpp"
#include "uwr/allocator/malloc_allocator.hpp"

#include <boost/container/vector.hpp>

#define DONT_COMPARE

template<class T>
using tested_vector_t = uwr::vector<T>;
template<class T>
using compare_vector_t = boost::container::vector<T>;

template<class T>
using gf_vector = uwr::vector<T, uwr::growth_factor_1_5>;
template<class T>
using gf_alloc_vector = uwr::vector<T, uwr::growth_factor_1_5, uwr::mem::malloc_allocator<T>>;

static constexpr uwr::mem::len_t S = 5; 

#include "vector_test_base.hpp"

template<class V>
class VectorTestFixture : public VectorTestBaseFixture<V> {
};

TYPED_TEST_SUITE(VectorTestFixture, TestedTypes, TypeNames);

TYPED_TEST(VectorTestFixture, CopyConstructWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);

   gf_alloc_vector<typename TypeParam::value_type> v2(v);
}

TYPED_TEST(VectorTestFixture, MoveConstructWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);

   gf_vector<typename TypeParam::value_type> v2(std::move(v));
}

TYPED_TEST(VectorTestFixture, CopyAssignWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);
   gf_alloc_vector<typename TypeParam::value_type> v2;

   v2 = v;
}

TYPED_TEST(VectorTestFixture, MoveAssignWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);
   gf_vector<typename TypeParam::value_type> v2;

   v2 = std::move(v);
}

TYPED_TEST(VectorTestFixture, SwapWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);
   gf_vector<typename TypeParam::value_type> v2;

   v.swap(v2);
}

TYPED_TEST(VectorTestFixture, StdSwapWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);
   gf_vector<typename TypeParam::value_type> v2;

   std::swap(v, v2);
}

TYPED_TEST(VectorTestFixture, ReferenceAssignWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);

   gf_vector<typename TypeParam::value_type>& v2 = v;
   const gf_vector<typename TypeParam::value_type>& v3 = v;

   (void)v2;
   (void)v3;
}

TYPED_TEST(VectorTestFixture, OperatorsWithDifferentGF) {
   auto v1 = this->GetVectorOfSize(S);
   gf_alloc_vector<typename TypeParam::value_type> v2 = v1;

   EXPECT_TRUE(v1 == v2);
   EXPECT_FALSE(v1 != v2);
   EXPECT_TRUE(v1 <= v2);
   EXPECT_FALSE(v1 < v2);
   EXPECT_TRUE(v1 >= v2);
   EXPECT_FALSE(v1 > v2);
#if CPP_ABOVE_17
   EXPECT_TRUE((v1 <=> v2) == 0);
#endif
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
