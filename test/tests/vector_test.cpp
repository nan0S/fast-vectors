#include "uwr/vector.hpp"

#include <boost/container/vector.hpp>

#define DONT_COMPARE

template<class T>
using tested_vector_t = uwr::vector<T>;
template<class T>
using compare_vector_t = boost::container::vector<T>;

template<class T>
using gf_vector = uwr::vector<T, uwr::growth_factor_1_5>;
static constexpr uwr::mem::len_t S = 5; 

#include "vector_test_base.hpp"

template<class V>
class VectorTestFixture : public VectorTestBaseFixture<V> {
};

TYPED_TEST_SUITE(VectorTestFixture, TestedTypes, TypeNames);

TYPED_TEST(VectorTestFixture, CopyConstructWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);

   gf_vector<typename TypeParam::value_type> v2(v);
}

TYPED_TEST(VectorTestFixture, MoveConstructWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);

   gf_vector<typename TypeParam::value_type> v2(std::move(v));
}

TYPED_TEST(VectorTestFixture, CopyAssignWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);
   gf_vector<typename TypeParam::value_type> v2;

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

TYPED_TEST(VectorTestFixture, ReferenceAssignWithDifferentGF) {
   auto v = this->GetVectorOfSize(S);

   gf_vector<typename TypeParam::value_type>& v2 = v;
   const gf_vector<typename TypeParam::value_type>& v3 = v;

   (void)v2;
   (void)v3;
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
