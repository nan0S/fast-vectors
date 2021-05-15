#include "static_vector_tests.hpp"

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

TYPED_TEST(StaticVectorTests, CopyConstructor) {
    auto v1 = this->GetVectorOfSize(5);
    static_vector<TypeParam, this->C> v2(v1);
    
    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(StaticVectorTests, MoveConstructor) {
    const auto val = this->GetValue(10);
    const auto size = 5;
    static_vector<TypeParam, this->C> v1(size, val);
    static_vector<TypeParam, this->C> v2(std::move(v1));
    
    EXPECT_EQ(v2.size(), size);
    for (const auto& x : v2)
        EXPECT_EQ(x, val);
}

TYPED_TEST(StaticVectorTests, InitializerListConstructor) {
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(1), this->GetValue(2),
        this->GetValue(3), this->GetValue(4), this->GetValue(5)
    };
    static_vector<TypeParam, this->C> v(il);

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(StaticVectorTests, CopyOperator) {
    auto v1 = this->GetVectorOfSize(5);
    static_vector<TypeParam, this->C> v2(3);
    v2 = v1;

    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(StaticVectorTests, MoveOperator) {
    const auto val = this->GetValue(10);
    const auto size = 5;
    static_vector<TypeParam, this->C> v1(size, val);
    static_vector<TypeParam, this->C> v2(3);
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), size);
    for (typename TestFixture::size_type i = 0; i < size; ++i)
        EXPECT_EQ(v2[i], val);
}

TYPED_TEST(StaticVectorTests, InitializerListOperator) {
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    static_vector<TypeParam, this->C> v(3);
    v = il;

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(StaticVectorTests, BeginEndInEmptyVector) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(StaticVectorTests, BeginEndInEmptyConstVector) {
    const static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(StaticVectorTests, BeginEndMarkSize) {
    static_vector<TypeParam, this->C> v(5);
    
    EXPECT_EQ(std::distance(v.begin(), v.end()), v.size());
    EXPECT_EQ(std::distance(v.cbegin(), v.cend()), v.size());
    EXPECT_EQ(std::distance(v.rbegin(), v.rend()), v.size());
    EXPECT_EQ(std::distance(v.crbegin(), v.crend()), v.size());
}

TYPED_TEST(StaticVectorTests, IteratorsPointCorrectly) {
    const typename TestFixture::size_type size = 5;
    auto v = this->GetVectorOfSize(size);
    int i;
    
    i = 0;
    for (auto it = v.begin(); it != v.end(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(i));
    EXPECT_EQ(i, v.size());

    i = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(i));
    EXPECT_EQ(i, v.size());

    i = 0;
    for (auto it = v.rbegin(); it != v.rend(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(size - i - 1));
    EXPECT_EQ(i, v.size());

    i = 0;
    for (auto it = v.crbegin(); it != v.crend(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(size - i - 1));
    EXPECT_EQ(i, v.size());
}

TYPED_TEST(StaticVectorTests, IsSizeChanging) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.size(), 0);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 1);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 2);
    v.clear();
    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(StaticVectorTests, IsMaxSizeConstantAndCorrect) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.max_size(), this->C);
    v.resize(5);
    EXPECT_EQ(v.max_size(), this->C);
}

TYPED_TEST(StaticVectorTests, IsCapacityConstantAndCorrect) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.capacity(), this->C);
    v.resize(5);
    EXPECT_EQ(v.capacity(), this->C);
}

TYPED_TEST(StaticVectorTests, IsVectorInitiallyEmpty) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(StaticVectorTests, IsNonEmptyVectorNonEmpty) {
    static_vector<TypeParam, this->C> v(5);
    
    EXPECT_FALSE(v.empty());
}

TYPED_TEST(StaticVectorTests, DoesResizeChangeSize) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.size(), 0);
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    v.resize(3);
    EXPECT_EQ(v.size(), 3);
}

TYPED_TEST(StaticVectorTests, ResizeDefaultConstruct) {
    static_vector<TypeParam, this->C> v;

    v.resize(5);
    for (const auto& x : v)
        EXPECT_EQ(x, TypeParam());
}

TYPED_TEST(StaticVectorTests, ResizeWithFill) {
    static_vector<TypeParam, this->C> v;
    const auto val = this->GetValue(13);

    v.resize(5, val);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(StaticVectorTests, Reserve) {
    static_vector<TypeParam, this->C> v;
    const auto initial_size = v.size();
    
    for (typename TestFixture::size_type i = 1; i <= this->C; ++i) {
        v.reserve(i);
        EXPECT_EQ(v.size(), initial_size);
        EXPECT_GE(v.capacity(), i);
    }
}

TYPED_TEST(StaticVectorTests, ShrinkToFit) {
    static_vector<TypeParam, this->C> v(5);
    v.shrink_to_fit();

    EXPECT_GE(v.capacity(), v.size());
}

TYPED_TEST(StaticVectorTests, AccessOperators) {
    auto v = this->GetVectorOfSize(5);

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], this->GetValue(i));
        EXPECT_EQ(v.at(i), this->GetValue(i));
    }

    EXPECT_THROW(v.at(-1), std::out_of_range);
    EXPECT_THROW(v.at(v.size()), std::out_of_range);
}

TYPED_TEST(StaticVectorTests, FrontBack) {
    auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(StaticVectorTests, ConstFrontBack) {
    const auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(StaticVectorTests, Data) {
    auto v = this->GetVectorOfSize(5);
    TypeParam* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(StaticVectorTests, ConstData) {
    const auto v = this->GetVectorOfSize(5);
    const TypeParam* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(StaticVectorTests, RangeAssign) {
    const TypeParam a[] = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    const int s = sizeof(a) / sizeof(a[0]);
    auto v = this->GetVectorOfSize(5);

    v.assign(a, a + s);

    EXPECT_EQ(v.size(), s);
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], a[i]);
}

TYPED_TEST(StaticVectorTests, RangeFill) {
    auto v = this->GetVectorOfSize(3);
    const auto val = this->GetValue(rand());
    

    v.assign()
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
