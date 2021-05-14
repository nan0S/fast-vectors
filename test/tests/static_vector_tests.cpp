#include "static_vector_tests.hpp"
#include <static_vector.hpp>
using namespace uwr;

template<>
void
StaticVectorTests<test_type>::SetUp() {
    test_type::instances = 0;
}

template<>
void
StaticVectorTests<test_type>::TearDown() {
    EXPECT_EQ(test_type::instances, 0);
}

template<>
std::string
StaticVectorTests<std::string>::GetValue(int id) {
    return "test" + std::to_string(id);
}

template<>
std::array<int, 10>
StaticVectorTests<std::array<int, 10>>::GetValue(int id) {
    std::array<int, 10> a;
    a.fill(id);

    return a;
}

TYPED_TEST(StaticVectorTests, DefaultConstructor) {
    static_vector<TypeParam, this->C> v;

    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(StaticVectorTests, CountConstructor) {
    static_vector<TypeParam, this->C> v(5);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, TypeParam());
}

TYPED_TEST(StaticVectorTests, CountValueConstructor) {
    TypeParam val;
    static_vector<TypeParam, this->C> v(5, val);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(StaticVectorTests, RangeConstructor) {
    const TypeParam a[] = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    const int s = sizeof(a) / sizeof(a[0]);
    static_vector<TypeParam, this->C> v(a, a + s);

    EXPECT_EQ(v.size(), s);
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], a[i]);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
